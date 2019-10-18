#include "common.h"
#include "objects.h"

extern int debug;

void WriteScreen(int x, int y, char align, int index, FONT *f, SURFACES &s, char str[]);
void NumString(int x, int y, char align, int index, FONT *f, SURFACES &s, int num);
void SetSpr(int x, int y, SDL_Renderer *view, SDL_Texture *spr, SDL_Rect *rec);
void DrawRec(SDL_Renderer *s, int x, int y, unsigned int w, unsigned int h, Uint32 color, Uint8 alpha);
void DrawLine(SDL_Renderer *s, int x, int y, int sz, double a, int l, Uint32 c, Uint8 alpha);

extern ScreenOffset scr_off;
//-----------------------------------------------------------------------------
Camera::Camera()
{
	x = y = 0.0;
	follow = 0;
	scene.w = SCREEN_WIDTH;
	scene.h = SCREEN_HEIGHT;
	scene.x = 0;
	scene.y = 0;

	return;
}
//-----------------------------------------------------------------------------
double Camera::GetX(void) const
{
	return x;
}
//-----------------------------------------------------------------------------
double Camera::GetY(void) const
{
	return y;
}
//-----------------------------------------------------------------------------
void Camera::SetX(double a)
{
	x = a;

	return;
}
//-----------------------------------------------------------------------------
void Camera::SetY(double a)
{
	y = a;

	return;
}
//-----------------------------------------------------------------------------
void Camera::SetFollow(double xx){

	SetX(xx - 213.0);
	return;
}
//-----------------------------------------------------------------------------
ScreenFx::ScreenFx(void)
{
	/*x = 0;
    y = 0;
    h_str = 0;
	v_str = 0;
    spr = 0;
    factor = 5;
    time = 0;
    vert = false;
    on = false;
    type = 0;

    spr = SDL_CreateRGBSurface(SDL_SWSURFACE, w,
        w, s->format->BitsPerPixel, s->format->Rmask,
        s->format->Gmask, s->format->Bmask, 0);*/

	return;
}
//-----------------------------------------------------------------------------
ScreenFx::~ScreenFx()
{
	if (h_str)
	{
		delete[] h_str;
		h_str = 0;
	}

	if(v_str)
	{
		delete[] v_str;
		h_str = 0;
	}
	
	if (spr)
	{
		delete[] spr;
		h_str = 0;
	}

	return;
}
//-----------------------------------------------------------------------------
ScreenOffset::ScreenOffset()
{
	off_x = 0;//offset
	off_y = 0;
	shake_x = 0;//shaking screen offset independent form screen offset
	shake_y = 0;
	scale = 1; //bitwise multiplication used
	spd = 1.0; //for fast slow motion

	return;
}
//-----------------------------------------------------------------------------
Hud::Hud()
{
	score = 0;
	life = 0;
	dash = 0;
	pow = 0;
	chain = 0;
	c_time = 0;
	super = false;

	enemy = 0;
	wave = 0;

	alpha = 255;

	return;
}
//-----------------------------------------------------------------------------
void Hud::GetPlayerData(Player &p)
{
	score = p.GetScore();
	life = p.GetLife();
	dash = p.GetDash();
	pow = p.GetPow();
	
	if (p.GetChain() > chain)
		counter_space = 6;
	
	chain = p.GetChain();
	c_time = p.GetCTime();

	super = ((p.GetStateNo() == 6000 && p.GetTime() <= 66) ? true : false);

	if (super)
	{
		b_time = (130 + static_cast<int>(p.GetBTime()));
		b_shot = (130 + static_cast<int>(p.GetBShot()));
	}

	AlphaCheck(p.GetY());
	
	return;
}
//-----------------------------------------------------------------------------
void Hud::GetEnemyData(NpcHead &n)
{
	enemy = n.GetEnemy();
	wave = n.GetWave();

	return;
}
//-----------------------------------------------------------------------------
void Hud::AlphaCheck(double pos)
{
	if ((pos >= 220) || (pos <= 20))
		alpha = 100;
	else
		alpha = 255;

	return;
}
//-----------------------------------------------------------------------------
void Hud::DrawText(SURFACES &s, FONT *f)
{
	SDL_SetTextureAlphaMod(s.font_spr[0].spr, alpha);

	WriteScreen(2, 2, -1, 0, f, s, "score- ");
    NumString(38, 2, -1, 0, f, s, score);

	//WriteScreen(4, 228, -1, 0, f, s, "l- ");
    //WriteScreen(64, 228, -1, 0, f, s, "d ");

	WriteScreen((SCREEN_WIDTH - 50), 228, -1, 0, f, s, "wave- ");
    NumString((SCREEN_WIDTH - 20), 228, -1, 0, f, s, wave);

    WriteScreen((SCREEN_WIDTH - 37), 2, -1, 0, f, s, "-enemy");
    NumString((SCREEN_WIDTH - 43), 2, 1, 0, f, s, enemy);

	if (!super)
	{
		WriteScreen(144, 228, -1, 0, f, s, "pow-    %");
		NumString(170, 228, 1, 0, f, s,
        static_cast<int>((static_cast<double>(pow) / 1000.0) * 100));
	}

	
	return;
}
//-----------------------------------------------------------------------------
void Hud::DrawHud(SURFACES &s, FONT *f)
{
	DrawText(s, f);
	DrawBars(s);
	DrawCombo(s, f);

	return;
}
//-----------------------------------------------------------------------------
void Hud::DrawBars(SURFACES &s)
{
	SDL_Rect temp = {(142), 0,
    (6), (10)};

	SDL_SetTextureAlphaMod(s.misc_spr[5].spr, alpha);
	
	for(int i = 0; i < life; ++i)
        SetSpr((10 + (i * 7)), 229, s.bg, s.misc_spr[5].spr, &temp);

	temp.x = 0;
    temp.w = (47);
    SetSpr(69, 230, s.bg, s.misc_spr[5].spr, &temp); //dash bars
    temp.x = (49);
    temp.w  = static_cast<unsigned short>((43.0 * (1)) *
        (static_cast<double>(dash + 1) / static_cast<double>(DASH_MAX + 1)));
    SetSpr(71, 230, s.bg, s.misc_spr[5].spr, &temp);

	if (super)
	{
		temp.x = 0;
		temp.w = (74);
		temp.h = (22);

        SetSpr(130, 223, s.bg, s.misc_spr[7].spr, &temp);//bar

        temp.x += (74);
        SetSpr(b_shot, 223, s.bg, s.misc_spr[7].spr, &temp);//sweetspot

        temp.x += (74);
        SetSpr(b_time, 223, s.bg, s.misc_spr[7].spr, &temp);//slider
    }


	return;
}
//-----------------------------------------------------------------------------
void Hud::DrawCombo(SURFACES &s, FONT *f)
{
	if (chain >= 2)
	{
		NumString((SCREEN_WIDTH - 64), 17, 0, 1, &(f[1]), s, chain);

		if (counter_space > 0)
		{
			SDL_SetTextureAlphaMod(s.font_spr[1].spr, 100);
			NumString((SCREEN_WIDTH - 64), (17 + counter_space), 0, 1, &(f[1]), s, chain);
			NumString((SCREEN_WIDTH - 64), (17 - counter_space), 0, 1, &(f[1]), s, chain);
			NumString((SCREEN_WIDTH - 64), (16 + counter_space), 0, 1, &(f[1]), s, chain);
			NumString((SCREEN_WIDTH - 64), (18 - counter_space), 0, 1, &(f[1]), s, chain);
			SDL_SetTextureAlphaMod(s.font_spr[1].spr, 255);
		}

		WriteScreen((SCREEN_WIDTH - 41), 24, -1, 0, f, s, "chain");
		
		DrawRec(s.bg, (SCREEN_WIDTH - 45), 36, 40, 2, 0, 255);
		DrawRec(s.bg, (SCREEN_WIDTH - 45), 36,(static_cast<int>(40.0 * (static_cast<double>(c_time) / 200.0))), 
			2, 0xffffff, 255);
	}
	
	return;
}
//-----------------------------------------------------------------------------
void Hud::HudState(void)
{
	if (counter_space > 0)
		--counter_space;

	return;
}
//-----------------------------------------------------------------------------
BgObj::BgObj()
{
	memset(&anim, 0, sizeof(FIXED_ANIM));
	time = 0;
	x = y = 0.0; //pos based on room
	cx = cy = 0.0; //pos based on camera
	w = h = 0; //spacing width and height for tiling
	hs = vs = 0;//horizontal speed, vertical speed
	t_type = s_type = 0;;//tile type, scroll type
	at = true;//automatic scrolling?
	scene = 1;//init is 0
	layer = 0;
	line = NULL;

	return;
}
//-----------------------------------------------------------------------------
void BgObj::SetLineList(int num)
{
	line = (new (std::nothrow) P_LINE[num]);

	return;
}
//-----------------------------------------------------------------------------
void BgObj::SetLine(int index, int xx, int yy, double a, int l, int sz, Uint32 c)
{
	line[index].x = xx;
	line[index].y = yy;
	line[index].angle = a;
	line[index].length = l;
	line[index].size = sz;
	line[index].color = c;

	return;
}
//-----------------------------------------------------------------------------
void BgObj::DrawBgA(SURFACES *s, int cx, int cy)
{
	int color = ((RandNum(100) + 155) << 16);//uniform red
	
	if(layer == 2)
	{	
		DrawRec(s->bg, (0 - cx), (20 - cx), SCREEN_WIDTH, line->size, color, 255);
		DrawRec(s->bg, (0 - cx), (60 - cx), SCREEN_WIDTH, line->size, color, 255);
	}
	else if(layer == 3)
	{	
		DrawRec(s->bg, (0 - cx), (170 - cx), SCREEN_WIDTH, line->size, color, 255);
		DrawRec(s->bg, (0 - cx), (215 - cx), SCREEN_WIDTH, line->size, color, 255);
	}


	if (layer != 1)
	{
		int xx = (line->x - cx);
		int yy = ((layer == 3) ? 170 : (line->y - cy));
		double aa = line->angle;
		
		for(int i = 0; i < 5; ++i)
		{
			DrawLine(s->bg, xx, yy, line->size, ((layer == 2) ? -aa : aa), line->length, color, 255);
			xx -= 107;
			aa -= 18;

			if (xx < -1)
			{
				xx += (SCREEN_WIDTH + 115);
				aa += 90;
			}
		}

		line->x -= 2;
		line->angle -= 0.35;

		if (line->x < -1)
		{
			line->x = (SCREEN_WIDTH + 1);
			line->angle = 315;
		}
	}

	if (layer == 1)
	{
		//int yy = (line[0].y - cy);
		//int sz = line[0].size;

		for(int i = 0; i < 5; ++i)
		{
			if (line[i].y < 119 && line[i].y > 63)
			{
				DrawRec(s->bg, 0, line[i].y, SCREEN_WIDTH, line[i].size, (RandNum(100) + 100), 255);

				if (!(line[i].y % 11))
					line[i].size -= 1;
			}
			
			line[i].y += 1;

			if (line[i].y > 119)
			{
				line[i].y = 20;
				line[i].size = 5;
			}
		}
		
		for(int i = 5; i < 10; ++i)
		{
			if (line[i].y > 120 && line[i].y < 160)
			{
				DrawRec(s->bg, 0, line[i].y, SCREEN_WIDTH, line[i].size, (RandNum(100) + 100), 255);

				if (!(line[i].y % 11))
					line[i].size -= 1;
			}
			
			line[i].y -= 1;

			if (line[i].y < 120)
			{
				line[i].y = 220;
				line[i].size = 5;
			}
		}
	}

	return;
}
//-----------------------------------------------------------------------------
void BgObj::DrawBg(SURFACES *s, int cx, int cy)
{
	
	return;
}
//-----------------------------------------------------------------------------
void BgObj::SetLayer(int num)
{
	layer = num;
	return;
}
//-----------------------------------------------------------------------------
int BgObj::GetSceneNum(void)
{
	return scene;
}
//-----------------------------------------------------------------------------
BgHead::BgHead()
{

	return;
}
//-----------------------------------------------------------------------------
void BgHead::BgGenA(int set)
{
	BgObj b;
	
	if (set == 1 || set == 2)
	{
		b.SetLineList(1);
		b.SetLine(0, (SCREEN_WIDTH - 3), 60, 315,  90, 2, 0xff0000);
	}
	else
	{
		b.SetLineList(10);
		
		for(int i = 0; i < 5; ++i)
			b.SetLine(i, 0, (0 + (i * 20)), 0,  0, 5, 0);

		for(int i = 5; i < 10; ++i)
			b.SetLine(i, 0, (240 - ((i - 5) * 20)), 0,  0, 5, 0);
	}

	b.SetLayer(b_list.size() + 1);
    b_list.push_front(b);
	
	return;
}
//-----------------------------------------------------------------------------
void BgHead::DrawBgs(SURFACES* s, Camera& cam)
{
	if (!b_list.size())
		return;
	else
	{
		std::list<BgObj>::iterator i;
		for(i = b_list.begin(); i != b_list.end(); ++i)
		{
			if (i->GetSceneNum() == 1)
				i->DrawBgA(s, static_cast<int>(cam.GetX()), static_cast<int>(cam.GetY()));
		}
	}

	return;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


