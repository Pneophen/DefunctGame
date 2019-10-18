#include "common.h"
#include "objects.h"

extern ScreenOffset scr_off;
extern int debug;
extern SDL_Window *window;

void Render(SURFACES &s, OBJECTS &o, FONT f[]);
bool LoadSprite(int id, SPRITE *final, SDL_Renderer *rend);
bool LoadSound(int id, SOUND *final);
bool LoadMusic(int id, MUSIC *final);
void PlaySound(AUDIO &a, unsigned int type, unsigned int index, int loops);
void PlayBgm(AUDIO &a, unsigned int index);
void SetFont(unsigned short width, unsigned short height, FONT *f);
void PlayerActions(OBJECTS &o, ObjManFx &om, short &pad, AUDIO &a);
void ProjectileActions(OBJECTS &o, ObjManFx &om, AUDIO &a);
void VfxActions(OBJECTS &o, ObjManFx &om);
void NpcActions(OBJECTS &o, ObjManFx &om, AUDIO a);
//void NpcSpawn(NpcHead &n);
void ProcessProj(ObjManFx &om, ProjectileHead &ph);
void ProcessVfx(ObjManFx &om, VfxHead &vh);
void GenLand(int landed[][15]);
void InitLand(int landed[][15]);

//-----------------------------------------------------------------------------
void DebugLoad(OBJECTS &o, SURFACES &s, AUDIO &a)
{
	LoadSprite(0, &(s.obj_spr[0]), s.bg);//p1
    LoadSprite(1, &(s.misc_spr[0]), s.bg);//bullets
    LoadSprite(2, &(s.misc_spr[1]), s.bg);//p1 after image
    LoadSprite(3, &(s.misc_spr[2]), s.bg);//explodes
    LoadSprite(13, &(s.misc_spr[3]), s.bg);//explodes 2
    LoadSprite(16, &(s.misc_spr[4]), s.bg);//big bullet
    LoadSprite(21, &(s.misc_spr[5]), s.bg);//bars
    LoadSprite(22, &(s.misc_spr[6]), s.bg);//lens flare *need to fix too big*
    LoadSprite(23, &(s.misc_spr[7]), s.bg);//b shot bar
    LoadSprite(24, &(s.misc_spr[8]), s.bg);//uber shot
    LoadSprite(25, &(s.misc_spr[9]), s.bg);//uber shot less
    LoadSprite(26, &(s.misc_spr[10]), s.bg);//uber shot tail
    LoadSprite(27, &(s.misc_spr[11]), s.bg);//uber shot ring
    LoadSprite(28, &(s.misc_spr[12]), s.bg);//b shot burst
    LoadSprite(30, &(s.misc_spr[13]), s.bg);//die splode
    LoadSprite(34, &(s.misc_spr[14]), s.bg);//hit zone
    LoadSprite(37, &(s.misc_spr[15]), s.bg);//squa part
    LoadSprite(42, &(s.misc_spr[16]), s.bg);//noise filter

    /*LoadSprite(4, &(s.bg_spr[0]), s.bg);//
    LoadSprite(5, &(s.bg_spr[1]), s.bg);//
    LoadSprite(6, &(s.bg_spr[2]), s.bg);//
    LoadSprite(7, &(s.bg_spr[3]), s.bg);//
    LoadSprite(8, &(s.bg_spr[4]), s.bg);//
    LoadSprite(9, &(s.bg_spr[5]), s.bg);//
    LoadSprite(10, &(s.bg_spr[6]), s.bg);//*/

    LoadSprite(11, &(s.obj_spr[1]), s.bg);//e1
    LoadSprite(14, &(s.obj_spr[2]), s.bg);//e2
    LoadSprite(15, &(s.obj_spr[3]), s.bg);//e3
    LoadSprite(17, &(s.obj_spr[4]), s.bg);//e4
    LoadSprite(18, &(s.obj_spr[5]), s.bg);//brick
    LoadSprite(20, &(s.obj_spr[6]), s.bg);//e5
    LoadSprite(29, &(s.obj_spr[7]), s.bg);//pow up
    LoadSprite(33, &(s.obj_spr[8]), s.bg);//kill screen
    LoadSprite(35, &(s.obj_spr[9]), s.bg);//e6
    LoadSprite(36, &(s.obj_spr[10]), s.bg);//e7
    LoadSprite(38, &(s.obj_spr[11]), s.bg);//e9
    LoadSprite(39, &(s.obj_spr[12]), s.bg);//e9a
    LoadSprite(40, &(s.obj_spr[13]), s.bg);//e9b

    LoadSprite(19, &(s.font_spr[0]), s.bg);//default text
    LoadSprite(32, &(s.font_spr[1]), s.bg);//chain count
    LoadSprite(41, &(s.font_spr[2]), s.bg);//chain text

    LoadSound(2000, &(a.common[0]));//p1 shot
    LoadSound(2001, &(a.common[1]));//explode
    LoadSound(2002, &(a.common[2]));//super pause
    LoadSound(2003, &(a.common[3]));//uber shot less
    LoadSound(2004, &(a.common[4]));//uber shot
    LoadSound(2005, &(a.common[5]));//confirm
    LoadSound(2007, &(a.common[6]));//get
    LoadSound(2006, &(a.common[7]));//deny
    LoadSound(2008, &(a.common[8]));//shield
    LoadSound(2009, &(a.common[9]));//explode2
    LoadSound(2010, &(a.common[10]));//explode3
    LoadSound(2011, &(a.common[11]));//explode boss
    LoadSound(2012, &(a.common[12]));//got hit
    LoadSound(2013, &(a.common[13]));//die
    LoadSound(2014, &(a.common[14]));//danger tone
    LoadSound(2015, &(a.common[15]));//boss danger
    LoadSound(2018, &(a.common[18]));//v hard mode
    LoadSound(2019, &(a.common[19]));//v lexodoss
    LoadSound(2020, &(a.common[20]));//v max chain
    LoadSound(2021, &(a.common[21]));//v normal mode
    LoadSound(2022, &(a.common[22]));//v okay
    LoadSound(2023, &(a.common[23]));//v power max
    LoadSound(2024, &(a.common[24]));//v wave init
    LoadSound(2025, &(a.common[25]));//v wave up
    LoadSound(2026, &(a.common[26]));//big ship shot
    LoadSound(2027, &(a.common[27]));//charge up
    LoadSound(2028, &(a.common[28]));//dash
    LoadSound(2029, &(a.common[29]));//life up


    //LoadMusic(4000, &(a.bgm[0]));

    a.bgm_index = 0; //setup test music
    a.bgm[(a.bgm_index)].loop = true;
    a.bgm[(a.bgm_index)].loop_pos = 8;

    //PlayBgm(a, 0); //play test music

    o.p1.Init(s.obj_spr[0]);
	
	o.cam.scene.x = 0;
    o.cam.scene.y = 0;
    o.cam.scene.w = SCREEN_WIDTH;
    o.cam.scene.h = SCREEN_HEIGHT;
	
	o.b.BgGenA(0);	
	o.b.BgGenA(1);
	o.b.BgGenA(2);
	
	return;
}

