#include <ctime>
#include <cmath>
#include <ctype>
#include <stdio.h>

#include <sys/time.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_image.h>

#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480


// various tracking parameters (in seconds)
const double MHI_DURATION = 1;
const double MAX_TIME_DELTA = 0.5;
const double MIN_TIME_DELTA = 0.05;
// number of cyclic frame buffer used for motion detection
// (should, probably, depend on FPS)
const int N = 2;

// ring image buffer
IplImage **buf = 0;
int last = 0;

// temporary images
IplImage *mhi = 0; // MHI
IplImage *orient = 0; // orientation
IplImage *mask = 0; // valid orientation mask
IplImage *segmask = 0; // motion segmentation map
IplImage *erodeImg = 0; // motion segmentation map
CvMemStorage* storage = 0; // temporary storage

//SDL

SDL_Surface *frameSur;
SDL_Surface *snareSur;
SDL_Surface *snareSurHit;
SDL_Surface *bassSur;
SDL_Surface *bassSurHit;
SDL_Surface *screen;


Mix_Chunk *snare;			//Pointer to our music, in memory
Mix_Chunk *kick;			//Pointer to our music, in memory

static struct timeval start_time;

void tick()
{
    gettimeofday(&start_time, NULL);
}

float tock(int exp)
{
    struct timeval end_time;
    gettimeofday(&end_time, NULL);
    int micros = 1000000 * (end_time.tv_sec - start_time.tv_sec) +
                 end_time.tv_usec - start_time.tv_usec;
    int mult = -6 - exp;
    return micros * pow(10, mult);
}

void drawSprite(SDL_Surface* imageSurface,
                SDL_Surface* screenSurface,
                int srcX, int srcY,
                int dstX, int dstY,
                int width, int height)
{
    SDL_Rect srcRect;
    srcRect.x = srcX;
    srcRect.y = srcY;
    srcRect.w = width;
    srcRect.h = height;
    SDL_Rect dstRect;
    dstRect.x = dstX;
    dstRect.y = dstY;
    dstRect.w = width;
    dstRect.h = height;
    SDL_SetAlpha(imageSurface, SDL_SRCALPHA, 200);
    SDL_BlitSurface(imageSurface, &srcRect, screenSurface, &dstRect);
}


int initVideo(Uint32 flags = SDL_DOUBLEBUF | SDL_SWSURFACE) {

    atexit(SDL_Quit); // Clean it up nicely :)

   //flags = SDL_DOUBLEBUF | SDL_FULLSCREEN;


    // fullscreen can be toggled at run time :) any you might want to change the flags with params?
    //set the main screen to SCREEN_WIDTHxSCREEN_HEIGHT with a colour depth of 16:
    screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 24, flags);
    if (screen == NULL) {
        fprintf(stderr, "Unable to set video mode: %s\n", SDL_GetError());
        return false;
    }
    return true;
}

void loadImages() {
    snareSur = IMG_Load("snare.gif");
    if (snareSur == NULL) {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
        return;
    }
    snareSurHit = IMG_Load("snare-hit.gif");
    if (snareSurHit == NULL) {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
        return;
    }
    bassSur = IMG_Load("bass.gif");
    if (bassSur == NULL) {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
        return;
    }
    bassSurHit = IMG_Load("bass-hit.gif");
    if (bassSurHit == NULL) {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
        return;
    }

}



void loadSound()
{
    int audio_rate = 22050;			//Frequency of audio playback
    Uint16 audio_format = AUDIO_S16SYS; 	//Format of the audio we're playing
    int audio_channels = 2;			//2 channels = stereo
    int audio_buffers = 4096;		//Size of the audio buffers in memory

    if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0)
    {
        printf("Unable to initialize audio: %s\n", Mix_GetError());
        return;
    }
    //Load our OGG file from disk
    snare = Mix_LoadWAV("snare.wav");
    if (snare == NULL)
    {
        printf("Unable to load OGG file: %s\n", Mix_GetError());
        return;
    }
    kick = Mix_LoadWAV("snare.wav");
    if (kick == NULL)
    {
        printf("Unable to load OGG file: %s\n", Mix_GetError());
        return;
    }
}


