/**
 * @brief VDrum: A virtual drum program, play drum using your webcam, originally written by
 * @author: Guo Xiaoyong
 */


#include <cstdio>
#include <ctime>
#include <cmath>
#include <cctype>

#include <sys/time.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_image.h>

#include <timer.h>



const int SCREEN_WIDTH  = 640;
const int SCREEN_HEIGHT = 480;

/* tracking parameters (in seconds) */
const double MHI_DURATION = 1;
const double MAX_TIME_DELTA = 0.5;
const double MIN_TIME_DELTA = 0.05;

// number of cyclic frame buffer used for motion detection (should, probably, depend on FPS)
const int N = 2;

// ring image buffer
IplImage *buf[256] ;
int last = 0;

// temporary images
IplImage *mhi = 0;           // MHI
IplImage *orient = 0;        // orientation
IplImage *mask = 0;          // valid orientation mask
IplImage *segmask = 0;       // motion segmentation map
IplImage *erodeImg = 0;      // motion segmentation map


/* SDL datastructures */
SDL_Surface *frameSur;
SDL_Surface *snareSur;
SDL_Surface *snareSurHit;
SDL_Surface *bassSur;
SDL_Surface *bassSurHit;
SDL_Surface *screen;
/* snare and kick sound */
Mix_Chunk *snare;
Mix_Chunk *kick;


// parameters:
//  img - input video frame
//  dst - resultant motion picture
//  args - optional parameters
void  update_mhi(IplImage* img, IplImage* dst, int diff_threshold)
{
    double timestamp = (double)clock() / CLOCKS_PER_SEC; // get current time in seconds
    CvSize size = cvSize(img->width, img->height); // get current frame size
    int i, j, idx1 = last, idx2;
    IplImage* silh;


    /* keep image mirror-like */
    cvFlip(img, NULL, 1);

    // allocate images at the beginning or
    // reallocate them if the frame size is changed
    if (!mhi || mhi->width != size.width || mhi->height != size.height)
    {
        for ( i = 0; i < N; i++ )
        {
            cvReleaseImage(&buf[i]);
            buf[i] = cvCreateImage(size, IPL_DEPTH_8U, 1);
            cvZero( buf[i] );
        }

        cvReleaseImage( &mhi );
        cvReleaseImage( &orient );
        cvReleaseImage( &segmask );
        cvReleaseImage( &mask );

        mhi      = cvCreateImage(size, IPL_DEPTH_32F, 1);
        orient   = cvCreateImage(size, IPL_DEPTH_32F, 1);
        segmask  = cvCreateImage(size, IPL_DEPTH_32F, 1);
        erodeImg = cvCreateImage(size, IPL_DEPTH_8U,  1);
        mask     = cvCreateImage(size, IPL_DEPTH_8U,  1);
        
        cvZero(mhi); // clear MHI at the beginning
    }

//    buf[last]= cvCloneImage(img);
    cvCvtColor(img, buf[last], CV_BGR2GRAY); // convert frame to grayscale

    idx2 = (last + 1) % N; // index of (last - (N-1))th frame
    last = idx2;
    silh = buf[idx2];

    cvAbsDiff(buf[idx1], buf[idx2], silh); // get difference between frames
//    cvShowImage( "Silhueta", silh );

    cvThreshold( silh, silh, diff_threshold, 255, CV_THRESH_BINARY ); // and threshold it
    cvUpdateMotionHistory(silh, mhi, timestamp, MHI_DURATION); // update MHI

    for (i = 0; i < img->height; i++)
    {
        for (j = 0; j < img->width; j++)
        {
            CvScalar result_color = cvGetAt(img,i,j);
            result_color.val[0] += cvGetAt(silh,i,j).val[0];
            cvSetAt(img,result_color,i,j);
        }
    }

    static int erode = 0;


//    cvShowImage( "Silhueta1", silh );
    cvCreateTrackbar("iterations", "Erode", &erode, 10, NULL);
    cvErode(silh, erodeImg, NULL, erode);
//    cvShowImage( "Erode", erodeImg);

    //printf("Timestamp %lf\n",timestamp);

    cvZero( dst );
    cvCvtPlaneToPix( 0, erodeImg, 0, 0, dst );
}



