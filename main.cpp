#include "common.h"
#include "objects.h"

ScreenOffset scr_off;//used for screen variables
int debug;
SDL_Window *window;

const char Author[] = "Lexofase Engine ©2012 Patrick Ching II";

void PMain(SDL_Joystick *j, SURFACES &ss);
//-----------------------------------------------------------------------------
bool StartUp(SDL_Joystick *j, SURFACES &s)
{
	int width, height;
    Uint8  vid_bpp;
    Uint32 vid_flags;
	std::ofstream out;

	out.open("syslog.txt");//reset log
	out.close();

	LogIt("Game");
    LogIt(VERSION);
    LogIt(__DATE__);
    LogIt(__TIME__);
    LogIt("Initializing Engine...");

	LogIt("Initializing SDL...");
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_JOYSTICK) < 0)//start check
    {
		LogIt("Fail!");
		return false;
    }

	IMG_Init(IMG_INIT_PNG);
	SDL_ShowCursor(SDL_DISABLE);

	width = 852;
    height = 480;
    vid_bpp = SCREEN_BPP;
    vid_flags = 0;//(SDL_SWSURFACE|SDL_ANYFORMAT|SDL_HWPALETTE);

	LogIt("Get video...");

    window = SDL_CreateWindow( "ewuofwijo", SDL_WINDOWPOS_CENTERED,
				SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);//_WINDOW_SHOWN  _FULLSCREEN_DESKTOP
	s.bg = SDL_CreateRenderer(window, -1, (SDL_RENDERER_ACCELERATED|SDL_RENDERER_TARGETTEXTURE));

    if (s.bg == 0)
    {
        LogIt("Video setup failed.");
        return false;
    }
    LogIt("OK!");

	LogIt("Target Render?");
	(!(SDL_RenderTargetSupported(s.bg)) ? LogIt("No!") : LogIt("Yes!"));


	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, 0);
	SDL_RenderSetLogicalSize(s.bg, SCREEN_WIDTH, SCREEN_HEIGHT);

	LogIt("Get Audio...");
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
    {
		LogIt(Mix_GetError());
        return false;
    }

	if (Mix_Init(MIX_INIT_MOD) == -1)
	{
		LogIt(Mix_GetError());
        return false;
    }

	LogIt("OK!");

    Mix_VolumeMusic(0);//128
    Mix_Volume(-1, MIX_MAX_VOLUME);
	Mix_AllocateChannels(32);

	if (SDL_NumJoysticks() > 0)
    {
		LogIt("Get Joystick...");
		j = SDL_JoystickOpen(0);

        if (j == 0)
			LogIt("Unable to open joystick.");
		else
			LogIt(SDL_JoystickName(j));
    }

	fclose(stdout);//get rid of these files because I don't need them
    fclose(stderr);
    remove("stdout.txt");
    remove("stderr.txt");

	LogIt("Get Some!");

    return true;
}
//-----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	SURFACES s;
	SDL_Joystick *j = 0;

	if (StartUp(j, s))
		PMain(j, s);

	Mix_CloseAudio();
	SDL_Quit();

    return 0;
}
//-----------------------------------------------------------------------------