void playSound(Mix_Chunk* sound) 
{
    int channel;
    channel = Mix_PlayChannel(-1, sound, 0);

    if (channel == -1) 
    {
        fprintf(stderr, "Unable to play WAV file: %s\n", Mix_GetError());
    }

}


void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch (bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}



// parameters:
//  img - input video frame
//  dst - resultant motion picture
//  args - optional parameters
void  update_mhi( IplImage* img, IplImage* dst, int diff_threshold )
{
    double timestamp = (double)clock()/CLOCKS_PER_SEC; // get current time in seconds
    CvSize size = cvSize(img->width,img->height); // get current frame size
    int i,j, idx1 = last, idx2;
    IplImage* silh;
    CvSeq* seq;
    CvRect comp_rect;
    double count;
    double angle;
    CvPoint center;
    double magnitude;
    CvScalar color;
    cvFlip(img,img,1);

    // allocate images at the beginning or
    // reallocate them if the frame size is changed
    if ( !mhi || mhi->width != size.width || mhi->height != size.height ) {
        if ( buf == 0 ) {
            buf = (IplImage**)malloc(N*sizeof(buf[0]));
            memset( buf, 0, N*sizeof(buf[0]));
        }

        for ( i = 0; i < N; i++ ) {
            cvReleaseImage( &buf[i] );
            buf[i] = cvCreateImage( size, IPL_DEPTH_8U, 1);
            cvZero( buf[i] );
        }
        cvReleaseImage( &mhi );
        cvReleaseImage( &orient );
        cvReleaseImage( &segmask );
        cvReleaseImage( &mask );

        mhi = cvCreateImage( size, IPL_DEPTH_32F, 1 );
        cvZero( mhi ); // clear MHI at the beginning
        orient = cvCreateImage( size, IPL_DEPTH_32F, 1 );
        segmask = cvCreateImage( size, IPL_DEPTH_32F, 1 );
        erodeImg = cvCreateImage(size, IPL_DEPTH_8U, 1);
        mask = cvCreateImage( size, IPL_DEPTH_8U, 1 );
    }

//    buf[last]= cvCloneImage(img);
    cvCvtColor( img, buf[last], CV_BGR2GRAY ); // convert frame to grayscale

    idx2 = (last + 1) % N; // index of (last - (N-1))th frame
    last = idx2;

    silh = buf[idx2];

    cvAbsDiff( buf[idx1], buf[idx2], silh ); // get difference between frames
//    cvShowImage( "Silhueta", silh );

    cvThreshold( silh, silh, diff_threshold, 255, CV_THRESH_BINARY ); // and threshold it
    cvUpdateMotionHistory( silh, mhi, timestamp, MHI_DURATION ); // update MHI

    for ( i=0;i<img->height;i++) {
        for (j=0;j<img->width;j++) {
            CvScalar result_color= cvGetAt(img,i,j);
            result_color.val[0]+=cvGetAt(silh,i,j).val[0];
            cvSetAt(img,result_color,i,j);
        }
    }
    static int erode = 0;


//    cvShowImage( "Silhueta1", silh );
    cvCreateTrackbar("iterations", "Erode", &erode, 10, NULL);
    cvErode(silh, erodeImg,NULL, erode);
//    cvShowImage( "Erode", erodeImg);


    printf("Timestamp %lf\n",timestamp);

    cvZero( dst );
    cvCvtPlaneToPix( 0, erodeImg, 0, 0, dst );

}

void initSDL() 
{
    if ( SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO) < 0 ) 
    {
        fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
        exit(1);
    }

    loadSound();
    loadImages();
}




