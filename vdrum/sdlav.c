void DrawSprite( SDL_Surface* imageSurface,
                 SDL_Surface* screenSurface,
                 int srcX,  int srcY,
                 int dstX,  int dstY,
                 int width, int height )
{
    SDL_Rect srcRect;
    SDL_Rect dstRect;

    srcRect.x = srcX;
    srcRect.y = srcY;
    srcRect.w = width;
    srcRect.h = height;

    dstRect.x = dstX;
    dstRect.y = dstY;
    dstRect.w = width;
    dstRect.h = height;

    SDL_SetAlpha(imageSurface, SDL_SRCALPHA, 200);
    SDL_BlitSurface(imageSurface, &srcRect, screenSurface, &dstRect);
}


void LoadImages()
{
    snareSur = IMG_Load("snare.gif");
    if (snareSur == NULL)
    {
        fprintf(stderr, "Unable to load bitmap: %s\n", SDL_GetError());
        exit(1);
    }

    snareSurHit = IMG_Load("snare-hit.gif");
    if (snareSurHit == NULL)
    {
        fprintf(stderr, "Unable to load bitmap: %s\n", SDL_GetError());
        exit(1);
    }

    bassSur = IMG_Load("bass.gif");
    if (bassSur == NULL)
    {
        fprintf(stderr, "Unable to load bitmap: %s\n", SDL_GetError());
        exit(1);
    }

    bassSurHit = IMG_Load("bass-hit.gif");
    if (bassSurHit == NULL)
    {
        fprintf(stderr, "Unable to load bitmap: %s\n", SDL_GetError());
        exit(1);
    }
}



void LoadSound()
{
    int audio_rate = 22050;			//Frequency of audio playback
    Uint16 audio_format = AUDIO_S16SYS; 	//Format of the audio we're playing
    int audio_channels = 2;			//2 channels = stereo
    int audio_buffers = 4096;		//Size of the audio buffers in memory

    if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0)
    {
        fprintf(stderr, "Unable to initialize audio: %s\n", Mix_GetError());
        exit(1);
    }

    //Load our OGG file from disk
    snare = Mix_LoadWAV("snare.wav");
    if (snare == NULL)
    {
        fprintf(stderr, "Unable to load OGG file: %s\n", Mix_GetError());
        exit(1);
    }

    kick = Mix_LoadWAV("kick.wav");
    if (kick == NULL)
    {
        fprintf(stderr, "Unable to load OGG file: %s\n", Mix_GetError());
        exit(1);
    }
}


void PlaySound(Mix_Chunk* sound)
{
    int channel;
    channel = Mix_PlayChannel(-1, sound, 0);

    if (channel == -1)
    {
        fprintf(stderr, "Unable to play WAV file: %s\n", Mix_GetError());
        exit(1);
    }
}



void PutPixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch (bpp)
    {
    case 1:
        *p = pixel;
        break;
    case 2:
        *(Uint16 *)p = pixel;
        break;
    case 3:
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
        {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        }
        else
        {
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



