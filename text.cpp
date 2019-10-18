#include "common.h"
#include "objects.h"

enum FONT_NUM
{
    FONT_A, FONT_B, FONT_C, FONT_D, FONT_E, FONT_F, FONT_G, FONT_H, FONT_I,
    FONT_J, FONT_K, FONT_L, FONT_M, FONT_N, FONT_O, FONT_P, FONT_Q, FONT_R,
    FONT_S, FONT_T, FONT_U, FONT_V, FONT_W, FONT_X, FONT_Y, FONT_Z, FONT_0,
    FONT_1, FONT_2, FONT_3, FONT_4, FONT_5, FONT_6, FONT_7, FONT_8, FONT_9,
    FONT_PERIOD, FONT_APOST, FONT_PLUS, FONT_MINUS, FONT_EQUAL, FONT_BSLASH,
    FONT_FSLASH, FONT_COMMA, FONT_EX, FONT_DOLLAR, FONT_BAR, FONT_PERCENT,
    FONT_QUESTION, FONT_BLANK
};

extern ScreenOffset scr_off;
void SetSpr(int x, int y, SDL_Renderer *view, SDL_Texture *spr, SDL_Rect *rec);
//-----------------------------------------------------------------------------
void SetFont(unsigned short width, unsigned short height, FONT *f)
{
    int i;
    short next = 1;

    for(i = 0; i < 70; i++)
    {
        f->ch[i].y = 0;
        f->ch[i].w = (width);
        f->ch[i].h = (height);
        f->ch[i].x = next;
        next += (width);
    }

    return;
}
//-----------------------------------------------------------------------------
void WriteScreen(int x, int y, char align, int index, FONT *f, SURFACES &s, char str[])
{//writes text on screen
    int ch;
    int count = strlen(str);
    int off_x = 0, off_y = 0;
    int i, j;

    if (!align)
        off_x -= (((count - 1) * ((f->ch[0].w - 1))) >> 1);


    for(i = 0; i < count; i++)
    {
        j = ((align == 1) ? ((count - 1) - i) : i);

        switch(str[j])
        {
            case 'a': ch = FONT_A;break;
            case 'b': ch = FONT_B;break;
            case 'c': ch = FONT_C;break;
            case 'd': ch = FONT_D;break;
            case 'e': ch = FONT_E;break;
            case 'f': ch = FONT_F;break;
            case 'g': ch = FONT_G;break;
            case 'h': ch = FONT_H;break;
            case 'i': ch = FONT_I;break;
            case 'j': ch = FONT_J;break;
            case 'k': ch = FONT_K;break;
            case 'l': ch = FONT_L;break;
            case 'm': ch = FONT_M;break;
            case 'n': ch = FONT_N;break;
            case 'o': ch = FONT_O;break;
            case 'p': ch = FONT_P;break;
            case 'q': ch = FONT_Q;break;
            case 'r': ch = FONT_R;break;
            case 's': ch = FONT_S;break;
            case 't': ch = FONT_T;break;
            case 'u': ch = FONT_U;break;
            case 'v': ch = FONT_V;break;
            case 'w': ch = FONT_W;break;
            case 'x': ch = FONT_X;break;
            case 'y': ch = FONT_Y;break;
            case 'z': ch = FONT_Z;break;

            case '0': ch = FONT_0;break;
            case '1': ch = FONT_1;break;
            case '2': ch = FONT_2;break;
            case '3': ch = FONT_3;break;
            case '4': ch = FONT_4;break;
            case '5': ch = FONT_5;break;
            case '6': ch = FONT_6;break;
            case '7': ch = FONT_7;break;
            case '8': ch = FONT_8;break;
            case '9': ch = FONT_9;break;

            case '*': ch = FONT_APOST;break;//>80 use for apost
            case '.': ch = FONT_PERIOD;break;
            case '+': ch = FONT_PLUS;break;
            case '-': ch = FONT_MINUS;break;
            case '=': ch = FONT_EQUAL;break;
            case '^': ch = FONT_BLANK;//use for newline left align only at moment
            off_x = 0;off_x -= ((f->ch[0].w - 1));
            off_y += f->ch[0].h;
            break;
            case '/': ch = FONT_FSLASH;break;
            case '|': ch = FONT_BAR;break;
            case ',': ch = FONT_COMMA;break;
            case '!': ch = FONT_EX;break;
            case '$': ch = FONT_DOLLAR;break;
            case '?': ch = FONT_QUESTION;break;
            case '%': ch = FONT_PERCENT;break;
            default: ch = FONT_BLANK;break;
        }

        if  (align == -1)
        {
            SetSpr(x+off_x, y+off_y, s.bg, s.font_spr[index].spr, &(f->ch[ch]));
            off_x += ((f->ch[0].w - 1));
        }
        else if  (align == 1)
        {
            SetSpr(x-off_x, y+off_y, s.bg, s.font_spr[index].spr, &(f->ch[ch]));
            off_x += ((f->ch[0].w - 1));
        }
        else
        {
            SetSpr(x+off_x, y+off_y, s.bg, s.font_spr[index].spr, &(f->ch[ch]));
            off_x += ((f->ch[0].w - 1));
        }
    }

    return;
}
//-----------------------------------------------------------------------------
void NumString(int x, int y, char align, int index, FONT *f, SURFACES &s, int num)
{
    char converted[12];

    _itoa(num, converted, 10);
    WriteScreen(x, y, align, index, f, s, converted);//the number is now a string

    return;
}
//-----------------------------------------------------------------------------






