#include "common.h"
#include "objects.h"

extern ScreenOffset scr_off;
extern int debug;

void PlaySound(AUDIO &a, unsigned int type, unsigned int index, int loops);
int NpcLifeSet(int life, int level);
double NpcShotSet(double spd, int level);
//-----------------------------------------------------------------------------
void Npc::Zoomer(Player &p, ObjManFx &om, AUDIO &a)
{
    int i;
    int b_type = 10;

    if (level >= 6)
        b_type = ((RandNum(5) == 3) ? 14 : 10);

    switch(state_no)//zoomer
    {
        case -1:
            muteki = 2;
            if (x > SCREEN_WIDTH)
                vx = -6.0;

            if (y < 0)
                vy = 6.0;

            if (y > 240)
                vy = -6.0;

			if ((x <= (SCREEN_WIDTH - 5)) && (y >= 5) && (y <= 235))
                SetStateNo(0, true);
        break;
        case 0:
            vx = 0.0;
            vy = 0.0;
        break;
        case 20://move somewhere
            if (time == 0)
            {
                if (x <= 195)
                    ex[0] = RandNum(90);
                else
                    ex[0] = (RandNum(90)+90);

                if (y <= 120)
                    ex[0] = (360 - ex[0]);

                SetVel(ex[0], 7.0);
            }

            if (time == 10)
                vx = vy = 0.0;

            if (x <= 130 || x >= 310)
            {
                x += ((x <= 130) ? 2 : -2);
                vx = vy = 0.0;
            }
            if (y <= 10 || y >= 230)
            {
                y += ((y <= 10) ? 2 : -2);
                vx = vy = 0.0;
            }

            if (time >= 30)
                SetStateNo(0, true);
        break;
        case 200://shoot at player
            if (time == 0 && (p.GetX() < (x - 10.0)))
            {
                om.AddProj(b_type, (n_id), (x - 12.0), y,
                MathGetAngle((x - (p.GetX() + 10.0)), (y - p.GetY())), 0.5);
            }

            if (time >= 5)
                SetStateNo(0, true);
        break;
        case 201://shoot at player(spread)
            if (time == 0 && (p.GetX() < (x - 10)))
            {
                for(i = 0; i < 60; i += 12)
                    om.AddProj(11, n_id, (x - 12.0), y, (156 + i),
                    NpcShotSet(0.5, level));
            }

            if (time >= 30)
                SetStateNo(0, true);
        break;
        case 202://shoot at player(rapid)
            if (time <= 60 && !(GetIntTime() % 10))
            {
                om.AddProj(b_type, n_id, (x - 12), y,
                ((MathGetAngle((x - (p.GetX() + 10)), (y - p.GetY())) + 5) - RandNum(10)), 0.5);
            }

            if (time >= 100)
                SetStateNo(0, true);
        break;
        case 5900://died
            if (RandNum(2) == 1)
                PlaySound(a, SND_COMMON, 1, 0);
            else
                PlaySound(a, SND_COMMON, 10, 0);

            om.AddVfx(3, n_id, x, y, 0);
            alive = false;
            visible = false;

            if (hard)
            {
                for(i = 0; i < 4; ++i)
                {
					om.AddProj(11, n_id, x, y,(135 + RandNum(90)), static_cast<double>(((0.5 + RandNum(2))/2.0)));
                }
            }
        break;
        default:;
    }

    if (move_type == 'h')//show flash frame
    {
        if (anim.frame == 0)
            anim.frame = 1;
        else
        {
            anim.frame = 0;
            move_type = 'n';
        }
    }

    return;
}
//-----------------------------------------------------------------------------
void Npc::PartTime(Player &p, ObjManFx &om, AUDIO &a)
{
   
    switch(state_no)
    {
        case -1://decide where to move
			if (x >  SCREEN_WIDTH)
                vx = -5.0;
            else if (y < 0.0)
                vy = 3.0;
            else if (y > 240.0)
                vy = -3.0;

            ex[0] = 0;

            if (time >= 30)
                SetStateNo(1, true);
        break;
        case 1://move a bit shoot then exit
            if (time == 0)
                vx = vy = 0.0;

            if (time == 59 && RandNum(100) >= 50 && level >= 2)
                SetStateNo(3, true);
            else if (time == 60)
            {
                if (RandNum(100) >= 50)
                {
                    om.AddProj(10, n_id, (x - 8.0), (y + 7.0),
                    MathGetAngle((x - (p.GetX() + 10)), (y - p.GetY())), 0.5);
                }
                else
                {
                    for(int i = 0; i < 60; i += 12)
						om.AddProj(10, n_id, (x - 8.0), (y + 7.0), (156 + i),
						NpcShotSet(0.5, level));
                }
            }

            if (time >= 70)
                SetStateNo(2, true);

        break;
        case 2://exit then remove self
            if (time <= 0)
                ex[0] = ((y >= 121.0) ? -1 : 1);

            if (speed < 2.0)
                vy += ((ex[0] == -1) ? -0.05 : 0.05);

            if (y < -15 || y > 255)
            {
                alive = false;
                visible = false;
            }
        break;
        case 3:// shoot zig zag
            if (time == 80)
				for (int i = 0; i < 90; i += 9)
					om.AddProj(11, n_id, (x - 8.0), (y + 7.0), (135 + i),
					NpcShotSet(0.5 + (RandNum(3) / 2.0), level));

            if (time >= 90)
                SetStateNo(2, true);
        break;
        case 5900://died
            if (RandNum(2) == 1)
                PlaySound(a, SND_COMMON, 1, 0);
            else
                PlaySound(a, SND_COMMON, 10, 0);

            om.AddVfx(3, n_id, x, y, 0);
            alive = false;
            visible = false;

            if (hard)
            {
                for(int i = 0; i < 4; ++i)
                {
					om.AddProj(11, n_id, x, y,
					(135 + RandNum(90)), static_cast<double>(((0.5 + RandNum(2))/2.0)));
                }
            }
        break;
        default:;
    }

    if (move_type == 'h')//show flash frame
    {
        if (anim.frame == 0)
            anim.frame = 1;
        else
        {
            anim.frame = 0;
            move_type = 'n';
        }
    }

	if (anim.frame == 2 && anim.anim_time == 1)
	{
		anim.frame = 0;
		anim.anim_time = 0;
	}

    return;
}
//-----------------------------------------------------------------------------
void Npc::BigShip(Player &p, ObjManFx &om, AUDIO &a)
{
    int b_type = 10;

    if (level >= 6)
        b_type = ((RandNum(5) == 3) ? 14 : 10);

    switch(state_no)
    {
        case -1://decide where to move
            anim.frame = 0;
            move_type = 'n';

            if (time == 0)
            {
				muteki = 5;

				if (x > SCREEN_WIDTH)
                    vx = -2.0;

                if (y < 0)
                    vy = 2.0;

                if (y > 240)
                    vy = -2.0;
            }

            /*if (time == 1)
            {
                fric = 1.0;
                grav = 1.0;
            }*/

			if ((x <= (SCREEN_WIDTH - 50)) && (y >= 1) && (y <= 239))
            {
                fric = 0.0;
                grav = 0.0;
                SetStateNo(0, true);
            }
        break;
        case 0://decide where to move
            vx = vy = 0.0;
        break;
        case 20://move somewhere
            if (time == 0)
            {
                if (x <= (SCREEN_WIDTH - 50))
                    ex[0] = RandNum(180);
                else
                    ex[0] = (RandNum(180)+90);

                if (y <= 120)
                    ex[0] = (360 - ex[0]);

                SetVel(ex[0], 1.0);
            }

            if (x <= 270 || x >= (SCREEN_WIDTH - 50))
            {
                x += ((x <= 270) ? 2.0 : -2.0);
                vx *= -1.0; 
				vy = 0;
            }
            if (y <= 50 || y >= 190)
            {
                y += ((y <= 50) ? 2.0 : -2.0);
                vx = vy = 0;
            }

            if (y > 65.0 && ex[4] > 1 && ex[3] == 0 && vy > 0.0)
                vy *= -1.0;// there are 2 ships so stay on top


            if (y < 165.0 && ex[4] > 1 && ex[3] == 1 && vy < 0.0)
                vy *= -1.0;// there are 2 ships so stay on bottom

            if (time >= 30)
                SetStateNo(0, true);
        break;
        case 200://spread shots
            if (time <= 0)
                vx = vy = 0.0;
            if (time == 15)
            {
                for(int i = 0; i < 60; i += 12)
                {
                    om.AddProj(b_type, n_id, (x - 21.0), (y - 23.0), (150 + i),
                    NpcShotSet(0.5, level));
                    om.AddProj(b_type, n_id, (x - 21.0), (y + 23.0), (150 + i),
                    NpcShotSet(0.5, level));
                }

            }

            if (time == 45)
            {
                for(int i = 0; i < 60; i += 12)
                {
                    om.AddProj(b_type, n_id, (x - 11.0), (y - 40.0), (150 + i),
                    NpcShotSet(0.5, level));
                    om.AddProj(b_type, n_id, (x - 11), (y + 40.0), (150 + i),
                    NpcShotSet(0.5, level));
                }
            }

            if (time >= 60)
                SetStateNo(0, true);
        break;
        case 201://shot blanket
            if (time <= 0)
                vx = vy = 0.0;

            if (time <= 0 || time == 35)
                ex[0] = 0;

            if (!(GetIntTime() % 2) && time <= 34)
            {
                om.AddProj(10, n_id, (x - 11.0), (y - 40.0), (95 + ex[0]),
                NpcShotSet(0.5, level));
                om.AddProj(10, n_id, (x - 11.0), (y + 40.0), (265 - ex[0]),
                NpcShotSet(0.5, level));
                ex[0] += 10;
            }

            if (!(GetIntTime() % 2) && time <= 83 && time >= 49)
            {
                om.AddProj(b_type, n_id, (x - 21.0), (y - 23.0), (65 + ex[0]),
                NpcShotSet(0.5, level));
                om.AddProj(b_type, n_id, (x - 21.0), (y + 23.0), (295 - ex[0]),
                NpcShotSet(0.5, level));
                ex[0] += 10;
            }

            if (time >= 100)
                SetStateNo(0, true);
        break;
        case 202://big beam time well not really a beam anymore
            if (time == 0)
            {
                vx = vy = 0;
                om.AddVfx(4, n_id, (x - 25.0), y, 0);
                PlaySound(a, SND_COMMON, 27, 0);
            }

            if (time == 20)
            {
                PlaySound(a, SND_COMMON, 26, 0);
                om.AddProj(13, n_id, (x - 25.0), y, 180, 20.0);
            }

            if (time == 45)
                SetStateNo(0, true);
        break;
        case 203://shoot at player
            if (time <= 0)
                vx = vy = 0.0;
            if (time == 10)
            {
                om.AddProj(b_type, n_id, (x - 21.0), (y - 23.0),
                MathGetAngle(((x - 21.0) - (p.GetX() + 10.0)), ((y - 23.0) - p.GetY())),
                0.5);
                om.AddProj(b_type, n_id, (x - 21.0), (y + 23.0),
                MathGetAngle(((x - 21.0) - (p.GetX() + 10.0)), ((y + 23.0) - p.GetY())),
                0.5);
                om.AddProj(b_type, n_id, (x - 11.0), (y - 40.0),
                MathGetAngle(((x - 11.0) - (p.GetX() + 10.0)), ((y - 40.0) - p.GetY())),
                0.5);
                om.AddProj(b_type, n_id, (x - 11.0), (y + 40.0),
                MathGetAngle(((x - 11.0) -( p.GetX() + 10.0)), ((y + 40.0) - p.GetY())),
                0.5);
            }

            if (time == 50)
                SetStateNo(0, true);
        break;
        case 5900://died
            //if (alive)
                //VfxKill(v, 4, (n_id));

			if (hard)
            {
                for(int i = 0; i < 10; ++i)
                {
					om.AddProj(11, n_id, x, y,
					(135 + RandNum(90)), static_cast<double>(((0.5 + RandNum(2))/2.0)));
                }
            }

            PlaySound(a, SND_COMMON, 9, 0);
            for(int i = 0; i < 8; ++i)
                om.AddVfx(3, n_id, ((x - 15.0) + static_cast<double>(RandNum(30))),
                ((y - 23.0) + static_cast<double>(RandNum(45))), 0);
            alive = false;
            visible = false;

            if (hard)
            {
                for(int i = 0; i < 4; ++i)
                {
					om.AddProj(11, n_id, x, y,
					(135 + RandNum(90)), ((0.5 + RandNum(2))/2));
                }
            }
        break;
        default:;
    }

    if (move_type == 'h')//show flash frame
    {
        if (anim.frame == 0)
            anim.frame = 1;
        else
        {
            anim.frame = 0;
            move_type = 'n';
        }
    }

    if (ex[4] == 1 && ex[3] == 1)
        ex[3] = 0; //is only single ship so up/dwn movement not restricted

    return;
}
//-----------------------------------------------------------------------------
void Npc::Brick(Player &p, ObjManFx &om, AUDIO &a)
{//indestructable brick object

    life = 9999;

    switch(state_no)
    {
        case -1://go foward
            if (x > (SCREEN_WIDTH + 19.0))
				vx = -1.0;
			else
				vx = -0.5;
            SetStateNo(0, false);
        break;
        case 0:
            if (x <= 150)
                vx -= 0.5;

            if (x < -15.0)
            {
                alive = false;
                visible = false;
            }
        break;
        case 5900://died
            PlaySound(a, SND_COMMON, 1, 0);
            om.AddVfx(3, n_id, x, y, 0);
            alive = false;
            visible = false;
        break;
        default:;
        break;
    }

    return;
}
//-----------------------------------------------------------------------------
void Npc::Leech(Player &p, ObjManFx &om, AUDIO &a)
{//hides behind brick
    
    switch(state_no)
    {
        case -1://go foward
            vx = -0.5;
            SetStateNo(0, true);
        break;
        case 0://go till in position
            if (x <= 180.0 || (MathGetDist(x, p.GetX()) <= 10))
                SetStateNo(1, true);
        break;
        case 1://move foward phase
            if (time == 1.0)
                vx = 0;

            if (time == 3.0)
            {
                if (y >= 120)
                    vy = -1.0;
                else
                    vy = 1.0;
            }

            if (time >= 40.0)
            {
                vy = 0.0;
                vx -= 0.1;
                if (!(GetIntTime() % 6) && (time >= 35))
                {
                    if (y >= p.GetY())
                        om.AddProj(11, (n_id), x - 4, y - 7, 90, 0.5);
                    else
                        om.AddProj(11, (n_id), x - 4, y + 7, 270, 0.5);
                }

            }


            if (x < -15.0) //offscreen so it is dead
            {
                alive = false;
                visible = false;
            }
        break;
        case 5900://died
            if (hard)
            {
                for(int i = 0; i < 4; ++i)
                {
                    om.AddProj(11, (n_id), x, y,
                    (135 + RandNum(90)), static_cast<double>(((0.5 + RandNum(2))/2.0)));
                }
            }

            if (RandNum(2) == 1)
                PlaySound(a, SND_COMMON, 1, 0);
            else
                PlaySound(a, SND_COMMON, 10, 0);

            om.AddVfx(3, (n_id), x, y, 0);
            alive = false;
            visible = false;
        break;
        default:;
        break;
    }

    return;
}
//-----------------------------------------------------------------------------
void Npc::Mine(Player &p, ObjManFx &om, AUDIO &a)
{
    switch(state_no)
    {
        case -1://go foward
            vx = -0.8;
            muteki = 2;

            if (x < 320)
                SetStateNo(0, false);
        break;
        case 0: //move up down
            if (y < p.GetY())
                vy += 0.004;
            else if (y > p.GetY())
                vy -= 0.004;

            if ((x <= 160) || ((MathGetDist(x, p.GetX()) <= 70)
                && (MathGetDist(y, p.GetY()) <= 80)))
                SetStateNo(1, false);
        break;
        case 1: //self destruct
            for(int i = 0; i < 40; ++i)
            {
                om.AddProj(11, n_id, x, y, (i * 9), static_cast<double>((0.5 + RandNum(3))/2.0));
            }

            if (RandNum(2) == 1)
                PlaySound(a, SND_COMMON, 1, 0);
            else
                PlaySound(a, SND_COMMON, 10, 0);

            om.AddVfx(3, n_id, x, y, 0);
            alive = false;
            visible = false;
        break;
        case 5900://died

            if (RandNum(2) == 1)
                PlaySound(a, SND_COMMON, 1, 0);
            else
                PlaySound(a, SND_COMMON, 10, 0);

            om.AddVfx(3, n_id, x, y, 0);
            alive = false;
            visible = false;

            if (hard)
            {
                for(int i = 0; i < 4; ++i)
                {
                    om.AddProj(11, n_id, x, y, (135 + RandNum(90)), static_cast<double>(((0.5 + RandNum(2))/2.0)));
                }
            }
        break;
        default:;
        break;
    }

    if (move_type == 'h')//show flash frame
    {
        if (anim.frame == 0)
            anim.frame = 1;
        else
        {
            anim.frame = 0;
            move_type = 'n';
        }
    }

    if (anim.frame == 3 && anim.anim_time == 6)
        anim.frame = 0;

    return;
}
//-----------------------------------------------------------------------------
void Npc::Formation(Player &p, ObjManFx &om, AUDIO &a)
{
    switch(state_no)
    {
        case -1://go foward
            vx = -1.5;
            muteki = 5;

            if (y > 120)
                ex[0] = 1;
            else
                ex[0] = -1;

			//ex[1] = 0;

            if (x < 320)
                SetStateNo(0, false);
        break;
        case 0:

            if (ex[0] == 1)
                ex[1] -= 1;
            else
                ex[1] += 1;

            if (ex[1] <= -30)
                ex[0] = -1;
            else if (ex[1] >= 30)
                ex[0] = 1;

            
			SetVel((180 + ex[1]), 1.5);

            if (time == 60 && RandNum(2) == 1)
            {
                om.AddProj(11, n_id, (x - 8.0), y,
                MathGetAngle((x - (p.GetX() + 10.0)), (y - p.GetY())), 0.5);

                if (level >= 3)
                {
                    om.AddProj(11, n_id, (x - 8.0), y,
                    (MathGetAngle((x - (p.GetX() + 10.0)), (y - p.GetY())) - 10), 0.5);
                    om.AddProj(11, n_id, (x - 8.0), y,
                    (MathGetAngle((x - (p.GetX() + 10.0)), (y - p.GetY())) + 10), 0.5);
                }
            }

            if (x < -15.0)
            {
                alive = false;
                visible = false;
            }

        break;
        case 5900://died
            if (hard)
            {
                for(int i = 0; i < 4; ++i)
                {
                    om.AddProj(11, (n_id), x, y,
                    (135 + RandNum(90)), static_cast<double>(((0.5 + RandNum(2))/2.0)));
                }
            }
            PlaySound(a, SND_COMMON, 1, 0);
            om.AddVfx(3, n_id, x, y, 0);
            alive = false;
            visible = false;
        break;
        default:;
        break;
    }

    if (move_type == 'h')//show flash frame
    {
        if (anim.frame == 0)
            anim.frame = 1;
        else
        {
            anim.frame = 0;
            move_type = 'n';
        }
    }

    if (anim.frame == 3 && anim.anim_time == 6)
    {
        anim.frame = 0;
        anim.anim_time = 0;
    }

    return;
}
//-----------------------------------------------------------------------------
void Npc::SquareWave(Player &p, ObjManFx &om, AUDIO &a)
{
    switch(state_no)
    {
        case -1://go foward
            vx = -10.0;
            muteki = 2;

            ex[0] = ((y < 120.0) ? 0 : 1);

            if (x < 320.0)
            {
                vx = 0.0;
                SetStateNo(0, false);
            }
        break;
        case 0: //forward
            if (time == 0)
                vx = -10.0;

            if (time == 5)
                vx = 0.0;

            if (time == 20)
            {
                if (ex[0] == 0)
                {
                    ex[0] = 1;
                    SetStateNo(10, false);
                }
                else
                {
                    ex[0] = 0;
                    SetStateNo(11, false);
                }
            }

            if (x < -15)
            {
                alive = false;
                visible = false;
            }
        break;
        case 10: //down
            if (time == 0)
                vy = 10.0;

            if (time == 5)
                vy = 0.0;

            if (time == 20)
                SetStateNo(0, false);
        break;
        case 11: //up
            if (time == 0)
                vy = -10.0;

            if (time == 5)
                vy = 0.0;

            if (time == 20)
                SetStateNo(0, false);
        break;
        case 5900://died
            if (hard)
            {
                for(int i = 0; i < 4; ++i)
                {
                    om.AddProj(11, n_id, x, y,
                     (135 + RandNum(90)), static_cast<double>(((0.5 + RandNum(2))/2.0)));
                }
            }

            PlaySound(a, SND_COMMON, 1, 0);
            om.AddVfx(3, n_id, x, y, 0);
            alive = false;
            visible = false;
        break;
        default:;
        break;
    }

    if (move_type == 'h')//show flash frame
    {
        if (anim.frame == 0)
            anim.frame = 1;
        else
        {
            anim.frame = 0;
            move_type = 'n';
        }
    }

    if (anim.frame == 1 && anim.anim_time == 1)
    {
        anim.frame = 0;
        anim.anim_time = 0;
    }

    return;
}
//-----------------------------------------------------------------------------
void Npc::Wall(Player &p, ObjManFx &om, AUDIO &a)
{
    int b_type = 14;

    b_type = ((RandNum(100) <= 50) ? 10 : 14);

    switch(state_no)
    {
        case -1://go foward
            SetStateNo(0, false);
        break;
        case 0:
            if (y < 120)
				SetVel((225 + RandNum(91)), 6.0);
            else
				SetVel((45 + RandNum(91)), 6.0);

            for(int i = 0; i < 10; ++i)
                om.AddVfx(13, -2, ((x + 5.0) - static_cast<double>(RandNum(10))), 
				((y + 5.0) - static_cast<double>(RandNum(10))), 0);

            om.AddProj(b_type, n_id, x, y, 180, 1.0);

            if (time >= 20)
                alive = false;
        break;
        case 5900://died
            PlaySound(a, SND_COMMON, 1, 0);
            om.AddVfx(3, n_id, x, y, 0);
            alive = false;
            visible = false;
        break;
        default:;
        break;
    }

    return;
}
//-----------------------------------------------------------------------------
void Npc::ZigZag(Player &p, ObjManFx &om, AUDIO &a)
{
    switch(state_no)
    {
        case -1://go foward

            if (time == 0)
                vx = -10.0;

            muteki = 2;

            ex[0] = ((y < 120.0) ? 0 : 1);

            if (x < 320.0)
            {
                vx = 0.0;
                SetStateNo(0, false);
            }
        break;
        case 0: //hold
            if (time == 0)
                vx = 0.0;

            if (time == 20)
            {
                if (ex[0] == 0)
                {
                    ex[0] = 1;
                    SetStateNo(10, false);
                }
                else
                {
                    ex[0] = 0;
                    SetStateNo(11, false);
                }
            }

            if (x < -15.0)
            {
                alive = false;
                visible = false;
            }
        break;
        case 10: //down
            if (time == 0)
                SetVel(225, 12.0);

            if (time == 5)
                vx = vy = 0.0;

            if (time == 20)
                SetStateNo(0, false);
        break;
        case 11: //up
            if (time == 0)
				SetVel(135, 12.0);

            if (time == 5)
                vx = vy = 0.0;

            if (time == 20)
                SetStateNo(0, false);
        break;
        case 5900://died
            if (hard)
            {
                for(int i = 0; i < 4; ++i)
                {
                    om.AddProj(11, n_id, x, y,
                    (135 + RandNum(90)), static_cast<double>(((0.5 + RandNum(2))/2.0)));
                }
            }

            PlaySound(a, SND_COMMON, 1, 0);
            om.AddVfx(3, n_id, x, y, 0);
            alive = false;
            visible = false;
        break;
        default:;
        break;
    }

    if (move_type == 'h')//show flash frame
    {
        if (anim.frame == 0)
            anim.frame = 1;
        else
        {
            anim.frame = 0;
            move_type = 'n';
        }
    }

    if (anim.frame == 7 && anim.anim_time == 1)
    {
        anim.frame = 0;
        anim.anim_time = 0;
    }

    return;
}
//-----------------------------------------------------------------------------
void Npc::Choppa(Player &p, ObjManFx &om, AUDIO &a)
{
    int angle;
    int b_type = ((level >= 6) ? 14 : 10);

    switch(state_no)
    {
        case -1://go foward
            vx = -1.5;
            muteki = 2;

            if (y > 120)
                ex[0] = -1;
            else
                ex[0] = 1;

            ex[1] = 0;

            if (x < 320)
                SetStateNo(0, false);
        break;
        case 0:

            if (time >= 70 && time <= 90)
                ex[1] += ((ex[0] == 1) ? 2 : -2);


            SetVel((180 + ex[1]), 1.0);

            if (time == 75)
            {
                angle = MathGetAngle((x - (p.GetX() + 10.0)), (y - p.GetY()));

                om.AddProj(b_type, n_id, (x - 10.0), (y), (angle - 20), 0.5);
                om.AddProj(b_type, n_id, (x - 10.0), (y), (angle - 10), 0.5);
                om.AddProj(b_type, n_id, (x - 10.0), (y), angle, 1.0);
                om.AddProj(b_type, n_id, (x - 10.0), (y), (angle + 10), 0.5);
                om.AddProj(b_type, n_id, (x - 10.0), (y), (angle + 20), 0.5);
            }

            if (y < -10 || y > 250)
            {
                alive = false;
                visible = false;
            }

        break;
        case 5900://died
            if (hard)
            {
                for(int i = 0; i < 4; ++i)
                {
                    om.AddProj(11, (n_id), x, y,
                    (135 + RandNum(90)), static_cast<double>(((0.5 + RandNum(2))/2.0)));
                }
            }
            PlaySound(a, SND_COMMON, 1, 0);
            om.AddVfx(3, n_id, x, y, 0);
            alive = false;
            visible = false;
        break;
        default:;
        break;
    }

    if (move_type == 'h')//show flash frame
    {
        if (anim.frame == 0)
            anim.frame = 1;
        else
        {
            anim.frame = 0;
            move_type = 'n';
        }
    }

    /*if (anim.frame == 2 && anim.anim_time == 1)
    {
        anim.frame = 0;
        anim.anim_time = 0;
    }*/

    return;
}
//-----------------------------------------------------------------------------
void Npc::Diamond(Player &p, ObjManFx &om, AUDIO &a)
{
    int angle;
    int b_type = ((level >= 6) ? 14 : 10);

    switch(state_no)
    {
        case -1://go foward
            vx = -10.0;
            muteki = 2;

            if (y > 120.0)
                ex[0] = -1;
            else
                ex[0] = 1;

            ex[1] = 0;

            if (x <= 310.0)
            {
                vx = 0.0;
                SetStateNo(0, false);
            }
        break;
        case 0:

            if (time >= 70 && time <= 159)
            {
                ex[1] += ((ex[0] == 1) ? 2 : -2);
				SetVel((180 + ex[1]), 3.0);
            }
            else
				SetVel((180 + ex[1]), 2.0);

            if (time == 75)
            {
                angle = MathGetAngle((x - (p.GetX())), (y - p.GetY()));

                om.AddProj(b_type, n_id, x, y, angle, 0.5);

                if (level >= 3)
                {
                    om.AddProj(10, n_id, x, y, (angle - 10), 0.5);
                    om.AddProj(10, n_id, x, y, (angle + 10), 0.5);
                }

            }

            if (x > (SCREEN_WIDTH + 5.0))
            {
                alive = false;
                visible = false;
            }

        break;
        case 5900://died
            if (hard)
            {
                for(int i = 0; i < 4; ++i)
                {
                    om.AddProj(11, (n_id), x, y, (135 + RandNum(90)), 
						static_cast<double>(((0.5 + RandNum(2))/2.0)));
                }
            }
            PlaySound(a, SND_COMMON, 1, 0);
            om.AddVfx(3, n_id, x, y, 0);
            alive = false;
            visible = false;
        break;
        default:;
        break;
    }

    if (move_type == 'h')//show flash frame
    {
        if (anim.frame == 0)
            anim.frame = 1;
        else
        {
            anim.frame = 0;
            move_type = 'n';
        }
    }

    /*if (anim.frame == 2 && anim.anim_time == 2)
    {
        anim.frame = 0;
        anim.anim_time = 0;
    }*/

    return;
}
//-----------------------------------------------------------------------------
void Npc::KillScreen(Player &p, ObjManFx &om, AUDIO &a)
{
    switch(state_no)
    {
        case -1://decide where to move
            muteki = 2;
            life = 5000;
            vx = -10.0;

            if ((x <= 280) && (y >= 1) && (y <= 239))
                SetStateNo(0, true);
        break;
        case 0://destruction!!!!!!
            if (time == 0)
                vx = vy = 0.0;

            life = 5000;

            for(int i = 0; i < 3; ++i)
            {
                om.AddProj(14, n_id, (x - 21.0), (y - 23.0), RandNum(360), 2.0);
                om.AddProj(14, n_id, (x - 21.0), (y + 23.0), RandNum(360), 2.0);

                om.AddProj(14, n_id, (x - 11.0), (y - 40.0), RandNum(360), 2.0);
                om.AddProj(14, n_id, (x - 11.0), (y + 40.0), RandNum(360), 2.0);
            }
        break;
        default:;
        break;
    }

    return;
}
//-----------------------------------------------------------------------------
void Npc::PowUp(Player &p, ObjManFx &om, AUDIO &a)
{
    muteki = 1000;
    vx -= 0.06;

    if (x < -15)
    {
        alive = false;
        visible = false;
    }

    if (time == 0)
    {
        int num = RandNum(100);

        if (num >= 90)
        {
            life = 60;
            anim.frame = 2;
        }
        else if (num < 90 && num >= 70)
        {
            life = 30;
            anim.frame = 1;
        }
        else
        {
            life = 15;
            anim.frame = 0;
        }
    }

    if (life == 15)
        anim.frame = ((anim.frame == 0 && !(GetIntTime() % 10)) ? 3 : 0);
    else if (life == 30)
        anim.frame = ((anim.frame == 1  && !(GetIntTime() % 10)) ? 4 : 1);
    else if (life == 60)
        anim.frame = ((anim.frame == 2  && !(GetIntTime() % 10)) ? 5 : 2);

	if (y < p.GetY())
	{
		if (vy < -1.0)
			vy = 0.2;
		vy += 0.02;
	}
    else if (y > p.GetY())
	{
		if (vy > 1.0)
			vy = -0.2;
		vy -= 0.02;
	}



    if (life == 999)
        PlaySound(a, SND_COMMON, 6, 0);

    return;
}


