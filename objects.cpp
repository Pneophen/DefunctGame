#include "common.h"
#include "objects.h"

extern ScreenOffset scr_off;
extern int debug;
unsigned int BaseObj::o_id = 0;

void SetFixedAnim(FIXED_ANIM &anim, unsigned short w, unsigned short h, unsigned short frames);
void SetSpr(int x, int y, SDL_Renderer *view, SDL_Texture *spr, SDL_Rect *rec);
void DrawBlurObj(SDL_Texture *spr, SDL_Renderer *bg, SDL_Rect *rec, double x, double y, double angle, double speed);
void PlaySound(AUDIO &a, unsigned int type, unsigned int index, int loops);
bool ClsnCheck(const P_RECT &a, const P_RECT &b);
void DrawClsn(SDL_Renderer *s, P_RECT &c);

//-----------------------------------------------------------------------------
BaseObj::BaseObj()
{
	x = y = 0.0;//axis
	vx = vy = 0.0;//velocity
	sx = sy = 0;//sprite offset
	px = py = 0.0; //previous x and y positions
	tx = ty = 0; //x, y pos based on tile coordinates
	time = 0.0;
	state_no = 0;
	ctrl = false;
	life = 0;
	muteki = 0; //invincible
	angle = 0.0;
	speed = 0.0;
	physics = 'n';//a = air, g = ground, n = none
	state_type = 's';//s = stand, c = crouch, a = air
	move_type = 'n';//a = attacking, h = getting hit, n = neutral
	grav = fric = 0.0;
	facing = prev_facing = 1;
	pause = false;//whether object is in a pause state
	pause_type = 0;
	visible = true;
	u_id = o_id++;//assign id then increment
	anim.anim_clip = 0;
	color = 0xffffff; //white
	alpha = 255; //visible
	additive = false;
	x_scale = 1.0;
	y_scale = 1.0;
	
	memset(&anim, 0, sizeof(FIXED_ANIM));
	memset(&clsn, 0, sizeof(P_RECT));

	return;
}
//-----------------------------------------------------------------------------
BaseObj::~BaseObj()
{
	/*if (anim.anim_clip != 0)
	{
		delete[] (anim.anim_clip);
		anim.anim_clip = NULL;
	}*/

	return;
}
//-----------------------------------------------------------------------------
void BaseObj::ClearAnim(void)
{
	if (anim.anim_clip != 0)
	{
		delete[] (anim.anim_clip);
		anim.anim_clip = 0;
	}
	
	return;
}
//-----------------------------------------------------------------------------
void BaseObj::SetPos(double xx, double yy)
{
	if (xx > -9998.0)
		x = xx;

	if (yy > -9998.0)
		y = yy;
	
	return;
}
//-----------------------------------------------------------------------------
void BaseObj::SetVel(double xx, double yy)
{
	if (xx >= -998.0)//do nothing if less than or equal to this
		vx = xx;

	if (yy >= -998.0)
		vy = yy;
	
	return;
}
//-----------------------------------------------------------------------------
void BaseObj::SetVel(int ang, double spd)
{
	AngleMoveF(static_cast<double>(ang), spd, vx, vy);
	
	return;
}
//-----------------------------------------------------------------------------
void BaseObj::SetSprOff(int xx, int yy)
{
	if (xx > -9999)//
		sx = xx;

	if (yy > -9999)
		sy = yy;
	
	return;
}
//-----------------------------------------------------------------------------
void BaseObj::SetPrevPos(void)
{
		px = x;
		py = y;
	
	return;
}
//-----------------------------------------------------------------------------
void BaseObj::SetTilePos(int xx, int yy)
{
	if (xx > -9999)
		tx = xx;

	if (yy > -9999)
		ty = yy;

	return;
}
//-----------------------------------------------------------------------------
void BaseObj::SetTime(double t)
{
	time = t;
	return;
}
//-----------------------------------------------------------------------------
int BaseObj::GetIntTime(void) const
{
	return static_cast<int>(time);
}
//-----------------------------------------------------------------------------
void BaseObj::SetStateNo(short sn)
{
	state_no = sn;
	time = -1.0;//always reset time for new states
	return;
}
//-----------------------------------------------------------------------------
void BaseObj::SetStateNo(short sn, bool ct)
{
	state_no = sn;
	ctrl = ct;
	time = -1.0;//always reset time for new states
	return;
}
//-----------------------------------------------------------------------------
void BaseObj::SetCtrl(bool c)
{
	ctrl = c;
	return;
}
//-----------------------------------------------------------------------------
void BaseObj::SetLife(short l)
{
	life = l;
	return;
}
//-----------------------------------------------------------------------------
void BaseObj::SetInv(short i)
{
	muteki = i;
	return;
}
//-----------------------------------------------------------------------------
void BaseObj::SetAngle(double a)
{
	angle = a;
	return;
}
//-----------------------------------------------------------------------------
void BaseObj::SetSpeed(double s)
{
	speed = s;
	return;
}
//-----------------------------------------------------------------------------
void BaseObj::SetPhysics(double g, double f)// grav and fric
{
	if (g >= -998.0)
		grav = g;

	if (f >= -998.0)
		fric = f;

	return;
}
//-----------------------------------------------------------------------------
void BaseObj::MoveObj(void)
{
	px = x;
    py = y;
	x += vx;
	y += vy;

	return;
}
//-----------------------------------------------------------------------------
void BaseObj::SetFacing(char f)
{
	prev_facing = facing;
	facing = f;

	return;
}
//-----------------------------------------------------------------------------
void BaseObj::SetPause(bool p)
{
	pause = p;
	return;
}
//-----------------------------------------------------------------------------
void BaseObj::SetVisible(bool v)
{
	visible = v;
	return;
}
//-----------------------------------------------------------------------------
void BaseObj::SetClsn(short xx, short yy, unsigned short ww, unsigned short hh)
{	
	clsn.x = xx;
	clsn.y = yy;
	clsn.w = ww;
	clsn.h = hh;

	return;
}
//-----------------------------------------------------------------------------
void BaseObj::SetAnim(unsigned short w, unsigned short h, unsigned short f)
{
	SetFixedAnim(anim, w, h, f);

	return;
}
//-----------------------------------------------------------------------------
void BaseObj::SetAlive(bool a)
{
	alive = a;
	return;
}
//-----------------------------------------------------------------------------
void BaseObj::SetColor(Uint8 r, Uint8 g, Uint8 b)
{
	color = ((r << 16) + (g << 8) + b);
	
	return;
}
//-----------------------------------------------------------------------------
void BaseObj::SetColor(Uint32 c)
{
	color = c;
	return;
}
//-----------------------------------------------------------------------------
void BaseObj::SetAlpha(Uint8 a)
{
	alpha = a;
	return;
}
//-----------------------------------------------------------------------------
void BaseObj::SetAdditive(bool add)
{
	additive = add;
	return;
}
//-----------------------------------------------------------------------------
void BaseObj::SetScale(double xx, double yy)
{
	x_scale = xx;
	y_scale = yy;

	return;
}
//-----------------------------------------------------------------------------
double BaseObj::GetX(void) const
{
	return x;
}
//-----------------------------------------------------------------------------
double BaseObj::GetY(void) const
{
	return y;
}
//-----------------------------------------------------------------------------
double BaseObj::GetVX(void) const
{
	return vx;
}
//-----------------------------------------------------------------------------
double BaseObj::GetVY(void) const
{
	return vy;
}
//-----------------------------------------------------------------------------
int BaseObj::GetSX(void) const
{
	return sx;
}
//-----------------------------------------------------------------------------
int BaseObj::GetSY(void) const
{
	return sy;
}
//-----------------------------------------------------------------------------
double BaseObj::GetPX(void) const
{
	return px;
}
//-----------------------------------------------------------------------------
double BaseObj::GetPY(void) const
{
	return py;
}
//-----------------------------------------------------------------------------
int BaseObj::GetTX(void) const
{
	return tx;
}
//-----------------------------------------------------------------------------
int BaseObj::GetTY(void) const
{
	return ty;
}
//-----------------------------------------------------------------------------
double BaseObj::GetTime(void) const
{
	return time;
}
//-----------------------------------------------------------------------------
short BaseObj::GetStateNo(void) const
{
	return state_no;
}
//-----------------------------------------------------------------------------
bool BaseObj::GetCtrl(void) const
{
	return ctrl;
}
//-----------------------------------------------------------------------------
short BaseObj::GetLife(void) const
{
	return life;
}
//-----------------------------------------------------------------------------
short BaseObj::GetInv(void) const
{
	return muteki;
}
//-----------------------------------------------------------------------------
double BaseObj::GetAngle(void) const
{
	return angle;
}
//-----------------------------------------------------------------------------
double BaseObj::GetSpeed(void) const
{
	return speed;
}
//-----------------------------------------------------------------------------
double BaseObj::GetGrav(void) const
{
	return grav;
}
//-----------------------------------------------------------------------------
double BaseObj::GetFric(void) const
{
	return fric;
}
//-----------------------------------------------------------------------------
char BaseObj::GetFacing(void) const
{
	return facing;
}
//-----------------------------------------------------------------------------
bool BaseObj::GetPause(void) const
{
	return pause;
}
//-----------------------------------------------------------------------------
char BaseObj::GetPauseType(void) const
{
	return pause_type;
}
//-----------------------------------------------------------------------------
bool BaseObj::GetVisible(void) const
{
	return visible;
}
//-----------------------------------------------------------------------------
FIXED_ANIM BaseObj::GetAnim(void) const
{
	return anim;
}
//-----------------------------------------------------------------------------
P_RECT BaseObj::GetClsn(void) const
{
	return clsn;
}
//-----------------------------------------------------------------------------
int BaseObj::GetAnimData(char type) const
{
	switch(type)
	{
		case 0: return static_cast<int>(anim.offset); break;
		case 1: return static_cast<int>(anim.anim_time); break;
		case 2: return static_cast<int>(anim.delay); break;
		case 3: return static_cast<int>(anim.frame); break;
		case 4: return static_cast<int>(anim.total_frames); break;
		default: ; break;
	}

	return 0;
}
//-----------------------------------------------------------------------------
SDL_Rect BaseObj::GetAnimData(void) const
{
	return (anim.anim_clip[anim.frame]);//?
}
//-----------------------------------------------------------------------------
unsigned int BaseObj::GetUId(void) const
{
	return u_id;
}
//-----------------------------------------------------------------------------
void BaseObj::AdjClsn(unsigned short ww, unsigned short hh)
{	
	clsn.w = ww;
	clsn.h = hh;
	clsn.x = (static_cast<int>(x) - static_cast<int>(clsn.w >> 1));
    clsn.y = (static_cast<int>(y) - static_cast<int>(clsn.h >> 1));

	return;
}
//-----------------------------------------------------------------------------
Uint32 BaseObj::GetColor(void) const
{
	return color;
}
//-----------------------------------------------------------------------------
Uint8 BaseObj::GetAlpha(void) const
{
	return alpha;
}
//-----------------------------------------------------------------------------
bool BaseObj::GetAdditive(void) const
{
	return additive;
}
//-----------------------------------------------------------------------------
bool BaseObj::IsAlive(void) const
{
	return alive;
}
//-----------------------------------------------------------------------------
double BaseObj::GetXScale(void) const
{
	return x_scale;
}
//-----------------------------------------------------------------------------
double BaseObj::GetYScale(void) const
{
	return y_scale;
}
//-----------------------------------------------------------------------------
void BaseObj::CalcAngle(void)
{
	angle = MathGetAngleMove(vx, -(vy));
	return;
}
//-----------------------------------------------------------------------------
void BaseObj::CalcSpeed(void)
{
    speed = MathGetVSpeed(vx, vy);
	return;
}
//-----------------------------------------------------------------------------
void BaseObj::AnimInc(void)
{
     anim.anim_time += 1;

     if (anim.anim_time > (anim.delay - 1))
     {
         anim.anim_time = 0;
         anim.frame += 1;

         if (anim.frame > (anim.total_frames - 1))
            anim.frame = 0;
     }

     return;
}
//-----------------------------------------------------------------------------
void BaseObj::TimeInc(void)
{
    time++;
	return;
}
//-----------------------------------------------------------------------------
void BaseObj::InvInc(void)
{
    muteki--;
	return;
}
//=============================================================================
//Player class
//=============================================================================
Player::Player()
{
    clsn.w = 4;
    clsn.h = 2;
    clsn.x = (static_cast<short>(x) - (clsn.w >> 1));
    clsn.y = (static_cast<short>(y) - (clsn.h >> 1));

	return;
}
//-----------------------------------------------------------------------------
Player::~Player()
{
	return;
}
//-----------------------------------------------------------------------------
void Player::Init(const SPRITE &s)
{
    SetAnim(s.w, s.h, s.frames);
    x = 420.0;
    y = 115.0;
    sx = 17;
    sy = 11;
	vx = vy = 0.0;
	px = py = 0.0; //previous x and y positions
	tx = ty = 0; //x, y pos based on tile coordinates
    ctrl = false;
	physics = 'n';//a = air, g = ground, n = none
	state_type = 's';//s = stand, c = crouch, a = air
    move_type = 'n';
    visible = true;
    life = 6;
    muteki = 60;
    score = 0;
    dash = DASH_MAX;
    gun_lvl = 1;
    pow = 0;
    super = false;
    pause = false;
	pause_type = 0;
    b_time = 0.0f;
    b_shot = (double)(RandNum(36) + 29);
    got_bshot = 0;
    d_type = 0;
    blur = false;
    max_chain = false;
    state_no = -1;
    absorbed = 0;
    allow_spawn = false;
	time = 0;
	angle = 0.0;
	speed = 0.0;
	grav = fric = 0.0;
	facing = prev_facing = 1;
	pause_type = 0;
	freq = 0;
	chain = 0;
	gun_xp = 0;
	gun_next = 50;

    return;
}
//-----------------------------------------------------------------------------
void Player::DrawPlayer(SURFACES *s, Camera &c)
{
	
	//SDL_SetTextureColorMod(s->obj_spr[0].spr, 255, 0, 255);
	if (visible)
    {
        if (additive)
			SDL_SetTextureBlendMode(s->obj_spr[0].spr, SDL_BLENDMODE_ADD);
		
		SetSpr(((static_cast<int>(x) - sx) - static_cast<int>(c.GetX())), 
			((static_cast<int>(y) - sy) - static_cast<int>(c.GetY())),
        s->bg, s->obj_spr[0].spr, &(anim.anim_clip[anim.frame]));

		if (additive)
			SDL_SetTextureBlendMode(s->obj_spr[0].spr, SDL_BLENDMODE_BLEND);

        if (blur)
            DrawBlurObj(s->obj_spr[0].spr, s->bg, &(anim.anim_clip[anim.frame])
            , ((static_cast<int>(x) - sx) - static_cast<int>(c.GetX())), 
			((static_cast<int>(y) - sy) - static_cast<int>(c.GetY())),
            static_cast<int>(angle), speed);
    }
	
	if (CLSN)
		DrawClsn((s->bg), GetClsn());

	return;
}
//-----------------------------------------------------------------------------
void Player::SetState(short st, bool c)
{
    if (state_no == st)
        return;

    SetStateNo(st, c);
	time = -1;

    return;
}
//-----------------------------------------------------------------------------
void Player::ScoreAdd(unsigned int pts)
{
    if (score >= 1000000000)
        score += (pts >> 2);
    else if (score >= 500000000)
        score += (pts >> 1);
    else
        score += pts;

    if ((score > 2000000000))
        score = 2000000000;

    return;
}
//-----------------------------------------------------------------------------
void Player::CmdCheck(ObjManFx &om, short &pad)
{
    if (life > 0 && (pad & C_START))
    {
        pad &= ~C_START;

		if (!pause_type)
			pause =  ((pause) ? false : true);

		if (state_no == 6000)
			pause_type = ((pause_type == 1) ? 0 : 1);
    }

    if (pause)
        return;

    if (life <= 0 && state_no != 5900)
        SetState(5900, false);

    /*//dashing
    if (state_no != 100 && dash >= DASH_MAX && ctrl && ((pad & C_DOWN) || (pad & C_UP)
    || (pad & C_LEFT) || (pad & C_RIGHT)) && (pad & C_BOOST))
        SetState(100, false);*/

    //absorb
    if (state_no != 101 && dash >= DASH_MAX && ctrl && (pad & C_BOOST))
    {
        move_type = 'n';
        SetState(101, false);
    }

    //movement state
    if (state_no != 20  && state_no != 100 && ctrl && ((pad & C_DOWN) || (pad & C_UP)
    || (pad & C_LEFT) || (pad & C_RIGHT)))
        SetState(20, true);

	//super shot
    if ((ctrl || state_no == 101) && state_no != 6000 && (pad & C_BOMB) && pow >= 0)//1000
    {
        pause_type = 1;
		pause = true;
		SetState(6000, false);
    }
    return;
}
//-----------------------------------------------------------------------------
void Player::PlayerStates(ObjManFx &om, short &pad, AUDIO &a)
{
    int i;
	
    if (muteki == 60 && move_type == 'h')
    {
        PlaySound(a, SND_COMMON, 12, 0);

        if (life == 1)
            PlaySound(a, SND_COMMON, 14, 0);
    }

	//shoot
    if ((ctrl || state_no == 101) && !(freq) && (pad & C_SHOOT))
    {
		if (gun_lvl == 1)
		{
			om.AddProj(1, -1, ((x + vx)+24), ((y + vy)), 0, 6);
            freq = 7;
		}
		else if (gun_lvl == 2)
		{
			om.AddProj(1, -1, ((x + vx)+24), ((y + vy)), 20, 4);
            om.AddProj(1, -1, ((x + vx)+24), ((y + vy)), 340, 4);
            freq = 7;
		}
		else if (gun_lvl == 3)
		{
			om.AddProj(1, -1, ((x + vx)+24), ((y + vy)), 60, 6);
            om.AddProj(1, -1, ((x + vx)+24), ((y + vy)), 300, 6);
			om.AddProj(1, -1, ((x + vx)+24), ((y + vy)), 0, 6);
            freq = 5;
		}
		else if (gun_lvl == 4)
		{
			om.AddProj(1, -1, ((x + vx)+24), ((y + vy)), 60, 8);
            om.AddProj(1, -1, ((x + vx)+24), ((y + vy)), 340, 4);
			om.AddProj(1, -1, ((x + vx)+24), ((y + vy)), 20, 4);
            om.AddProj(1, -1, ((x + vx)+24), ((y + vy)), 300, 8);
            freq = 5;
		}
		else if (gun_lvl == 5)
		{
			om.AddProj(2, -1, ((x + vx)+24), ((y + vy)), 335 + RandNum(50), 8);
            om.AddProj(2, -1, ((x + vx)+24), ((y + vy) - 5), 0, 8);
			om.AddProj(2, -1, ((x + vx)+24), ((y + vy) + 5), 0, 8);
			om.AddProj(2, -1, ((x + vx)+24), ((y + vy)), 335 + RandNum(50), 8);
            freq = 3;
		}

		om.AddVfx(1, -1, x+20, ((y + vy)), 0);
		PlaySound(a, SND_COMMON, 0, 0);
    }
	
    switch(state_no)
    {
        case -1:
            om.AddVfx(14, -1, 0, 0, 0);

            om.AddVfx(12, -1, (x), (y), 0);

            if (time == 150)
            {
                allow_spawn = true;
                SetState(0, true);
            }
        break;
        case 0://neutral state
            if (time <= 0)
            {
                vx = vy = 0.0;
                SetPhysics(0.0, 0.0);
            }

            if (time == 3)
                anim.frame = 0;

            if (!(muteki)) //can't be hit
                visible = true;
            if ((GetIntTime() % 2) && move_type == 'h')//got hit flicker
                visible = ((visible == true) ? false : true);
        break;
        case 20://moving state

		case 101://absorb
            if (pad & C_DOWN)
                vy = 2.0;
            if (pad & C_UP)
                vy = -2.0;
            if (pad & C_LEFT)
                vx = -2.0;
            if (pad & C_RIGHT)
                vx = 2.0;

			if ((freq) && (pad & C_SHOOT))//shooting slows you down
			{
				vx /= 2;
				vy /= 2;
			}

            if (vy < 0.0 && state_no != 101)
                anim.frame = 2;
            if (vy > 0.0 && state_no != 101)
                anim.frame = 1;

            if (!(pad & C_DOWN) && !(pad & C_UP))
            {
                vy = 0.0;

				if (state_no != 101)
                anim.frame = 0;
            }
            if (!(pad & C_LEFT) && !(pad & C_RIGHT))
                vx = 0.0;

            if (!(pad & C_DOWN) && !(pad & C_UP) && !(pad & C_LEFT) && !(pad & C_RIGHT) && state_no != 101)
                SetState(0, true);

            if (!(muteki))
                visible = true;
            if ((GetIntTime() % 2) && move_type == 'h')//got hit flicker
                visible = ((visible == true) ? false : true);

			if (state_no == 101)
			{
				if (!(GetIntTime() % 2))
                anim.frame = ((anim.frame == 3) ? 4 : 3);

				muteki = 0;
				dash = 0;
				visible = true;
				pad &= ~C_BOOST;

				if (time == 0)
				{
					d_type = 1;
					move_type = 'n';
					PlaySound(a, SND_COMMON, 8, 0);
					AdjClsn(15, 10);
				}

				/*if (pow >= 1000 && time < 140)
				{
					time = 140;
					pow = 1000;
					PlaySound(a, SND_COMMON, 23, 0);
				}*/

				if (time == 180)
					PlaySound(a, SND_COMMON, 7, 0);

				if (time >= 200 || move_type == 'h')
				{
					AdjClsn(4, 2);
					muteki = 60;
					anim.frame = 0;
					SetState(20, true);
				}
			}
        break;
        case 100://dash
            if (time == 0.0)
            {
                anim.frame = 0;
                d_type = 0;
                PlaySound(a, SND_COMMON, 28, 0);

                if (pad & C_DOWN)
                    vy = 12.0;
                if (pad & C_UP)
                    vy = -12.0;
                if (pad & C_LEFT)
					vx = -12.0;
                if (pad & C_RIGHT)
                    vx = 12.0;

				if (vx && vy)//so diag movement is similar to straight movement
				{
					vx /= 1.5;
					vy /= 1.5;
				}

                muteki = 25;
                dash = 0;
                move_type = 'n';
                visible = true;
                pad &= ~C_BOOST;//so you can't hold boost
				AdjClsn(15, 10);
				
            }
            
            if (time < 14)
                om.AddVfx(0, -1, px, py, 0);

            if (time == 6)//(time >= 5 && !(vx) && !(vy))
            {
                AdjClsn(4, 2);
                muteki = 20;
                SetState(0, true);
            }
        break;
        /*case 101://absorb

            if (!(GetIntTime() % 2))
                anim.frame = ((anim.frame == 3) ? 4 : 3);

            muteki = 0;
            dash = 0;
            visible = true;
            pad &= ~C_BOOST;

            if (pad & C_DOWN)
                vy = 1.0;
            if (pad & C_UP)
                vy = -1.0;
            if (pad & C_LEFT)
                vx = -1.0;
            if (pad & C_RIGHT)
                vx = 1.0;

            if (!(pad & C_DOWN) && !(pad & C_UP))
                vy = 0.0;

            if (!(pad & C_LEFT) && !(pad & C_RIGHT))
                vx = 0.0;

            if (time == 0)
            {
                d_type = 1;
                move_type = 'n';
                PlaySound(a, SND_COMMON, 8, 0);
				AdjClsn(15, 10);
            }

            if (pow >= 1000 && time < 140)
            {
                time = 140;
                pow = 1000;
                PlaySound(a, SND_COMMON, 23, 0);
            }

            if (time == 180)
                PlaySound(a, SND_COMMON, 7, 0);

            if (time >= 200 || move_type == 'h')
            {
                AdjClsn(4, 2);
                muteki = 60;
                anim.frame = 0;
                SetState(0, true);
            }
        break;*/
        case 5900:

            if (time == 0)
            {
                om.AddVfx(10, -1, x, y, 0);
                om.AddProj(100, -1, 0, 0, 0, 0);

                for(i = 0; i < 200; ++i)
                    om.AddVfx(11, -1, x, y, 0);

                PlaySound(a, SND_COMMON, 13, 0);
            }

            if (time > 0)
            {
                x = 20.0;
                y = 115.0;
            }

            muteki = 10;
            visible = false;
        break;
        case 6000:

			pow = 0;

            if (time <= 0)
                pad &= ~C_BOMB;

            if (time == 0)
            {
                AdjClsn(4, 2);
                Mix_HaltChannel(-1);
                PlaySound(a, SND_COMMON, 2, 0);
                anim.frame = 0;
                move_type = 'n';
                visible = true;
                vx = 0.0;
                vy = 0.0;
                muteki = 86;
                om.AddVfx(4, -1, (x + 13), y, 0);
            }

            if (time == 24)
                om.AddVfx(6, -1, (x + 15), (y + 3), 0);

            if (b_time < 70.0f && !(got_bshot))
            {
                b_time += ((b_time < 25.0) ? 3.0 : 1.5);//+ ((float)(lvl) * 0.1f);

                if (b_time > 70.0)
                    b_time = 70.0;
            }

            if ((pad & C_BOMB) && !(got_bshot))
            {
                if ((b_time >= b_shot) && (b_time <= (b_shot + 5.0f)))
                {
                    PlaySound(a, SND_COMMON, 5, 0);
                    PlaySound(a, SND_COMMON, 5, 0);
                    got_bshot = 1;
                }
                else
                    got_bshot = -1;

            }

            if (time == 66)
            {
                if (got_bshot <= 0)
                {
                    Mix_HaltChannel(-1);
                    om.AddProj(3, -1, (x +15),
                    (y + vy), 0, 15);

					for(i = 0; i < 50; ++i)
						om.AddVfx(15, -1, (x + 15), y, 0);
                }
                else
                {
                    Mix_HaltChannel(-1); //so projectiles sound is not canceled
                    om.AddProj(4, -1, ((x + vx)+50),
                    (y + vy), 0, 15);
                }

                om.AddVfx(9, -1, (x + 6), (y), 0);
                muteki = 20;
				vx = -1;
                pause = false;
				pause_type = 0;
                b_shot = static_cast<double>(RandNum(28) + 37);
                b_time = 0;
                got_bshot = 0;
            }

            if (time == 67)
            {
                fric = 1.0;
                grav = 1.0;
            }

            if (time >= 71)
				SetState(0, true);
		
        break;
        default:;
    }

    if (speed > 3.0)
        blur = true;

    if (blur && speed < 1.0)
        blur = false;

    if (max_chain)
    {
        max_chain = false;
        PlaySound(a, SND_COMMON, 20, 0);
    }


    return;
}
//-----------------------------------------------------------------------------
void Player::ChainAdd(void)
{
    chain++;

    c_time = 200;

    if (chain >= 1000)
    {
        chain = 0;
        c_time = 0;
        ScoreAdd(20000);
		pow = 1000;

        //max_chain = true;
    }


    return;
}
//-----------------------------------------------------------------------------
void Player::Move(const Camera &c)
{
    MoveObj();

    if (vx != 0 && fric > 0.0) //friction
    {
        if (vx > fric)
            vx -= fric;
        else if (vx < fric)
            vx += fric;
        else
        {
            fric = 0.0;
            vx = 0.0;
        }
    }

    if (vy != 0 && grav > 0.0) //gravity
    {
        if (vy > grav)
            vy -= grav;
        else if (vy < grav)
            vy += grav;
        else
        {
            grav = 0.0;
            vy = 0.0;
        }
    }

	if (x >= 853)
		x = (vx * 2.0);

	if (x <= -1)
		x = 852.0 + (vx * 2.0);
    //top side of screen
    if (static_cast<int>(y - 6.0) <= c.GetY())
    {
        vy = 0;
        y = static_cast<double>(c.GetY() + 6 + 1);
    }
    //bottom side of screen
    if ((static_cast<int>(y) + 6) >= (c.GetY() + SCREEN_HEIGHT))
    {
        vy = 0;
        y = static_cast<double>(((c.GetY() + (SCREEN_HEIGHT - 6)) - 1));
    }

    clsn.x = (static_cast<short>(x) - (clsn.w >> 1)); //move collision
    clsn.y = (static_cast<short>(y) - (clsn.h >> 1));

    CalcAngle();
    CalcSpeed();

    return;
}
//-----------------------------------------------------------------------------
void Player::HitAdd(void)
{
    hits++;

    h_time = 20;

    if (hits >= 10)
    {
        ChainAdd();
        hits = 0;
    }

    return;
}
//-----------------------------------------------------------------------------
short Player::GetPow(void) const
{
	return pow;
}
//-----------------------------------------------------------------------------
unsigned short Player::GetChain(void) const
{
	return chain;
}
//-----------------------------------------------------------------------------
short Player::GetCTime(void) const
{
	return c_time;
}
//-----------------------------------------------------------------------------
int Player::GetScore(void) const
{
	return score;
}
//-----------------------------------------------------------------------------
short Player::GetDash(void) const
{
	return dash;
}
//-----------------------------------------------------------------------------
double Player::GetBTime(void) const
{
	return b_time;
}
//-----------------------------------------------------------------------------
double Player::GetBShot(void) const
{
	return b_shot;
}
//-----------------------------------------------------------------------------
void Player::Action1(AUDIO &a)
{
	int pow_chk;

    if (!pause || pause_type)
    {
        time++;

        if (freq > 0)
            freq--;

        if (muteki > 0)
        {
            muteki--;

            if (muteki == 0)
                move_type = 'n';
        }

        if ((dash < DASH_MAX) && (life >= 1) && !(super))
        {
                dash++;

            if (dash == DASH_MAX)
                PlaySound(a, SND_COMMON, 22, 0);
        }

        if ((score > 2000000000))
            score = 2000000000;

        if (c_time > 0 && !(super))
        {
            c_time--;

            if ((c_time) <= 0)
            {
                score += (chain * 100);
                chain = 0;
            }
        }

        if (h_time > 0 && !(super))
        {
            h_time--;

            if ((h_time) == 0)
                hits = 0;
        }

        pow_chk = pow;
        

        if ((pow >= 1000) && (pow_chk != pow) && (state_no != 101))
            PlaySound(a, SND_COMMON, 23, 0);
	}
	
	return;
}
//-----------------------------------------------------------------------------
void Player::GotHit(int damage)
{
	if (life > 0)
		life -= damage;

    muteki = 60;
    hits = 0;
    chain = 0;
    c_time = 0;
    h_time = 0;
    move_type = 'h';

	return;
}
//-----------------------------------------------------------------------------
void Player::BulletAbsorb(void)
{
	if (absorbed < 6000)
	{
		ScoreAdd(1);
		absorbed++;
	}

	if (pow < 1000)
		pow += 10;

	if (gun_next > 0)
	{
		gun_xp++;

		if (gun_xp >= gun_next)
		{
			gun_lvl++;

			if (gun_lvl >= 5)
				gun_next = -1;

			gun_next *= 3;
			gun_xp = 0;
		}
		
	}
	
	return;
}
//-----------------------------------------------------------------------------
bool Player::AllowSpawn(void) const
{
	return allow_spawn;
}
//-----------------------------------------------------------------------------
void Player::PowerAdd(short val)
{
	if (val > 0)
		pow += val;
	
	if (pow > 1000)
		pow = 1000;

	return;
}
//-----------------------------------------------------------------------------
void PlayerClsnProj(OBJECTS &o)//fix
{
	std::list<Projectile>::iterator it;
		
	for(it = o.p.p_list.begin(); it != o.p.p_list.end(); ++it)
	{
		if (it->GetId() >= 10)
        {
			if (ClsnCheck(o.p1.GetClsn(), it->GetClsn())) //player hit something
            {
				if (o.p1.GetStateNo() != 101 || (it->CanAbsorb() == false))
                {
					o.p1.GotHit((o.p1.GetStateNo() != 101) ? it->GetDamage() : (it->GetDamage() << 1));
                    return;
                }
                else
                {
					o.p1.BulletAbsorb();
					it->SetStateNo(5901);
                }
            }
        }
	}

	return;
}
//-----------------------------------------------------------------------------
void PlayerClsnNpc(OBJECTS &o)//fix
{	
	std::list<Npc>::iterator it;
		
	for(it = o.n.n_list.begin(); it != o.n.n_list.end(); ++it)
	{
        if (it->GetId() >= 0)
        {
            if (ClsnCheck(o.p1.GetClsn(), it->GetClsn())) //player hit something
            {
                if (it->GetId() == 1000)//power collectable(ignore invicibility)
                {
                    o.p1.PowerAdd(it->GetLife());
                    o.p1.ScoreAdd(10);
                    it->SetAlive(false);
                    it->SetVisible(false);
                    it->SetLife(999);
                }
                else//regular enemy
                {
                    if (!(o.p1.GetInv()))
                    {
						o.p1.GotHit((o.p1.GetStateNo() != 101) ? 1 : 2);
                        return;
                    }
                }
            }
        }
    }

	return;
}
//-----------------------------------------------------------------------------
void PlayerActions(OBJECTS &o, ObjManFx &om, short &pad, AUDIO &a)
{
    o.p1.Action1(a);
    o.p1.CmdCheck(om, pad);

    if (!(o.p1.GetPause()) || (o.p1.GetPauseType()))
    {
		if((o.p1.GetLife() > 0))
		{
			if (!(o.p1.GetInv()))
				PlayerClsnProj(o);

			PlayerClsnNpc(o);
		}
        o.n.SetSpawn(o.p1.AllowSpawn());
		
		o.p1.PlayerStates(om, pad, a);
		o.p1.Move(o.cam);
		

        //if ((o.p1.state_no == 6000) && (o.p1.time == 66))//create screen shockwave
            //SfxSetWave(&(o.sfx), 0);
    }

    return;
}
//-----------------------------------------------------------------------------
//=============================================================================
//Projectile class
//=============================================================================
Projectile::Projectile(void)
{
	memset(&ex, 0, (sizeof(int) * 5));
	return;
}
//-----------------------------------------------------------------------------
Projectile::~Projectile(void)
{
	/*if (anim.anim_clip != NULL)
	{
		if (anim.frame > 1)
			delete[] (anim.anim_clip);
		else
			delete (anim.anim_clip);

	}*/
	
	return;
}
//-----------------------------------------------------------------------------
void Projectile::Init(int ang, double spd)
{
    const unsigned short pw = 46;
    const unsigned short ph = 23;

    blur = false;
    alive = true;
    visible = true;

    switch(id)
    {
        case 0://players normal projectile
            SetStateNo(0);
            SetVel(ang, spd);
            sx = 22; sy = 10;
            clsn.w = 12; clsn.h = 6;
            clsn.x = (static_cast<short>(x) - (clsn.w >> 1));
            clsn.y = (static_cast<short>(y) - (clsn.h >> 1));
            spr_id = 0;
            damage = ((RandNum(30) == 7) ? 10 : 1);
            SetAnim(pw, ph, 3);
            anim.delay = 9999;
        break;
        case 1://players normal projectile level 1-4
            SetStateNo(1);
            SetVel(ang, spd);
            sx = 22; sy = 10;
            clsn.w = 20; clsn.h = 8;
            clsn.x = (static_cast<short>(x) - (clsn.w >> 1));
            clsn.y = (static_cast<short>(y) - (clsn.h >> 1));
            spr_id = 0;
            damage = ((RandNum(100) == 7) ? 10 : 1);
            SetAnim(pw, ph, 5);
            anim.delay = 9999;
            anim.frame = 3;
			additive = true;
        break;
        case 2://players normal projectile level 5
            SetStateNo(0);
            SetVel(ang, spd);
            sx = 22; sy = 10;
            clsn.w = 7; clsn.h = 7;
            clsn.x = (static_cast<short>(x) - (clsn.w >> 1));
            clsn.y = (static_cast<short>(y) - (clsn.h >> 1));
            spr_id = 0;
            damage = ((RandNum(70) == 7) ? 12 : 2);
            SetAnim(pw, ph, 9);
            anim.delay = 9999;
            anim.frame = 4;
			additive = true;
        break;
        case 3://normal buster shot
            SetStateNo(3);
            SetVel(ang, spd);
            sx = 43; sy = 40;
            clsn.w = 76; clsn.h = 76;
            clsn.x = (static_cast<short>(x) - (clsn.w >> 1));
            clsn.y = (static_cast<short>(y) - (clsn.h >> 1));
            spr_id = 9;
            damage = 15;
            SetAnim(87, 81, 2);
            anim.delay = 1;
            ex[0] = 0;
            ex[1] = 0;
			additive = true;
        break;
        case 4://super buster shot
            SetStateNo(4);
            SetVel(ang, spd);
            sx = 72; sy = 75;
            clsn.w = 138; clsn.h = 138;
            clsn.x = (static_cast<short>(x) - (clsn.w >> 1));
            clsn.y = (static_cast<short>(y) - (clsn.h >> 1));
            spr_id = 8;
            damage = 100;
            SetAnim(144, 151, 2);
            anim.delay = 1;
        break;
        case 10://enemy big bullet
            SetStateNo(10);
            SetVel(ang, spd);
            damage = 1;
            sx = 22; sy = 10;
            clsn.w = 6; clsn.h = 6;
            clsn.x = (static_cast<short>(x) - (clsn.w >> 1));
            clsn.y = (static_cast<short>(y) - (clsn.h >> 1));
            spr_id = 0;
            SetAnim(pw, ph, 7);
            anim.delay = 9999;
            anim.frame = 1;
            can_absorb = true;
        break;
        case 11://enemy small bullet
            SetStateNo(11);
            SetVel(ang, spd);
            damage = 1;
            sx = 22; sy = 10;
            clsn.w = 2; clsn.h = 2;
            clsn.x = (static_cast<short>(x) - (clsn.w >> 1));
            clsn.y = (static_cast<short>(y) - (clsn.h >> 1));
            spr_id = 0;
            SetAnim(pw, ph, 7);
            anim.delay = 9999;
            anim.frame = 2;
            can_absorb = true;
        break;
        case 12://zig zag bullet
            SetStateNo(12);
            SetVel(ang, spd);
            sx = 22; sy = 10;
            damage = 1;
            clsn.w = 4; clsn.h = 4;
            clsn.x = (static_cast<short>(x) - (clsn.w >> 1));
            clsn.y = (static_cast<short>(y) - (clsn.h >> 1));
            spr_id = 0;
            SetAnim(pw, ph, 7);
            anim.delay = 9999;
            anim.frame = 1;
            can_absorb = true;
        break;
        case 13://big ship big shot
            SetStateNo(13);
            SetVel(ang, spd);
            damage = 2;
            sx = 33; sy = 29;
            clsn.w = 64; clsn.h = 34;
            clsn.x = (static_cast<short>(x) - (clsn.w >> 1));
            clsn.y = (static_cast<short>(y) - (clsn.h >> 1));
            spr_id = 4;
            SetAnim(76, 61, 4);
            anim.delay = 2;
            can_absorb = false;
			additive = true;
        break;
        case 14://can't absorb bullet
            SetStateNo(14);
            SetVel(ang, spd);
            damage = 1;
            sx = 22; sy = 10;
            clsn.w = 6; clsn.h = 6;
            clsn.x = (static_cast<short>(x) - (clsn.w >> 1));
            clsn.y = (static_cast<short>(y) - (clsn.h >> 1));
            spr_id = 0;
            SetAnim(pw, ph, 8);
            anim.frame = 5;
            anim.delay = 4;
            can_absorb = false;
        break;
        case 100://vert shake
            SetStateNo(100);
            damage = 0;
            sx = 0; sy = 0;
            clsn.w = 0; clsn.h = 0;
            clsn.x = 0;
            clsn.y = 0;
            spr_id = 0;
            visible = false;
            can_absorb = false;
        break;
        case 101://hori shake
            SetStateNo(101);
            damage = 0;
            sx = 0; sy = 0;
            clsn.w = 0; clsn.h = 0;
            clsn.x = 0;
            clsn.y = 0;
            spr_id = 0;
            visible = false;
            can_absorb = false;
        break;
        default:;
    }
    return;
}
//-----------------------------------------------------------------------------
void Projectile::Move(const Camera &c)
{
    MoveObj();

    if (vx != 0.0 && fric > 0.0)
    {
        if (vx > fric)
            vx -= fric;
        else if (vx < fric)
            vx += fric;
        else
        {
            fric = 0.0;
            vx = 0.0;
        }
    }

    if (vy != 0.0 && grav > 0.0)
    {
        if (vy > grav)
            vy -= grav;
        else if (vy < grav)
            vy += grav;
        else
        {
            grav = 0.0;
            vy = 0.0;
        }
    }

    //offscreen? then get rid of it
    if ((x <= (c.GetX() - 40)) || ((x - clsn.w) >= (c.GetX()+SCREEN_WIDTH + 10))
    || (y <= (c.GetY() - 20)) || (y >= (c.GetY()+SCREEN_HEIGHT + 10)))
    {
        visible = false;
        alive = false;
    }

    clsn.x = (static_cast<int>(x) - (clsn.w >> 1));
    clsn.y = (static_cast<int>(y) - (clsn.h >> 1));

    CalcAngle();
    CalcSpeed();

    return;
}
//-----------------------------------------------------------------------------
void Projectile::States(ObjManFx &om, AUDIO &a)
{
	switch(state_no)
    {
        case 0: //players projectile
        break;
		case 1://players projectile
			if (time >= 3 && time <= 6)
			{
				vx = 6;
				vy = 0;
			}
		break;
        case 3:

            if (time == 1)
                PlaySound(a, SND_COMMON, 3, 0);

            if (!(ex[0]))
                ex[1] += 2;
            else
                ex[1] -= 2;

            if (ex[1] >= 10)
                ex[0] = 1;

            if (ex[1] <= -10)
                ex[0] = 0;

            if (time >= 2)
				om.AddVfx(7, -2, (x - 5), (y + static_cast<double>(ex[1])), 0);
        break;
        case 4:

            if (time == 1)
                PlaySound(a, SND_COMMON, 4, 0);

            if (!(static_cast<int>(time) % 2))
            {
                for(int i = 0; i < 5; ++i)
                om.AddVfx(8, -2, x, (y + static_cast<double>(30)) -
                static_cast<double>(RandNum(60)), 180);
            }
        break;
        case 10://big bullet
			if (!(GetIntTime() % 2))
				anim.frame = ((anim.frame == 1) ? 5 : 1);
		break;
        case 11: //small bullet
			if (!(GetIntTime() % 2))
				anim.frame = ((anim.frame == 2) ? 6 : 2);
        break;
        case 12: //zig zag bullet

            if (ex[0] == 1)
                ex[1] -= 3;
            else
                ex[1] += 3;

            if (ex[1] <= -90)
                ex[0] = -1;
            else if (ex[1] >= 90)
                ex[0] = 1;

            if (time <= 99)
                SetVel(180 + ex[1], 1.0);

            if (time == 100)
                SetVel((135 + RandNum(90)), 1.0);

        break;
        case 13: //big ship big shot

            if (GetIntTime() % 5)
            {
                for(int i = 0; i < 2; ++i)
                om.AddVfx(5, -2, (x + static_cast<double>(30)), (y + static_cast<double>(10)) -
                static_cast<double>(RandNum(20)), 180);
            }

        break;
        case 14: //unabsorbable

            if (time > 9)
            {
                anim.frame = 5;
                anim.anim_time = 0;
                time = 0;
            }

        break;
        case 100://verticle shake set
            scr_off.shake_y = 8;
            SetStateNo(102);
        break;
        case 101://horizontal shake set
            scr_off.shake_x = 8;
            SetStateNo(103);
        break;
        case 102://verticle shake
            if (scr_off.shake_y == 0)
                SetStateNo(5900);

            scr_off.shake_y *= -1;

            if (scr_off.shake_y >= 0)
                scr_off.shake_y--;

        break;
        case 103://horizontal shake
            if (scr_off.shake_x == 0)
                SetStateNo(5900);

            scr_off.shake_x *= -1;

            if (scr_off.shake_x >= 0)
                scr_off.shake_x--;
        break;
        case 5900: //projectile is dead
            if (time == 0)
            {
                visible = false;
                alive = false;
                om.AddVfx(2, -2, (x + vx), y, 0);
            }
        break;
        case 5901: //projectile is absorbed
            if (time == 0)
            {
                PlaySound(a, SND_COMMON, 6, 0);
                visible = false;
                alive = false;
                om.AddVfx(2, -2, (x + vx), y, 0);
            }
        break;
        default:;
    }

    if (speed > 2.0)
        blur = true;

    if (blur && (speed < 1.0))
        blur = false;

    return;
}
//-----------------------------------------------------------------------------
void Projectile::SetId(short i)
{
	id = i;
	return;
}
//-----------------------------------------------------------------------------
short Projectile::GetId(void) const
{
	return id;
}
//-----------------------------------------------------------------------------
short Projectile::GetDamage(void) const
{
	return damage;
}
//-----------------------------------------------------------------------------
void Projectile::SetNId(int i)
{
	n_id = i;
	return;
}
//-----------------------------------------------------------------------------
void ProjectileActions(OBJECTS &o, ObjManFx &om, AUDIO &a)
{
	if (!(o.p.p_list.size()))
		return;
	else if (o.p1.GetPauseType())
		o.p.KillAll();
	else
	{
		std::list<Projectile>::iterator it;
		
		for(it = o.p.p_list.begin(); it != o.p.p_list.end();)
		{
			it->TimeInc();
			it->States(om, a);
			it->Move(o.cam);

			if (it->GetAnimData(AN_DELAY) != 9999)
				it->AnimInc();
			
			if (!(it->IsAlive()))
			{
				it->ClearAnim();
				it =  o.p.p_list.erase(it);
				o.p.DecCount();
			}
			else
				++it;
		}
	}

    return;
}
//-----------------------------------------------------------------------------
void Projectile::DrawProjectile(SURFACES *s, int xx, int yy)
{
	if (visible)
	{
		if (additive)
			SDL_SetTextureBlendMode(s->misc_spr[spr_id].spr, SDL_BLENDMODE_ADD);

		SetSpr((static_cast<int>(x) - sx) - xx, (static_cast<int>(y) - sy) - yy, s->bg,
        s->misc_spr[spr_id].spr, &(anim.anim_clip[anim.frame]));

		if (additive)
			SDL_SetTextureBlendMode(s->misc_spr[spr_id].spr, SDL_BLENDMODE_BLEND);
	}
	
	if (CLSN)
		DrawClsn((s->bg), GetClsn());
  
    return;
}
//-----------------------------------------------------------------------------
bool Projectile::CanAbsorb(void) const
{
	return can_absorb;
}
//=============================================================================
//Npc class
//=============================================================================
Npc::Npc(void)
{
	//link = NULL;
	memset(&ex, 0, (sizeof(int) * 5));
	return;
}
//-----------------------------------------------------------------------------
Npc::~Npc(void)
{
	return;
}
//-----------------------------------------------------------------------------
void Npc::SetId(short i)
{
	id = i;
	return;
}
//-----------------------------------------------------------------------------
void Npc::SetNId(int i)
{
	n_id = i;
	return;
}
//-----------------------------------------------------------------------------
void Npc::SetLevel(char lvl)
{
	level = lvl;
	return;
}
//-----------------------------------------------------------------------------
int NpcLifeSet(int life, int level)
{
    return static_cast<int>(life + (life * (static_cast<double>(level) * 0.3)));
}
//-----------------------------------------------------------------------------
double NpcShotSet(double spd, int level)
{
    return (spd + (static_cast<double>(level) * 0.05));
}
//-----------------------------------------------------------------------------
void Npc::SetEx(int index, int num)
{
	if (index < 0)
		index = 0;

	if (index > 4)
		index = 4;

	ex[index] = num;
	
	return;
}
//-----------------------------------------------------------------------------
short Npc::GetId(void) const
{
	return id;
}
//-----------------------------------------------------------------------------
void Npc::Init(bool difficult)
{
    blur = false;
    alive = true;
    visible = true;
	hard = difficult;

    switch(id)
    {
        case 0://zoom around ship
            SetStateNo(-1, true);
            spr_id = 1;
            life = NpcLifeSet(5, level);
			AdjClsn(10, 16);
            SetAnim(23, 25, 2);
            sx = 13; sy = 12;
            anim.delay = 9999;
            ctrl = true;
        break;
        case 1://enter, shoot, leave ship(is now choppa)
			SetStateNo(-1, true);
			spr_id = 12;
			life = NpcLifeSet(2, (level));
			AdjClsn(18, 16);
			SetAnim(59, 24, 4);
			sx = 23; sy = 11;
			anim.delay = 2;
			ctrl = false;
        break;
        case 2://big ship, bomber
            SetStateNo(-1, true);
            spr_id = 3;
            life = NpcLifeSet(50, (level));
			AdjClsn(38, 86);
            SetAnim(53, 103, 2);
            sx = 27; sy = 51;
            anim.delay = 9999;
            ctrl = false;
        break;
        case 3://brick
            SetStateNo(-1, true);
            spr_id = 5;
            life = 5000;
			AdjClsn(34, 36);
            SetAnim(37, 39, 1);
            sx = 18; sy = 19;
            anim.delay = 9999;
            ctrl = false;
        break;
        case 4://brick leech
            SetStateNo(-1, true);
            spr_id = 4;
            life = NpcLifeSet(1, (level));
			AdjClsn(24, 12);
            SetAnim(31, 23, 4);
            sx = 13; sy = 11;
            anim.delay = 3;
            ctrl = false;
        break;
        case 5://mine enemy
            SetStateNo(-1, true);
            spr_id = 6;
            life = NpcLifeSet(2, (level));
			AdjClsn(20, 20);
            SetAnim(24, 24, 5);
            sx = 12; sy = 12;
            anim.delay = 8;
            ctrl = false;
        break;
        case 6://formation enemy
			SetStateNo(-1, true);
			spr_id = 9;
			life = NpcLifeSet(1, (level));
			AdjClsn(18, 16);
			SetAnim(26, 25, 5);
			sx = 12; sy = 11;
			anim.delay = 7;
			ctrl = false;
        break;
        case 7://square wave enemy
            SetStateNo(-1, true);
            spr_id = 10;
            life = NpcLifeSet(2, (level));
			AdjClsn(20, 20);
            SetAnim(26, 25, 3);
            sx = 13; sy = 12;
            anim.delay = 2;
            ctrl = false;
        break;
        case 8://bullet wall enemy
            SetStateNo(-1, true);
            spr_id = 0;
            life = 1;
			AdjClsn(10, 10);
            SetAnim(0, 0, 1);
            sx = 0; sy = 0;
            visible = false;
            anim.delay = 9999;
            ctrl = false;
        break;
        case 9://zig zag enemy
            SetStateNo(-1, true);
            spr_id = 11;
            life = NpcLifeSet(2, (level));
			AdjClsn(20, 20);
            SetAnim(26, 25, 9);
            sx = 13; sy = 12;
            anim.delay = 2;
            ctrl = false;
        break;
        case 10://choppa enemy(now other type)
			SetStateNo(-1, true);
			spr_id = 2;
			life = NpcLifeSet(1, (level));
			AdjClsn(20, 6);
			SetAnim(32, 18, 2);
			sx = 18; sy = 7;
			anim.delay = 9999;
			ctrl = true;
        break;
        case 11://diamond enemy
            SetStateNo(-1, true);
            spr_id = 13;
            life = NpcLifeSet(1, (level));
			AdjClsn(18, 18);
            SetAnim(21, 21, 3);
            sx = 10; sy = 10;
            anim.delay = 2;
            ctrl = false;
        break;
        case 100://kill screen
            SetStateNo(-1, true);
            spr_id = 8;
            life = 5000;
			AdjClsn(38, 86);
            SetAnim(53, 103, 2);
            sx = 27; sy = 51;
            anim.delay = 9999;
            ctrl = false;
        break;
        case 1000://super shot energy
            SetStateNo(-1, true);
            spr_id = 7;
            life = 10;
            vx = vy = 0.0f;
			AdjClsn(18, 18);
            SetAnim(17, 17, 6);
            sx = 8; sy = 8;
            anim.delay = 9999;
            ctrl = false;
        break;
        case 9999://dummy ship, for testing
            SetStateNo(-1, true);
            spr_id = 3;
            life = 1000;
			AdjClsn(38, 86);
            SetAnim(53, 103, 2);
            sx = 27; sy = 51;
            anim.delay = 9999;
            ctrl = false;
        break;
        default:;
    }

    return;
}
//-----------------------------------------------------------------------------
void Npc::Move(const Camera &c)
{
    MoveObj();

    if (vx != 0 && fric > 0.0)
    {
        if (vx > fric)
            vx -= fric;
        else if (vx < fric)
            vx += fric;
        else
        {
            fric = 0.0;
            vx = 0.0;
        }
    }

    if (vy != 0 && grav > 0.0)
    {
        if (vy > grav)
            vy -= grav;
        else if (vy < grav)
            vy += grav;
        else
        {
            grav = 0.0;
            vy = 0.0;
        }
    }

    //offscreen?
    if ((x <= (c.GetX() - 10)) || (x >= (c.GetX() + SCREEN_WIDTH + 10)) || (y <= (c.GetY() - 10))
    || (y >= (c.GetY() + SCREEN_HEIGHT + 10)))
        visible = false;

    visible = true;
    clsn.x = (static_cast<short>(x) - (clsn.w >> 1));
    clsn.y = (static_cast<short>(y) - (clsn.h >> 1));

    CalcAngle();
    CalcSpeed();

    return;
}

