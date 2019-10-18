#define _CRT_SECURE_NO_DEPRECATE

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <ctime>
#include <cctype>
#include <list>
#include <sdl/SDL.h>
#include <sdl/SDL_image.h>
#include <sdl/SDL_mixer.h>
#include "maths_cpp.h"

#define VERSION "Version 0.1"
#define SCREEN_WIDTH 426// resolution scene scales to. not window/screen 853
#define SCREEN_HEIGHT 240//480
#define SCREEN_BPP 32
#define FPS_MAX 120
#define BG_SPRITES 20
#define OBJ_SPRITES 50
#define MISC_SPRITES 25
#define MAX_FONTS 3
#define MAX_BUTTONS 9
#define MAX_SOUNDS 40
#define MAX_BGM 10
#define PI 3.141592654
#define DTIME 16.66
#define NADA -9999
#define NULL 0
#define BULLET_CAP 300 //220
#define NPC_MAX 15
#define DASH_MAX 200
#define CLSN 1
#define MAPX 53
#define MAPY 15

//-----------------------------------------------------------------------------
enum OBJ_TYPE {OBJ_NPC, OBJ_PROJ1, OBJ_PROJ2, OBJ_PARTICLE, OBJ_VFX, OBJ_BG};
enum CTRL_ENUM {C_DOWN = 0x0001, C_RIGHT = 0x0002, C_UP = 0x0004, C_LEFT = 0x0008,
C_SELECT = 0x0200, C_START = 0x0100, C_SHOOT = 0x0010, C_BOMB = 0x0020, C_BOOST = 0x0040,
C_BOOSTED = 0x4000};
enum SND_TYPE {SND_COMMON};
enum SCROLL_TYPE {HORI, VERT, BOTH};
enum ANIM_DATA {AN_OFFSET, AN_TIME, AN_DELAY, AN_FRAME, AN_TFRAME};
enum EN_TYPE {E_ZOOMER, E_PARTTIME, E_BIG, E_BRICK, E_MINE, E_FORM, E_SQUARE,
E_WALL, E_ZIG, E_CHOPPA, E_DIAMOND};
//-----------------------------------------------------------------------------
struct P_RECT
{
    short x, y;
    unsigned short w, h;
};//rectangle for many uses

struct P_POINT
{
    int x, y;
}; //one pixel

struct FIXED_ANIM
{
    short offset;
    short anim_time;
    short delay;
    unsigned short frame;
    short total_frames;
    SDL_Rect *anim_clip;
};//static animation

struct SPRITE
{
    unsigned short w, h;
    unsigned short mw, mh;
    unsigned short frames;
    SDL_Texture *spr;
}; //sprite data

struct CIF_HEAD
{
    char name[30];
    unsigned char type;
    unsigned short images;
    unsigned short w, h;
    unsigned short mw, mh;
};

struct PADDING
{
    char name[40];
    int type;
};

struct LUMP_HEAD
{
    int id;
    int type;
    int size;
};

struct SOUND
{
    char channel;
    char loop;
    Mix_Chunk *s;
};

struct MUSIC
{
    bool loop;
    unsigned char loop_pos;//where to go to when looping
    Mix_Music *m;
};

struct AUDIO
{
    SOUND common[MAX_SOUNDS];
    MUSIC bgm[MAX_BGM];
    int bgm_index;//current music playing
};

struct LOADER_DEF
{
    int id;
    char type;
    unsigned char index;
    char loop;//loop data for sound/music only
    unsigned char loop_pos;//where to go to when looping
};

struct SURFACES
{
    Uint32 bg_colors[3];
    SDL_Renderer *bg;
    SPRITE bg_spr[BG_SPRITES];
    SPRITE obj_spr[OBJ_SPRITES];//player gets 0 index
    SPRITE misc_spr[MISC_SPRITES];//effects ect.
    SPRITE font_spr[MAX_FONTS];
}; //all the sprites


struct FONT
{
    SDL_Rect ch[70];
};

struct P_LINE
{
	int x, y;
	double angle;
	int length;
	int size;
	Uint32 color;
};


struct OBJ_DATA
{
	int id;
	int nid;
	double x;
	double y;
	double angle;
	double speed;
};//data used to create vfx/projectiles
//-----------------------------------------------------------------------------
void LogIt(const char *str);

//-----------------------------------------------------------------------------