//-----------------------------------------------------------------------------
void InitVars(OBJECTS &o, SURFACES &s, AUDIO &a, FONT f[])
{
	/*s.bg_colors[0] = SDL_MapRGB(s.bg->format, 0x00, 0x00, 0x00);//black
    s.bg_colors[1] = SDL_MapRGB(s.bg->format, 0xff, 0xff, 0xff);//white
    s.bg_colors[2] = SDL_MapRGB(s.bg->format, 0x11, 0xff, 0x11);//color lime*/
	
	window = NULL;

	SetFont(5, 9, &(f[0]));//default font
    SetFont(11, 19, &(f[1]));//big red number font

	DebugLoad(o, s, a);

	scr_off.spd = 1;

	InitLand(o.map);
	GenLand(o.map);

	return;
}
//-----------------------------------------------------------------------------
void LogIt(const char *str)
{
    std::ofstream out;
	
	out.open("syslog.txt", std::ios::app);

	out << str << std::endl;

	out.close();

    return;
}
//-----------------------------------------------------------------------------
bool CheckKeyboard(SDL_Event &e, short &b) //assigns correct button bits if input is detected
{
    switch(e.type)
     {
         case SDL_KEYDOWN:
         if (e.key.keysym.sym == SDLK_ESCAPE)
            return false;
         if (e.key.keysym.sym == SDLK_LEFT)
         {
             b |= C_LEFT;
             b &= ~C_RIGHT;
         }
         if (e.key.keysym.sym == SDLK_RIGHT)
         {
             b |= C_RIGHT;
             b &= ~C_LEFT;
         }
         if (e.key.keysym.sym == SDLK_DOWN)
         {
             b |= C_DOWN;
             b &= ~C_UP;
         }
         if (e.key.keysym.sym == SDLK_UP)
         {
             b |= C_UP;
             b &= ~C_DOWN;
         }

         if (e.key.keysym.sym == SDLK_c)
            b |= C_SHOOT;
         if (e.key.keysym.sym == SDLK_x)
            b |= C_BOMB;
         if (e.key.keysym.sym == SDLK_z)
            b |= C_BOOST;
         if (e.key.keysym.sym == SDLK_RETURN)
            b |= C_START;

         break;

         case SDL_KEYUP:
         if (e.key.keysym.sym == SDLK_LEFT)
            b &= ~C_LEFT;
         if (e.key.keysym.sym == SDLK_RIGHT)
            b &= ~C_RIGHT;
         if (e.key.keysym.sym == SDLK_DOWN)
            b &= ~C_DOWN;
         if (e.key.keysym.sym == SDLK_UP)
            b &= ~C_UP;


         if (e.key.keysym.sym == SDLK_c)
            b &= ~C_SHOOT;
         if (e.key.keysym.sym == SDLK_x)
            b &= ~C_BOMB;
         if (e.key.keysym.sym == SDLK_z)
            b &= ~C_BOOST;
         if (e.key.keysym.sym == SDLK_RETURN)
            b &= ~C_START;

         break;
         default: ;
         break;
     }

    return true;
}
//-----------------------------------------------------------------------------
void CheckGamePad(SDL_Event &e, short &b)
{
    switch(e.type)
    {
        case SDL_JOYAXISMOTION:
            if (e.jaxis.axis == 0)
            {
                if (e.jaxis.value >= 7000)
                {
                    b |= C_RIGHT;
                    b &= ~C_LEFT;
                }
                else if (e.jaxis.value <= -7000)
                {
                    b |= C_LEFT;
                    b &= ~C_RIGHT;
                }
                else if (e.jaxis.value > -7000 && e.jaxis.value < 7000)
                {
                    b &= ~C_LEFT;
                    b &= ~C_RIGHT;
                }
            }
            else if (e.jaxis.axis == 1)
            {
                if (e.jaxis.value >= 7000)
                {
                    b |= C_DOWN;
                    b &= ~C_UP;
                }
                else if (e.jaxis.value <= -7000)
                {
                    b |= C_UP;
                    b &= ~C_DOWN;
                }
                else if (e.jaxis.value > -7000 && e.jaxis.value < 7000)
                {
                    b &= ~C_UP;
                    b &= ~C_DOWN;
                }
            }
        break;
        case SDL_JOYBUTTONDOWN:
            if (e.jbutton.button == 1)
                b |= C_SHOOT;
            if (e.jbutton.button == 0)
                b |= C_BOMB;
            if (e.jbutton.button == 2)
                b |= C_BOOST;
            if (e.jbutton.button == 9)
                b |= C_START;
        break;
        case SDL_JOYBUTTONUP:
            if (e.jbutton.button == 1)
                b &= ~C_SHOOT;
            if (e.jbutton.button == 0)
                b &= ~C_BOMB;
            if (e.jbutton.button == 2)
                b &= ~C_BOOST;
            if (e.jbutton.button == 9)
                b &= ~C_START;
        break;
        default: ;
        break;
    }
    return;
}
//-----------------------------------------------------------------------------
int FrameRate(int start_time, int fps_old)
{
    static int frames = 0;
    static int throttle = 200;
    static int f_time = 0;
    int fps = 0;
    Uint32 delay = 0;

    frames++;
    f_time++;

    if (f_time == 30)
    {
        f_time = 0;

        if ((fps_old < FPS_MAX) && (throttle < 200))//adjusts so desired fps is met(hopefully)
            throttle += 10;
        else if ((fps_old > FPS_MAX) && (throttle > 0))
            throttle -= 5;
    }

    fps = ((1000 * frames) / ((SDL_GetTicks() - start_time) + 1)); //calculate frames per second
    delay = static_cast<Uint32>(1000 / (FPS_MAX + throttle)); //calc delay value
    SDL_Delay(delay); //delay

    return fps;
}
//-----------------------------------------------------------------------------
void UpdateTimes(double &nt, double &d, double &n, double &a)
{
	nt = SDL_GetTicks();
	d = (nt - n);
	n = nt;
	a += d;

	return;
}
//-----------------------------------------------------------------------------
void SetFixedAnim(FIXED_ANIM &anim, unsigned short w, unsigned short h, unsigned short frames)
{
    unsigned short next = anim.offset;
	
	if (!frames)
		frames = 1;

	anim.anim_clip = (new (std::nothrow) SDL_Rect[frames]);
	
	if (anim.anim_clip == 0)
        LogIt("Insufficient memory for animation data.");

    anim.total_frames = frames;
    anim.delay = 1;
    anim.frame = 0;
    anim.anim_time = 0;

    for(int i = 0; i < frames; ++i)
    {
        anim.anim_clip[i].y = 0;
        anim.anim_clip[i].w = w;
        anim.anim_clip[i].h = h;
        anim.anim_clip[i].x = next;
        next += w;
    }

    return;
}
//-----------------------------------------------------------------------------
void FixedAnimInc(FIXED_ANIM &a)
{
    if (a.delay > -1) 
	{
		a.anim_time += 1;

		if (a.anim_time > (a.delay - 1))
		{
			a.anim_time = 0;
			a.frame += 1;

			if (a.frame > (a.total_frames - 1))
				a.frame = 0;
		}
	}

     return;
}
//-----------------------------------------------------------------------------
bool ClsnCheck(const P_RECT &a, const P_RECT &b)
{
    short top1 = a.y;
    short bottom1 = (a.y + a.h);
    short left1 = a.x;
    short right1 = (a.x + a.w);

    short top2 = b.y;
    short bottom2 = (b.y + b.h);
    short left2 = b.x;
    short right2 = (b.x + b.w);

    if (top1 > bottom2) //no top collision
        return false;

    if (bottom1 < top2) //no bottom
        return false;

    if (left1 > right2) //ect...
        return false;

    if (right1 < left2)
        return false;

    return true; //collision happened
}
//-----------------------------------------------------------------------------
void InitLand(int landed[][15]){

	for (int i = 0; i < 53; i++){
		for (int j = 0; j < 15; j++){
			landed[i][j] = 0;
		}
	}

	return;
}
//-----------------------------------------------------------------------------
void GenLand(int landed[][15]){

	int pos = ((rand() + time(0)) % 4);
	int flux = ((rand() + time(0)) % 100);

	for (int i = 0; i < 27; i++){//generate bottom land
		for (int j = (10 + pos); j < 15; j++){
			if (j == (10 + pos))
				landed[i][j] = landed[52 - i][j] = 2;
			else
				landed[i][j] = landed[52 - i][j] = 1;

			if (j == 14 && (i % 2)){
				if (flux <= 43) //go up or down or do nothing
					pos--;
				else if (flux >= 66)
					pos++;

				if (pos < 0)
					pos = 1;

				if (pos > 3)
					pos = 2;

				flux = ((rand() + time(0)) % 100);
			}
		}
	}

	for (int i = 0; i < 27; i++){//generate top land
		for (int j = (3 - pos); j > -1; j--){
			if (j == (3 - pos))
				landed[i][j] = landed[52 - i][j] = 3;
			else
				landed[i][j] = landed[52 - i][j] = 1;

			if (j == 0 && (i % 2)){
				if (flux <= 33)
					pos--;
				else if (flux >= 66)
					pos++;

				if (pos < 0)
					pos = 1;

				if (pos > 3)
					pos = 2;

				flux = ((rand() + time(0)) % 100);
			}
		}
	}

	return;
}
//-----------------------------------------------------------------------------
void PMain(SDL_Joystick *j, SURFACES &ss)
{
	int start_time;
    int fps = 0;
    bool play = false;
	short ctrl_set = 0;//button check bits
    int i = 0;
    double now = 0.0;
    double accumulator = 0.0;
    double new_time, delta;
	SDL_Event events;
	SURFACES surfaces;
	OBJECTS obj;
	ObjManFx om;
	AUDIO audio;
	FONT fonts[MAX_FONTS];
	
	memset(&surfaces, 0, sizeof(SURFACES));
	surfaces.bg = ss.bg;
	InitVars(obj, surfaces, audio, fonts);

	start_time = SDL_GetTicks();

	do
    {
		UpdateTimes(new_time, delta, now, accumulator);

        while(accumulator >= delta) //loop to keep play state in check
        {
            while(SDL_PollEvent(&events))
            {
                play = CheckKeyboard(events, ctrl_set);
                CheckGamePad(events, ctrl_set);
            }

			//if ((obj.n.CanSpawn()) && !(obj.p1.GetPause()))//need trigger for super
               // obj.n.NpcSpawn();
			
			PlayerActions(obj, om, ctrl_set, audio);
			obj.cam.SetFollow(obj.p1.GetX());
			
			if (!(obj.p1.GetPause()))//need trigger for super
				NpcActions(obj, om, audio);

			ProcessProj(om, obj.p);
			ProcessVfx(om, obj.v);

			if (!(obj.p1.GetPause()) || obj.p1.GetPauseType() == 1)
			{
				ProjectileActions(obj, om, audio);
				VfxActions(obj, om);
			}

			obj.h.GetPlayerData(obj.p1);
			obj.h.GetEnemyData(obj.n);

			if (!(obj.p1.GetPause()))
				obj.h.HudState();

			accumulator -= (DTIME * scr_off.spd);
        }

        Render(surfaces, obj, fonts);
        fps = FrameRate(start_time, fps);
		//debug = static_cast<int>(obj.p1.GetX());
    }while(play);
	
	return;
}


