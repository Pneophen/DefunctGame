#include "common.h"
#include "objects.h"

extern ScreenOffset scr_off;
extern int debug;

void WriteScreen(int x, int y, char align, int index, FONT *f, SURFACES &s, char str[]);
void NumString(int x, int y, char align, int index, FONT *f, SURFACES &s, int num);
void AngleMoveF(double angle, double speed, double &x, double &y);
Uint32 SetRGB(int r, int g, int b);
//-----------------------------------------------------------------------------
void SetSpr(int x, int y, SDL_Renderer *view, SDL_Texture *spr, SDL_Rect *rec)
{
     SDL_Rect pos;
     pos.x = ((x + scr_off.shake_x));
     pos.y = ((y + scr_off.shake_y));
	 pos.w = (rec->w);
	 pos.h = (rec->h);
	
	 SDL_RenderCopy(view, spr, rec, &pos);

     return;
}
//-----------------------------------------------------------------------------
void SetSpr(int x, int y, SDL_Renderer *view, SDL_Texture *spr, SDL_Rect *rec, double angle, SDL_Point *center, 
			SDL_RendererFlip flip)
{
     SDL_Rect pos;
     pos.x = ((x + scr_off.shake_x));
     pos.y = ((y + scr_off.shake_y));
	 pos.w = (rec->w);
	 pos.h = (rec->h);
	
	 SDL_RenderCopyEx(view, spr, rec, &pos, angle, center, flip);

     return;
}
//-----------------------------------------------------------------------------
void DrawRec(SDL_Renderer *s, int x, int y, unsigned int w, unsigned int h, Uint32 color, Uint8 alpha)
{
    SDL_Rect sides = {(x), (y),
    (w), (h)};

	Uint8 r = ((color >> 16) & 0xFF);
	Uint8 g = ((color >> 8) & 0xFF);
	Uint8 b = (color & 0xFF) ;

    SDL_SetRenderDrawColor(s, r, g, b, alpha);
	SDL_RenderFillRect(s, &(sides));

    return;
}
//-----------------------------------------------------------------------------
void DrawLine(SDL_Renderer *s, int x, int y, int sz, double a, int l, Uint32 c, Uint8 alpha)
{
    double ax, ay;

    AngleMoveF(a, 1.0, ax, ay);

    for(int i = 0; i < l ; i++)
    {
        DrawRec(s, 
			(x + static_cast<int>(((ax * static_cast<double>(i))))),
			(y + static_cast<int>(((ay * static_cast<double>(i))))), 
			sz, sz, c, alpha);
    }

    return;
}
//-----------------------------------------------------------------------------
void DrawClsn(SDL_Renderer *s, P_RECT &c)
{
	//return;
	Uint32 color = 1000+ RandNum(64000);/*SDL_MapRGB(s->format, 
					(100+ RandNum(150)),
					(100+ RandNum(50)), 
					(100+ RandNum(150))
					);*/
	
	//top
	DrawLine(s, c.x, c.y, 1, 0, c.w, color, 255);
	//bottom
	DrawLine(s, c.x, (c.y + c.h), 1, 0, c.w, color, 255);
	//left
	DrawLine(s, c.x, c.y, 1, 270, c.h, color, 255);
	//right
	DrawLine(s, (c.x + c.w), c.y, 1, 270, c.h, color, 255);

	return;
}
//-----------------------------------------------------------------------------
void DrawBlurObj(SDL_Texture *spr, SDL_Renderer *bg, SDL_Rect *rec, double x, double y, double angle, double speed)
{
    int i;
    Uint8 alpha = 2;
    int extra = 0;
    double ax, ay;

    if (speed > 12.0f)
        speed = 12.0f;

    if (speed > 6.0f)
        extra = (int)(speed / 1.3f);

    AngleMoveF(angle, (speed / 3.0f), ax, ay);

    for(i = 0; i < (6 + extra) ; ++i)
    {
        SDL_SetTextureAlphaMod(spr, (255 / alpha));

        if (i < 3)
			SetSpr((static_cast<int>(x) + static_cast<int>(ax * static_cast<double>(i))) ,
            (static_cast<int>(y) + static_cast<int>(ay * static_cast<double>(i))), bg, spr, rec);

        SetSpr((static_cast<int>(x) - static_cast<int>(ax * static_cast<double>(i))),
        (static_cast<int>(y) - static_cast<int>(ay * static_cast<double>(i))), bg, spr, rec);

        alpha++;
    }

    SDL_SetTextureAlphaMod(spr, 255);

    return;
}
//-----------------------------------------------------------------------------
void RenderLand(int land[][15], Camera &c, SDL_Renderer *s, Player &pp){

	int xx, x2;
	int offset = static_cast<int>((c.GetX()));
	Uint32 color = 0;

	if (offset < 0){
		;
	}
	
	for (int yy = 0; yy < 15; yy++){
		xx = x2 = (static_cast<int>(c.GetX()) >> 4);
		for (int k = 0; k < 28; k++, xx++, x2++){
			if (xx > 52)
				xx = 0;//wrap

			if (xx < 0)
				xx = 53 + (static_cast<int>((c.GetX())) >> 4);//wrap correctly

			if (land[xx][yy] != 0)
			{
				if (land[xx][yy] == 2)
					DrawLine(s, (x2 << 4) - offset, (yy << 4), 16, 0, 1, SetRGB(255, 0, 0), 255);
				else if (land[xx][yy] == 3)
					DrawLine(s, (x2 << 4) - offset, (yy << 4), 16, 0, 1, SetRGB(0, 0, 255), 255);
				else
					DrawLine(s, (x2 << 4) - offset, (yy << 4), 16, 0, 1, color, 255);
			}
		}
	}

	debug = static_cast<int>(c.GetX()) + 213;

	return;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void RenderPlay(SURFACES &s, OBJECTS &o, FONT f[])
{
	
	//if (!(o.p1.GetPauseType()))
		//o.b.DrawBgs(&s, o.cam);

	RenderLand(o.map, o.cam, s.bg, o.p1);
	o.v.DrawVfxsBack(&s, o.cam);
	o.n.DrawNpcs(&s, o.cam, true);
	o.n.DrawNpcs(&s, o.cam, false);
	o.p1.DrawPlayer(&s, o.cam);
	o.v.DrawVfxs(&s, o.cam);
	o.p.DrawProjectiles(&s, o.cam);

	o.h.DrawHud(s, f);

	
	NumString(20, 20, -1, 0, f, s, debug);

	return;
}
//-----------------------------------------------------------------------------
void Render(SURFACES &s, OBJECTS &o, FONT f[])
{
	SDL_SetRenderDrawColor(s.bg, 0, 255, 0, 255);
	SDL_RenderClear(s.bg);//SDL_FillRect(s.bg, 0, s.bg_colors[2]);
	RenderPlay(s, o, f);//gameplay scene
	SDL_RenderPresent(s.bg);//SDL_Flip(s.bg);

	return;
}




