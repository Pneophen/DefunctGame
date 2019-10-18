#include "common.h"

//-----------------------------------------------------------------------------
void PlaySound(AUDIO &a, unsigned int type, unsigned int index, int loops)
{
    if (index > (MAX_SOUNDS-1))
        index = (MAX_SOUNDS-1);

    if (Mix_Playing(-1) == 32)//all channels used so don't bother
        return;

    switch(type)
    {
        case 0:
            if (Mix_PlayChannel(-1, (a.common[index].s), loops) == -1)
            {
                LogIt("Could not play sound");
                LogIt(Mix_GetError());
            }
        break;
        default:;
    }

    return;
}
//-----------------------------------------------------------------------------
void PlayBgm(AUDIO &a, unsigned int index)
{
    if (index > (MAX_BGM-1))
        index = (MAX_BGM-1);

    if (Mix_PlayMusic((a.bgm[index].m), -1) == -1)
    {
        LogIt("Could not play music");
        LogIt(Mix_GetError());
    }

    return;
}
//-----------------------------------------------------------------------------
void LoopBgm(AUDIO &a)
{//gah, I have to do this manually
    Mix_SetMusicPosition((a.bgm[(a.bgm_index)].loop_pos));

    return;
}
//-----------------------------------------------------------------------------
void CleanUpSnd(AUDIO &a)
{
    int i;

    for(i = 0; i < MAX_SOUNDS; i++)
    {
        if (a.common[i].s != NULL)
            Mix_FreeChunk((a.common[i].s));
    }

    return;
}
//-----------------------------------------------------------------------------
void CleanUpBgm(AUDIO &a)
{
    int i;

    for(i = 0; i < MAX_BGM; i++)
    {
        if (a.bgm[i].m != NULL)
            Mix_FreeMusic((a.bgm[i].m));
    }

    return;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------




