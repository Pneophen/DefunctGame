#include "common.h"

//-----------------------------------------------------------------------------
double AbsF(double num)
{
    if (num > 0.0f)
        return num;
    else
        return (num * -1.0);
}
//-----------------------------------------------------------------------------
int RandNum(int max) //for random numbers
{
    if (max == 0)
        max = 1;

    return ((rand() + time(0)) % max);
}
//-----------------------------------------------------------------------------
double MathGetRad(double ang) // convert angle into radians and return it
{
    ang *= (PI / 180);

    return ang;
}
//-----------------------------------------------------------------------------
void AngleMoveF(double angle, double speed, double &x, double &y)
{
    double rad;

    rad = MathGetRad(angle);

    y = (-(sin(rad)* speed));
    x = (cos(rad) * speed);

    return;
}
//-----------------------------------------------------------------------------
int MathGetDist(int a, int b)
{
    if (a > b)
        return (a - b);
    else
        return (b - a);
}
//-----------------------------------------------------------------------------
int MathGetDist(double a, double b)
{
    int aa = static_cast<int>(a);
	int bb = static_cast<int>(b);
	
	if (aa > bb)
        return (aa - bb);
    else
        return (bb - aa);
}
//-----------------------------------------------------------------------------
double MathGetVSpeed(double vx, double vy)
{
    return ((AbsF(vx) + AbsF(vy)) / 2.0);
}
//-----------------------------------------------------------------------------
int MathGetAngle(double dist_x, double dist_y)
{
    int angle = 0;
    double temp;

    if (!dist_x)
        dist_x = 0.00001; //so there is no division by zero

    if (!dist_y)
        dist_y = 0.00001;

    if ((dist_x > 0.0 && dist_y < 0.0) || (dist_x < 0.0 && dist_y < 0.0))
        angle = 90;
    else if ((dist_x < 0.0 && dist_y > 0.0) || (dist_x > 0.0 && dist_y > 0.0))
        angle = -90;

    temp = (PI + atan(dist_x / dist_y));
    temp *= (180 / PI);
    angle += static_cast<int>(temp);

    return (angle);
}
//-----------------------------------------------------------------------------
int MathGetAngleMove(double dist_x, double dist_y)
{
    int angle = 0;
    double temp;
    bool sub = false;

    if ((dist_x > 0.0 ) && (AbsF(dist_y) < 0.00001))
        return 0;
    else if (dist_x < 0.0 && (AbsF(dist_y) < 0.00001))
        return 180;
    else if (AbsF(dist_x) <= 0.00001f && (dist_y > 0.0))
        return 90;
    else if (AbsF(dist_x) <= 0.00001f && (dist_y < 0.0))
        return 270;

    if (dist_x < 0.0 && dist_y > 0.0)
        angle = 90;

    if (dist_y < 0.0 && dist_x > 0.0)
        angle = 270;

    if (dist_y > 0.0 && dist_x > 0.0)
    {
        angle = 90;
        sub = true;
    }

    if (dist_y < 0.0 && dist_x < 0.0)
    {
        angle = 270;
        sub = true;
    }

    dist_x = AbsF(dist_x);
    dist_y = AbsF(dist_y);

    if (!dist_x)
        dist_x = 0.000001; //so there is no division by zero

    if (!dist_y)
        dist_y = 0.000001;

    temp = atan(dist_x / dist_y);
    temp *= (180 / PI);

    if (!sub)
        angle += static_cast<int>(temp);
    else
        angle -= static_cast<int>(temp);

    return abs(angle);
}
//-----------------------------------------------------------------------------
Uint32 SetRGB(int r, int g, int b){
	return ((r << 16) + (g << 8) + b); // rgb
}