int main(int argc, char** argv)
{
    IplImage* motion = 0;
    CvCapture* capture = 0;
    SDL_Event event;

    initSDL();
    initVideo();
    //initVideo(SDL_DOUBLEBUF);



    if ( argc == 1 || (argc == 2 && strlen(argv[1]) == 1 && isdigit(argv[1][0])))
        capture = cvCaptureFromCAM( argc == 2 ? argv[1][0] - '0' : 0 );
    else if ( argc == 2 )
        capture = cvCaptureFromFile( argv[1] );


    if ( capture )
    {
//  cvNamedWindow( "Motion", 1 );
//	cvNamedWindow( "Silhueta", 1 );
//	cvNamedWindow( "Silhueta1", 1 );
//  cvNamedWindow( "Original",1);
//  cvNamedWindow( "Erode",1);

    for (;;)
    {
            while ( SDL_PollEvent( &event ) ) {

                switch (event.type) {
                case SDL_KEYDOWN:
                    printf("The %s key was pressed!\n",
                           SDL_GetKeyName(event.key.keysym.sym));
                    atexit(SDL_Quit);
//				SDL_FreeSurface(image);
                    exit(0);
                    break;
                case SDL_QUIT:
                    atexit(SDL_Quit);
//				SDL_FreeSurface(image);
                    exit(0);
                }
            }

            IplImage* image;
            tick();
            if ( !cvGrabFrame( capture ))
                break;
            image = cvRetrieveFrame( capture );
            printf("%f\n",tock(-3));

            if ( image )
            {
                if ( !motion )
                {
                    motion = cvCreateImage( cvSize(image->width,image->height), 8, 3 );
                    cvZero( motion );
                    motion->origin = image->origin;
                }
            }



            update_mhi( image, motion, 30 );

            for (int y=0;y<SCREEN_HEIGHT;y++) 
            {
                for (int x=0;x<SCREEN_WIDTH;x++) {
                    Uint32 myPixel = ((int)cvGetAt(image,y,x).val[2]<<16) |
                                     ((int)cvGetAt(image,y,x).val[1]<<8) |
                                     ((int)cvGetAt(image,y,x).val[0] );
                    putpixel(screen,x,y, myPixel );
                }
            }
            drawSprite(snareSur,screen,0,0,60,60,snareSur->w,snareSur->h);
            drawSprite(bassSur,screen,0,0,400,60,bassSur->w,bassSur->h);


// cvShowImage( "Motion", motion );

            CvFont font;
            double hScale=1.0;
            double vScale=1.0;
            int    lineWidth=1;
            cvInitFont(&font,CV_FONT_HERSHEY_SIMPLEX|CV_FONT_ITALIC, hScale,vScale,0,lineWidth);

            /* cvRectangle(image, cvPoint(100,100), cvPoint(200,200), cvScalar(0,255,0), -1);
            cvRectangle(image, cvPoint(500,100), cvPoint(600,200), cvScalar(0,0,255), -1);*/

            char numCol[200];
            int collisionPts = 0;
            static int snareLast = 0;
            static int myCount = 0;
            static int kickLast = 0;
            myCount++;

            for (int y=100;y<=200;y++) {
                for (int x=100;x<=200;x++) {
                    if (cvGetAt(motion,x,y).val[1]==255) {
                        collisionPts++;
                    }
                }
            }

            if (collisionPts>2000) {
                if (myCount-snareLast>=3) {
                    snareLast=myCount;

                    playSound(snare);
                    drawSprite(snareSurHit,screen,0,0,60,60,snareSur->w,snareSur->h);
                    //cvRectangle(image, cvPoint(100,100), cvPoint(200,200), cvScalar(255,0,0), -1);
                }
            }

            int collisionPtsDrum = 0;
            for (int y=100;y<=200;y++) {
                for (int x=500;x<=600;x++) {
                    if (cvGetAt(motion,y,x).val[1]==255) {
                        collisionPtsDrum++;
                    }
                }
            }
            if (collisionPtsDrum>2000) {
                if (myCount-kickLast>=0) {
                    kickLast=myCount;
                    playSound(kick);
                    drawSprite(bassSurHit,screen,0,0,400,60,bassSur->w,bassSur->h);
                    //cvRectangle(image, cvPoint(500,100), cvPoint(600,200), cvScalar(255,0,0), -1);
                }
            }



            sprintf(numCol,"Colid: %4d",collisionPts);

//    cvPutText(image,numCol, cvPoint(0,30),&font,cvScalar(0,0,255,0));
//    cvShowImage( "Original",image);
//    SDL_BlitSurface(image, &src, screen, &dest);

            SDL_Flip(screen);

            if ( cvWaitKey(10) >= 0 )
                break;

        }
        cvReleaseCapture( &capture );
        cvDestroyWindow( "Motion" );
    }


    return EXIT_SUCESSFUL;
}