//-----------------------------------------------------------------------------
void Npc::DrawNpc(bool big, SURFACES *s, int xx, int yy)
{
    if (big == true)//draw big enemies
    {
        if (visible && id == 2)
        {
			if (additive)
				SDL_SetTextureBlendMode(s->obj_spr[spr_id].spr, SDL_BLENDMODE_ADD);
			
			SetSpr((static_cast<int>(x) - sx) - xx, (static_cast<int>(y) - sy) - yy, 
			s->bg, s->obj_spr[spr_id].spr, &(anim.anim_clip[anim.frame]));

				if (additive)
					SDL_SetTextureBlendMode(s->obj_spr[spr_id].spr, SDL_BLENDMODE_BLEND);

            if (blur)
                DrawBlurObj(s->obj_spr[spr_id].spr, s->bg,
                &(anim.anim_clip[anim.frame])
                , (static_cast<int>(x) - sx) - xx, (static_cast<int>(y) - sy) - yy, angle, speed);
        }
    }
	else//draw others
    {
        if (visible && id != 2)
        {
            if (additive)
				SDL_SetTextureBlendMode(s->obj_spr[spr_id].spr, SDL_BLENDMODE_ADD);
			
			SetSpr((static_cast<int>(x) - sx) - xx, (static_cast<int>(y) - sy) - yy, 
			s->bg, s->obj_spr[spr_id].spr, &(anim.anim_clip[anim.frame]));

			if (additive)
				SDL_SetTextureBlendMode(s->obj_spr[spr_id].spr, SDL_BLENDMODE_BLEND);

            if (blur)
                DrawBlurObj(s->obj_spr[spr_id].spr, s->bg,
                &(anim.anim_clip[anim.frame])
                , (static_cast<int>(x) - sx) - xx, (static_cast<int>(y) - sy) - yy, angle, speed);
        }
    }

	if (CLSN)
		DrawClsn((s->bg), GetClsn());

    return;
}
//-----------------------------------------------------------------------------
bool Npc::Clsn(ProjectileHead &pr)//fix
{
    std::list<Projectile>::iterator i;

	if (muteki || !alive) // can't be hit don't bother
        return false;

	for(i = pr.p_list.begin(); i != pr.p_list.end(); ++i)
	{
        if (((i->GetId() <= 9) || (i->GetId() <= 12 && id == 3)))
        {
            if (ClsnCheck(clsn, i->GetClsn()))
            {
                //muteki = 1;

                if (i->GetId() != 3 && i->GetId() != 4)
                    i->SetStateNo(5900);
                move_type = 'h';
                life -= (i->GetDamage());

                if (i->GetId()<= 9)
                    return true;
                else
                    return false;
            }
        }
    }

    return false;
}
//-----------------------------------------------------------------------------
void NpcListAdd(NpcHead &main, NpcHead &list)
{
	main.n_list.splice(main.n_list.begin(), list.n_list);
	main.AddCount(list.GetCount());
	main.AddId(list.GetCount());

    return;
}
//-----------------------------------------------------------------------------
void NpcActions(OBJECTS &o, ObjManFx &om, AUDIO a)
{
    NpcHead queue_head;
	queue_head.SetId(o.n.GetId());

	if (!(o.n.n_list.size()))
		return;
	else
	{
		std::list<Npc>::iterator it;
		
		for(it = o.n.n_list.begin(); it != o.n.n_list.end();)
		{
			if (it->GetInv() > 0)
				it->InvInc();

			it->TimeInc();
			if (it->GetAnimData(AN_DELAY) != 9999)
				it->AnimInc();

			if (it->Clsn(o.p))
				o.p1.HitAdd();

			it->Ai(o.p1);
			it->States(o.p1, om, a);
			it->Move(o.cam);

			if (!(it->IsAlive()))
			{
				o.n.DecEnemyType(it->GetId());

				if (it->GetLife() <= 0)//died
				{
					if (o.p1.GetPow() < 1000)//don't spawn pow ups if p1 is maxed
						queue_head.Add(1000, it->GetX(), it->GetY());

					if(it->GetId() != 2)//not a big ship then give 50
						o.p1.ScoreAdd(50);
					else
						o.p1.ScoreAdd(100);

					o.p1.ChainAdd();
					o.n.EnemyAdd(-1);//decrease enemycount
				}

				it->ClearAnim();
				it =  o.n.n_list.erase(it);
				o.n.CountAdd(-1);//decrease count

				if (o.n.GetEnemy() <= 0)
				{
					int cur_lvl = o.n.GetLevel();

					o.n.Advance();

					PlaySound(a, SND_COMMON, 25, 0);//wave up!

					//if (cur_lvl != o.n.GetLevel()) //level went up so spd up bg
						//SbgSpeedUp((o.sbg));
				}
			}
			else
			{
				if (it->GetId() == 2)
					it->SetEx(4, static_cast<int>(o.n.GetEnemyCount(E_BIG)));

				++it;
			}
		}
	}
	
	if (queue_head.GetCount() > 0)
        NpcListAdd(o.n, queue_head);
	//npc can't be added to list while processing so a seperate list is created
    //then that list is transfered to main list as one chunk
	
    return;
}
//-----------------------------------------------------------------------------
void Npc::Ai(Player &p)
{
    int action = RandNum(1000);

    if (life <= 0)//die
        SetStateNo(5900, false);

    if(id == 0) //zoomer
    {

        if (!(ctrl))//don't bother if npc doesn't have control
            return;

        if ((action >= 0 && action <= 199) && (time >= 21) && state_no == 0)
            SetStateNo(20, false);//move somewhere

        if ((action >= 300 && action <= 530) && (time >= 20) && state_no != 200)
            SetStateNo(200, false);//shoot at player

        if ((action >= 600 && action <= 645) && (time >= 20)
        && state_no != 202 && level >= 1)
            SetStateNo(202, false);//shoot at player(rapid)

        if ((action >= 700 && action <= 750) && (time >= 19)
        && state_no != 201 && level >= 2)
            SetStateNo(201, false);//shoot at player(spread)

        return;
    }

    if(id == 2) //big ship
    {

        if (!(ctrl))//don't bother if npc doesn't have control
            return;

        if ((action >= 0 && action <= 299) && (time >= 21) && state_no == 0)
            SetStateNo(20, false);//move somewhere

        if ((action >= 500 && action <= 530) && (time >= 20)
        && state_no != 200)
            SetStateNo(200, false);//spread

        if ((action >= 600 && action <= 645) && (time >= 20)
        && state_no != 202 && (MathGetDist(static_cast<int>(y), static_cast<int>(p.GetY())) <= 15) 
		&& level >= 2)
            SetStateNo(202, false);//beam

        if ((action >= 700 && action <= 750) && (time >= 19)
        && state_no != 201 && level >= 3)
            SetStateNo(201, false);//blanket

        if ((action >= 751 && action <= 780) && (time >= 19)
        && state_no != 201)
            SetStateNo(203, false);//shoot at player

        return;
    }

    return;
}
//-----------------------------------------------------------------------------
void Npc::States(Player &p, ObjManFx &om, AUDIO &a)
{
    switch(id)
    {
        case 0:
            Zoomer(p, om, a);
        break;
        case 1:
            PartTime(p, om, a);
        break;
        case 2:
            BigShip(p, om, a);
        break;
        case 3:
            Brick(p, om, a);
        break;
        case 4:
            Leech(p, om, a);
        break;
        case 5:
            Mine(p, om, a);
        break;
        case 6:
            Formation(p, om, a);
        break;
        case 7:
            SquareWave(p, om, a);
        break;
        case 8:
            Wall(p, om, a);
        break;
        case 9:
            ZigZag(p, om, a);
        break;
        case 10:
            Choppa(p, om, a);
        break;
        case 11:
            Diamond(p, om, a);
        break;
        case 1000:
            PowUp(p, om, a);
        break;
        case 100:
            KillScreen(p, om, a);
        break;
        default:;
        break;
    }

    if (speed > 2.0)
        blur = true;

    if (blur && speed < 1.0)
        blur = false;

    return;
}
//=============================================================================
//Vfx class
//=============================================================================
Vfx::Vfx(void)
{
	memset(&ex, 0, (sizeof(int) * 5));
	alpha = 255;
	return;
}
//-----------------------------------------------------------------------------
Vfx::~Vfx(void)
{
	return;
}
//-----------------------------------------------------------------------------
void Vfx::Init(int angle)
{
    unsigned short set1[2] = {23, 21};

    switch(id)
    {
        case 0://players after image
            SetStateNo(0);
            vx = 0.0;
            vy = 0.0;
            visible = true;
            alive = true;
            back = false;
            sx = 17; sy = 11;
            spr_id = 1;
            anim.delay = 9999;
            anim.frame = 0;
            SetAnim(33, 22, 1);
        break;
        case 1://players shot burst
            SetStateNo(1);
            vx = 0.0;
            vy = 0.0;
            visible = true;
            alive = true;
            back = false;
            sx = 10; sy = 10;
            spr_id = 2;
            anim.delay = 1;
            SetAnim(set1[0], set1[1], 9);
            anim.total_frames = 3;
			additive = true;
        break;
        case 2://players shot hit
            SetStateNo(2);
            vx = 0.0;
            vy = 0.0;
            visible = true;
            alive = true;
            back = false;
            sx = 10; sy = 10;
            spr_id = 2;
            SetAnim(set1[0], set1[1], 9);
            anim.delay = 2;
            anim.frame = 3;
			additive = true;
        break;
        case 3://enemy explode
            SetStateNo(3);
            vx = 0.0;
            vy = 0.0;
            visible = true;
            alive = true;
            back = false;
            sx = 42; sy = 40;
            spr_id = 3;
            SetAnim(86, 81, 8);
            anim.delay = 2;
			additive = true;
        break;
        case 4://beam charge
            SetStateNo(4);
            vx = 0.0;
            vy = 0.0;
            visible = true;
            alive = true;
            back = false;
            sx = 43; sy = 43;
            spr_id = 3;
            SetAnim(86, 81, 14);
            anim.frame = 8;
            anim.delay = 3;
			additive = true;
        break;
        case 5://beam particle
            SetStateNo(5);
            SetVel(angle, static_cast<double>(RandNum(3) + 1));
            visible = true;
            alive = true;
            back = false;
            sx = 10; sy = 10;
            spr_id = 2;
            SetAnim(set1[0], set1[1], 15);
            anim.frame = 9;
            anim.delay = 5;
			additive = true;
        break;
        case 6://lens flare
            SetStateNo(6);
            visible = true;
            alive = true;
            back = true;
            sx = 122; sy = 122;
            spr_id = 6;
            SetAnim(277, 242, 12);
            anim.delay = 2;
			additive = true;
        break;
        case 7://buster shot tail
            SetStateNo(7);
            visible = true;
            alive = true;
            back = false;
            sx = 21; sy = 21;
            spr_id = 10;
            SetAnim(44, 42, 6);
            anim.delay = 2;
			additive = true;
        break;
        case 8://buster shot particle
            SetStateNo(7);
            SetVel(angle, static_cast<double>(RandNum(3) + 1));
            visible = true;
            alive = true;
            back = false;
            sx = 21; sy = 21;
            spr_id = 11;
            SetAnim(44, 42, 6);
            anim.delay = 2;
			additive = true;
        break;
        case 9://buster shot burst
            SetStateNo(7);
            visible = true;
            alive = true;
            back = false;
            sx = 0; sy = 16;
            spr_id = 12;
            SetAnim(67, 33, 6);
            anim.delay = 2;
			additive = true;
        break;
        case 10://die burst
            SetStateNo(7);
            visible = true;
            alive = true;
            back = false;
            sx = 128; sy = 40;
            spr_id = 13;
            SetAnim(256, 81, 7);
            anim.delay = 2;
			additive = true;
        break;
        case 11://burst particle
            SetStateNo(5);
            SetVel(RandNum(360), static_cast<double>(RandNum(7) + 1));
            visible = true;
            alive = true;
            back = false;
            sx = 10; sy = 10;
            spr_id = 2;
            SetAnim(set1[0], set1[1], 15);
            anim.frame = 9;
            anim.delay = 5;
			additive = true;
        break;
        case 12://hit zone
            SetStateNo(12);
            visible = true;
            alive = true;
            back = false;
            sx = 3; sy = 2;
            spr_id = 14;
            SetAnim(7, 5, 4);
            anim.delay = 4;
        break;
        case 13://square particle
            SetStateNo(13);
            visible = true;
            alive = true;
            back = false;
            sx = 3; sy = 3;
            spr_id = 15;
            SetAnim(7, 7, 4);
            anim.delay = 2;
        break;
        case 14://noise effect
            SetStateNo(14);
            visible = false;
            alive = true;
            back = false;
            sx = 0; sy = 0;
            spr_id = 16;
            SetAnim(34, 34, 6);
            anim.delay = 3;
        break;
		case 15://b shot particle
            SetStateNo(5);
            SetVel((RandNum(40) + 340), static_cast<double>(RandNum(7) + 3));
            visible = true;
            alive = true;
            back = false;
            sx = 10; sy = 10;
            spr_id = 2;
            SetAnim(set1[0], set1[1], 15);
            anim.frame = 9;
            anim.delay = 5;
			additive = true;
        break;
        default:;
        break;
    }

    return;
}
//-----------------------------------------------------------------------------
void Vfx::SetId(short i)
{
	id = i;
	return;
}
//-----------------------------------------------------------------------------
void Vfx::SetNId(int i)
{
	n_id = i;
	return;
}
//-----------------------------------------------------------------------------
void Vfx::Move(const Camera &c)
{
    MoveObj();

    //offscreen?
    if (state_no != 12 && ((x <= (c.GetX() - 20)) || (x >= (c.GetX()+ static_cast<double>(SCREEN_WIDTH + 20))) || 
		(y <= (c.GetY() - 20)) || (y >= (c.GetY()+ static_cast<double>(SCREEN_HEIGHT + 20)))))
    {
        visible = false;
        alive = false;
    }

    return;
}
//-----------------------------------------------------------------------------
void Vfx::States(double xx, double yy)
{
    switch(state_no)
    {
        case 0: //players after image
            //visible = ((visible) ? false  : true);
			alpha -= 20;

            if (time > 9)
            {
                alive = false;
                visible = false;
            }
        break;
        case 1: //players shot burst
            //visible = ((visible) ? false  : true);

            x = (xx + 20.0);

            if (y < yy)
                y = (yy + 5.0);
            else if (y > yy)
                y = (yy - 5.0);
            else
                y = (yy + 1.0);

            if (time > 6)
                alive = false;
        break;
        case 2: //players shot hit
			visible = ((visible) ? false  : true);

            if (anim.frame == 0)
                alive = false;
        break;
        case 3: //enemy explode
            if (time >= 15)
                alive = false;
        break;
        case 4: //beam charge
            if (anim.frame == 0)
                alive = false;
        break;
        case 5: //beam particle
            if (anim.frame < 9)
                alive = false;
        break;
        case 6: //lens flare
            if (time >= 10 && anim.frame == 0)
                alive = false;

            if (time == 8)
                anim.delay = 4;
        break;
        case 7: //shot tail
            if (anim.frame == 5 && time >= 5)
                alive = false;
        break;
        case 13: //square particle
                visible = ((visible) ? false  : true);

            if (time >= 8)
                alive = false;
        break;
        case 14:
        break;
        default:;
    }

    return;
}
//-----------------------------------------------------------------------------
void Vfx::DrawVfx(SURFACES *s, int xx, int yy)
{
    SDL_SetTextureAlphaMod(s->misc_spr[spr_id].spr, alpha);
	
	if (visible)
	{
		if (additive)
			SDL_SetTextureBlendMode(s->misc_spr[spr_id].spr, SDL_BLENDMODE_ADD);

		SetSpr((static_cast<int>(x) - sx) - xx, (static_cast<int>(y) - sy) - yy, 
		s->bg, s->misc_spr[spr_id].spr, &(anim.anim_clip[anim.frame]));

		if (additive)
			SDL_SetTextureBlendMode(s->misc_spr[spr_id].spr, SDL_BLENDMODE_BLEND);
	}

    return;
}
//-----------------------------------------------------------------------------
bool Vfx::GetBack(void) const
{
	return back;
}
//-----------------------------------------------------------------------------
void VfxBindPlayer(std::list<Vfx>::iterator v, const Player &p)
{
	v->SetPos(p.GetX(), p.GetY());

	v->SetVisible(p.GetVisible());

    if (p.GetStateNo() > 20)
        v->SetVisible(false);

    return;
}
//-----------------------------------------------------------------------------
void VfxActions(OBJECTS &o, ObjManFx &om)
{
	if (!(o.v.v_list.size()))
		return;
	else
	{
		std::list<Vfx>::iterator it;
		
		for(it = o.v.v_list.begin(); it != o.v.v_list.end();)
		{
			it->TimeInc();
			if (it->GetAnimData(AN_DELAY) != 9999)
				it->AnimInc();
			it->States(o.p1.GetX(), o.p1.GetY());
			it->Move(o.cam);

			if (it->GetStateNo() == 12)
				VfxBindPlayer(it, o.p1);
			
			if (!(it->IsAlive()))
			{
				it->ClearAnim();
				it =  o.v.v_list.erase(it);
				o.v.DecCount();
			}
			else
				++it;
		}
	}

    return;
}