#include "sdlav.c"


int main()
{
    IplImage*  motion  = 0;
    CvCapture* capture = 0;
    SDL_Event event;

    if (SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
        exit(1);
    }

    LoadSound();
    LoadImages();
    atexit(SDL_Quit);

    screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 24, SDL_DOUBLEBUF|SDL_SWSURFACE);
    if (screen == NULL)
    {
        fprintf(stderr, "Unable to set video mode: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_WM_SetCaption("Virtual Drum", NULL);

    capture = cvCaptureFromCAM(0);

    if (!capture)
    {
        perror("capture failed!\n");
        exit(1);
    }


    for (;;)
    {
        while ( SDL_PollEvent( &event ) )
        {
            switch (event.type)
            {
            case SDL_KEYDOWN:
                printf("The %s key was pressed!\n", 
                                SDL_GetKeyName(event.key.keysym.sym));
                exit(0);
                break;
            case SDL_QUIT:
                printf("SDL_QUIT event received!\n");
                exit(0);
            }
        }


        IplImage* image;
        
        timer tm;
        tick(&tm);

        if (!cvGrabFrame(capture)) break;

        image = cvRetrieveFrame( capture );

        if ( image )
        {
            if ( !motion )
            {
                motion 
                = cvCreateImage(cvSize(image->width,image->height), 8, 3);
                cvZero(motion);
                motion->origin = image->origin;
            }
        }


        update_mhi(image, motion, 30);

        /*
        for (int y = 0; y < SCREEN_HEIGHT; y++)
        {
            for (int x = 0; x < SCREEN_WIDTH; x++)
            {
                Uint32 myPixel = ((int)cvGetAt(image, y, x).val[2]<<16) |
                                 ((int)cvGetAt(image, y, x).val[1]<<8) |
                                 ((int)cvGetAt(image, y, x).val[0] );
                PutPixel(screen, x, y, myPixel);
            }
        }*/

        SDL_Surface* tmp = SDL_CreateRGBSurfaceFrom((void*)image->imageData,
                        image->width,
                        image->height,
                        image->depth * image->nChannels,
                        image->widthStep,
                        0xff0000, 0x00ff00, 0x0000ff, 0);

        SDL_BlitSurface(tmp, NULL, screen, NULL);
        SDL_FreeSurface(tmp);


        DrawSprite( snareSur, screen, 
                    0, 0, 60,  60, snareSur->w, snareSur->h);
        DrawSprite( bassSur,  screen, 
                    0, 0, 400, 60, bassSur->w,  bassSur->h);


        int collisionPts = 0;
        static int snareLast = 0;
        static int myCount = 0;
        static int kickLast = 0;
        
        
        myCount++;

        for (int y=100; y<=200; y++)
        {
            for (int x=100; x<=200; x++)
            {
                if (cvGetAt(motion, x, y).val[1] == 255)
                {
                    collisionPts++;
                }
            }
        }

        if (collisionPts > 2000)
        {
            if (myCount-snareLast>=3)
            {
                snareLast = myCount;
                PlaySound(snare);
                DrawSprite(snareSurHit, screen, 
                           0, 0, 60, 60, snareSur->w, snareSur->h);
            }
        }

        int collisionPtsDrum = 0;
        for (int y = 100; y <= 200; y++) 
        {
            for (int x = 500; x <= 600; x++) 
            {
                if (cvGetAt(motion,y,x).val[1]==255) 
                {
                    collisionPtsDrum++;
                }
            }
        }

        if (collisionPtsDrum > 2000)
        {
            if (myCount-kickLast>=0)
            {
                kickLast = myCount;
                PlaySound(kick);
                DrawSprite(bassSurHit, screen, 
                           0, 0, 400, 60, bassSur->w, bassSur->h);
            }
        }

        SDL_Flip(screen);
        tock(&tm);
        PrintTimer(&tm);
    }

    cvReleaseCapture(&capture);

    return 0;
}