//=============================================================================
//ProjectileHead class
//=============================================================================
ProjectileHead::ProjectileHead()
{
	count = 0;
	return;
}
//-----------------------------------------------------------------------------
ProjectileHead::~ProjectileHead()
{
	//if (count > 0 && link)
		//KillAll();

	if(p_list.size())
		p_list.clear();
	
	count = 0;
	//link = NULL;
	return;
}
//-----------------------------------------------------------------------------
void ProjectileHead::Add(int id, int nid, double x, double y, int angle, double speed)
{
    Projectile nu;
   
	//if (!(nu = (new (std::nothrow) Projectile)))
        //LogIt("Insufficient memory to create projectile.");

	nu.SetPos(x, y);
    nu.SetId(id);
    nu.SetNId(nid);

    nu.Init(angle, speed);

	p_list.push_front(nu);

    //nu->link = link;
    //link = nu;
    count++; //count for projectiles

    return;
}
//-----------------------------------------------------------------------------
void ProjectileHead::DecCount(void)
{
	--count;

    return;
}
//-----------------------------------------------------------------------------
void ProjectileHead::KillAll(void)
{//cycle though list and clear all
    /*Projectile temp;
    temp.link = link;
    Projectile *prev = &temp;
    Projectile *current = temp.link;

    while(prev->link != NULL)//total removal of all projectile objects
    {
        Delete(prev, current);
        current = temp.link;
    }

    link = temp.link;*/
	p_list.clear();
    count = 0;

    return;
}
//-----------------------------------------------------------------------------
int ProjectileHead::GetCount(void)const
{
	return count;
}
//-----------------------------------------------------------------------------
void ProjectileHead::DrawProjectiles(SURFACES* s, Camera& cam)
{
	if (!p_list.size())
		return;
	else
	{
		std::list<Projectile>::iterator i;
		for(i = p_list.begin(); i != p_list.end(); ++i)
			i->DrawProjectile(s, static_cast<int>(cam.GetX()), static_cast<int>(cam.GetY()));
	}

	return;
}
//=============================================================================
//VfxHead class
//=============================================================================
VfxHead::VfxHead()
{
	count = 0;
	//link = NULL;
	return;
}
//-----------------------------------------------------------------------------
VfxHead::~VfxHead()
{
	//if (count > 0 && link)
		//KillAll();
	
	if (v_list.size())
		v_list.clear();

	count = 0;
	
	return;
}
//-----------------------------------------------------------------------------
void VfxHead::Add(int id, int nid,  double x, double y, int angle)
{
    Vfx nu;

    //if (!(nu = (new (std::nothrow) Vfx)))
        //LogIt("Insufficient memory to create projectile.");

    nu.SetPos(x, y);
    nu.SetId(id);
    nu.SetNId(nid);

    nu.Init(angle);

    v_list.push_front(nu);
	//nu->link = link;
    count++;
    //link = nu;

    return;
}
//-----------------------------------------------------------------------------
void VfxHead::KillAll(void)
{
    /*Vfx temp;
    temp.link = link;
    Vfx *prev = &temp;
    Vfx *current = temp.link;

    while(prev->link != NULL)//total removal of all objects
    {
        Delete(prev, current);
        current = temp.link;
    }

    link = temp.link;*/
	v_list.clear();
    count = 0;

    return;
}
//-----------------------------------------------------------------------------
int VfxHead::GetCount(void)const
{
	return count;
}
//-----------------------------------------------------------------------------
void VfxHead::DecCount(void)
{
	--count;

    return;
}
//-----------------------------------------------------------------------------
void VfxHead::DrawVfxs(SURFACES* s, Camera& cam)
{
	if (!v_list.size())//nothing to draw
		return;
	else
	{
		std::list<Vfx>::iterator i;
		for(i = v_list.begin(); i != v_list.end(); ++i)
		{
			if (i->GetVisible() && !(i->GetBack()))
				i->DrawVfx(s, static_cast<int>(cam.GetX()), static_cast<int>(cam.GetY()));
		}
	}

	return;
}
//-----------------------------------------------------------------------------
void VfxHead::DrawVfxsBack(SURFACES* s, Camera& cam)
{
	if (!v_list.size())//nothing to draw
		return;
	else
	{
		std::list<Vfx>::iterator i;
		for(i = v_list.begin(); i != v_list.end(); ++i)
		{
			if (i->GetVisible() && i->GetBack())
				i->DrawVfx(s, static_cast<int>(cam.GetX()), static_cast<int>(cam.GetY()));
		}
	}

	return;
}
//=============================================================================
//NpcHead class
//=============================================================================
NpcHead::NpcHead(void)
{
	count = 0;//npcs in existance(related to list)
	id = 0;//unique id(based on npc count but doesn't reset)
	time = 0;//time before respawn
	big = 0;//big ship limit
	zoomer = 0;//
	part_time = 0;//
	brick = 0;//
	mine = 0;//
	form = 0;
	square = 0;
	wall = 0;
	zig = 0;
	choppa = 0;
	diamond = 0;

	wave = 1;
	en_max = 10;//max npcs allowable
	enemy = en_max;//npcs in existance(related to wave)
	level = 3;//difficulty level
	hard = true;//not normal mode
	can_spawn = false;

	//link = NULL;

	return;
}
//-----------------------------------------------------------------------------
NpcHead::~NpcHead(void)
{
	if (count > 0)
		KillAll();
	
	count = 0;
	//link = NULL;
	
	return;
}
//-----------------------------------------------------------------------------
void NpcHead::Add(int id1, double x, double y)
{
    Npc *nu = NULL;

    if (!(nu = (new (std::nothrow) Npc)))
        LogIt("Insufficient memory to create npc.");

    nu->SetPos(x, y);
    nu->SetId(id1);
    nu->SetLevel(level);
    nu->Init(hard);
    
    //link = nu;
    count++; //count for npcs
    id++; //npc count. does not go down. possible use for unique id
	//nu->link = link;
    nu->SetNId(id);

    if (id1 == 2 && big > 0)
		nu->SetEx(3, 1);
	n_list.push_front(*nu);
    IncEnemyType(id1);

    return;
}
//-----------------------------------------------------------------------------
void NpcHead::SetSpawn(bool ok)
{
	can_spawn = ok;
	return;
}
//-----------------------------------------------------------------------------
/*void NpcHead::Delete(Npc *prev, Npc *target)
{
    if (target->link == NULL)
        prev->link = NULL;
    else
        prev->link = target->link;

    delete[] (target);

    return;
}*/
//-----------------------------------------------------------------------------
void NpcHead::KillAll(void)
{
    /*Npc temp;
    temp.link = link;
    Npc *prev = &temp;
    Npc *current = temp.link;

    while(prev->link != NULL)//total removal of all objects
    {
        Delete(prev, current);
        current = temp.link;
    }

    link = temp.link;*/
	n_list.clear();
    count = 0;

    return;
}
//-----------------------------------------------------------------------------
void NpcHead::Advance(void)
{
    if (!(wave % 5) && ((level < 20 && !hard) || (level < 30 && hard)))
        (level)++;

    if (wave > 0)
        wave++;

    if (en_max < 60)
        en_max += 5;

    if (wave % 5)
        enemy = en_max;

    /*if (!wave)
    {
        enemy = 1000000;
        level = 100;
    }*/

    time = 0;

    return;
}
//-----------------------------------------------------------------------------
void NpcHead::IncEnemyType(int en)
{
	switch(en)
    {
        case 0:
            zoomer++;
        break;
        case 1:
            part_time++;
        break;
        case 2:
            big++;
        break;
        case 3:
            brick++;
        break;
        case 5:
            mine++;
        break;
        case 6:
            form++;
        break;
        case 7:
            square++;
        break;
        case 8:
            wall++;
        break;
        case 9:
            zig++;
        break;
        case 10:
            choppa++;
        break;
        case 11:
            diamond++;
        break;
        default:;
        break;
    }
	
	return;
}
//-----------------------------------------------------------------------------
void NpcHead::DecEnemyType(int en)
{
	switch(en)
    {
        case 0:
            zoomer--;
        break;
        case 1:
            part_time--;
        break;
        case 2:
            big--;
        break;
        case 3:
            brick--;
        break;
        case 5:
            mine--;
        break;
        case 6:
            form--;
        break;
        case 7:
            square--;
        break;
        case 8:
            wall--;
        break;
        case 9:
            zig--;
        break;
        case 10:
            choppa--;
        break;
        case 11:
            diamond--;
        break;
        default:;
        break;
    }
	return;
}
//-----------------------------------------------------------------------------
char NpcHead::GetEnemyCount(int en) const
{
	switch(en)
    {
        case 0:
            return zoomer;
        break;
        case 1:
            return part_time;
        break;
        case 2:
            return big;
        break;
        case 3:
            return brick;
        break;
        case 5:
            return mine;
        break;
        case 6:
            return form;
        break;
        case 7:
            return square;
        break;
        case 8:
            return wall;
        break;
        case 9:
            return zig;
        break;
        case 10:
            return choppa;
        break;
        case 11:
            return diamond;
        break;
        default:;
        break;
    }

	return 0;
}
//-----------------------------------------------------------------------------
void NpcHead::CountAdd(int c)
{
	count += c;
	return;
}
//-----------------------------------------------------------------------------
void NpcHead::EnemyAdd(int e)
{
	enemy += e;
	return;
}
//-----------------------------------------------------------------------------
int NpcHead::GetCount(void) const
{
	return count;
}
//-----------------------------------------------------------------------------
int NpcHead::GetEnemy(void) const
{
	return enemy;
}
//-----------------------------------------------------------------------------
char NpcHead::GetLevel(void) const
{
	return level;
}
//-----------------------------------------------------------------------------
bool NpcHead::CanSpawn(void) const
{
	return can_spawn;
}
//-----------------------------------------------------------------------------
void NpcHead::AddCount(int val)
{
	count += val;
	return;
}
//-----------------------------------------------------------------------------
void NpcHead::AddId(int val)
{
	id += val;
	return;
}
//-----------------------------------------------------------------------------
void NpcHead::SetId(int val)
{
	id = val;
	return;
}
//-----------------------------------------------------------------------------
int NpcHead::GetId(void) const
{
	return id;
}
//-----------------------------------------------------------------------------
void NpcHead::IncTime(void)
{
	time++;
	return;
}
//-----------------------------------------------------------------------------
short NpcHead::GetTime(void) const
{
	return time;
}
//-----------------------------------------------------------------------------
void NpcHead::SetTime(short t)
{
	time = t;
	return;
}
//-----------------------------------------------------------------------------
unsigned char NpcHead::GetWave(void) const
{
	return wave;
}
//-----------------------------------------------------------------------------
void NpcHead::NpcSpawn(void) //spawns random npc if possible
{
    int temp;
    IncTime();

    if (((GetTime() >= (20 + GetLevel())) && (GetCount() < NPC_MAX)) 
		|| !(GetCount()))//(GetCount() < GetEnemy()) && 
    {
        if (GetWave() == 0 && !(GetCount()))
        {
            Add(100, 340.0, 120.0);
            return;
        }

        switch(RandNum(12))
        {
            case 0://zoomer
                temp = RandNum(3);

                if (GetEnemyCount(E_ZOOMER) < 3 && (RandNum(6) == 4))
                {
                    if (temp == 0)
                        Add(0, 180.0 + RandNum(120), -10.0);
                    else if (temp == 1)
                        Add(0, 180.0 + RandNum(120), 250.0);
                    else
                        Add(0, (SCREEN_WIDTH + 5.0), (10 + RandNum(200)));
                }
            break;
            case 1://part time
                temp = RandNum(3);

                if (GetEnemyCount(E_PARTTIME) < 2 && (RandNum(10) == 6))
                {
                    if (temp == 0)
                        Add(1, 180.0 + RandNum(120), -10.0);
                    else if (temp == 1)
                        Add(1, 180.0 + RandNum(120), 250.0);
                    else
                        Add(1, 325.0, (RandNum(220) + 10.0));
                }
            break;
            case 2://big ship
                if (GetEnemyCount(E_BIG) < 2 && GetLevel() >= 1 && (RandNum(20) == 8))
                {
                    if (!(GetEnemyCount(E_BIG)))
                        Add(2, (SCREEN_WIDTH + 25.0), (50.0 + RandNum(150)));
                    else if (GetLevel() >= 5)
                        Add(2, (SCREEN_WIDTH + 25.0), (130.0 + RandNum(50)));
                    break;
                }
            case 3://brick set
                temp = RandNum(140);
                if (!(GetEnemyCount(E_BIG)) && (GetLevel() >= 3) && (RandNum(20) == 8) 
					&& (GetEnemyCount(E_BRICK) < 2))
                {
                    Add(3, (SCREEN_WIDTH + 15.0), (50.0 + temp));
                    Add(4, (SCREEN_WIDTH + 43.0), (50.0 + temp));
                    break;
                }
            case 4://mine set
                if (GetLevel() >= 7 && (GetEnemyCount(E_MINE) < 1) && (RandNum(50) == 8))
                {
                    Add(5, (SCREEN_WIDTH + 15.0), (10.0 + RandNum(170)));

                    break;
                }
            case 5://lone brick
                /*if (!(GetEnemyCount(E_BIG)) && RandNum(20) == 3 && (GetLevel() >= 2) &&
                (GetEnemyCount(E_BRICK) < 2) && (GetEnemy() > 5)) //no big ships so spawn ok
                    Add(3, (SCREEN_WIDTH + 21.0), (50.0 + RandNum(130)));*/
            break;
            case 6://formation set
                if ((RandNum(30) == 8) && (GetCount() < (GetEnemy() - 4))
                && (GetCount() < (NPC_MAX - 4)) && GetEnemyCount(E_FORM) < 1)
                {
                    temp = RandNum(141);

                    Add(6, (SCREEN_WIDTH + 15.0), (40.0 + temp));
                    Add(6, (SCREEN_WIDTH + 45.0), (40.0 + temp));
                    Add(6, (SCREEN_WIDTH + 75.0), (40.0 + temp));
                    Add(6, (SCREEN_WIDTH + 105.0), (40.0 + temp));

                }
            break;
            case 7://square wave
                if (GetEnemyCount(E_SQUARE) < 2  && GetEnemyCount(E_ZIG) < 2 && (RandNum(40) == 6))
                    Add(7, (SCREEN_WIDTH + 15.0), (60.0 + RandNum(140)));
            break;
            case 8://bullet wall
                if (GetEnemyCount(E_WALL) < 1 && GetLevel() >= 8 && (RandNum(60) == 6))
                {
                    if (RandNum(2) == 1)
                        Add(8, (420.0 + RandNum(100)), 0.0);
                    else
                        Add(8, (420.0 + RandNum(100)), 240.0);
                }
            break;
            case 9://zig zag
                if (GetEnemyCount(E_SQUARE) < 2  && GetEnemyCount(E_ZIG) < 2 
					&& (RandNum(40) == 6) && GetLevel() >= 1)
                    Add(9, (SCREEN_WIDTH + 15.0), (60.0 + RandNum(140)));
            break;
            case 10://choppas
                if ((RandNum(30) == 8) && (GetCount() < (GetEnemy() - 3))
                && (GetCount() < NPC_MAX - 3) && GetEnemyCount(E_CHOPPA) < 1)
                {
                    temp = RandNum(141);

                    Add(10, (SCREEN_WIDTH + 15.0), (40.0 + temp));
                    Add(10, (SCREEN_WIDTH + 85.0), (40.0 + temp));
                    Add(10, (SCREEN_WIDTH + 170.0), (40.0 + temp));
                }
            break;
            case 11://diamonds
                if ((RandNum(30) == 8) && GetEnemyCount(E_DIAMOND) < 1)
                {
                    if (RandNum(100) >= 49)
                        Add(11, (SCREEN_WIDTH + 15.0), 200.0);
                    else
                        Add(11, (SCREEN_WIDTH + 15.0), 40.0);
                }
            break;
            default:;
            break;
        }
    }

    if ((GetTime() >= (50 + GetLevel())) || GetTime() < 0)
        SetTime(0);

    return;
}
//-----------------------------------------------------------------------------
void NpcHead::DrawNpcs(SURFACES* s, Camera& cam, bool big)
{
	if (!n_list.size())
		return;
	else
	{
		std::list<Npc>::iterator i;
		for(i = n_list.begin(); i != n_list.end(); ++i)
			i->DrawNpc(big, s, static_cast<int>(cam.GetX()), static_cast<int>(cam.GetY()));
	}

	return;
}
//=============================================================================
//Object Manager for effects class
//=============================================================================
ObjManFx::ObjManFx()
{
    return;
}
//-----------------------------------------------------------------------------
ObjManFx::~ObjManFx()
{
    if (!(vfx.empty()))
        vfx.clear();

    if (!(proj.empty()))
        proj.clear();

    return;
}
//-----------------------------------------------------------------------------
void ObjManFx::AddVfx(int id, int n_id, double x, double y, int angle)
{
    OBJ_DATA d = {id, n_id, x, y, angle};

    vfx.push_front(d);

    //std::cout << vfx.size() << std::endl;

    return;
}
//-----------------------------------------------------------------------------
void ObjManFx::AddProj(int id, int n_id, double x, double y, int angle, double speed)
{
    OBJ_DATA d = {id, n_id, x, y, angle, speed};

    proj.push_front(d);

    return;
}
//-----------------------------------------------------------------------------
void ObjManFx::ClearVfx(void)
{
    vfx.clear();
    return;
}
//-----------------------------------------------------------------------------
void ObjManFx::ClearProj(void)
{
    proj.clear();
    return;
}
//-----------------------------------------------------------------------------
int ObjManFx::SizeProj(void)
{
    return proj.size();
}
//-----------------------------------------------------------------------------
int ObjManFx::SizeVfx(void)
{
    return vfx.size();
}
//-----------------------------------------------------------------------------
std::list<OBJ_DATA>::iterator ObjManFx::ProjIt(bool back)
{
	if (!back)
		return proj.begin();
	else
		return proj.end();
}
//-----------------------------------------------------------------------------
std::list<OBJ_DATA>::iterator ObjManFx::VfxIt(bool back)
{
	if (!back)
		return vfx.begin();
	else
		return vfx.end();
}
//-----------------------------------------------------------------------------
void ProcessProj(ObjManFx &om, ProjectileHead &ph)
{
	if (!(om.SizeProj()))//nothing to process
		return;
	else
	{
		std::list<OBJ_DATA>::iterator i;
		for(i = om.ProjIt(false); i != om.ProjIt(true); ++i)
			ph.Add(i->id, i->nid, i->x, i->y, static_cast<int>(i->angle), i->speed);

		om.ClearProj();
	}

	return;
}

//-----------------------------------------------------------------------------
void ProcessVfx(ObjManFx &om, VfxHead &vh)
{
	if (!(om.SizeVfx()))//nothing to process
		return;
	else
	{
		std::list<OBJ_DATA>::iterator i;
		for(i = om.VfxIt(false); i != om.VfxIt(true); ++i)
			vh.Add(i->id, i->nid, i->x, i->y, static_cast<int>(i->angle));

		om.ClearVfx();
	}

	return;
}

