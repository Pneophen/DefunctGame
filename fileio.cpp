#include "common.h"
#include "objects.h"

extern ScreenOffset scr_off;
//-----------------------------------------------------------------------------
void LoadToMem(FILE *fp, int size, void **v)
{
    *v = malloc(size);//allocate space for data
    fread(*v, size, 1, fp);//transfer data to memory

    return;
}
//-----------------------------------------------------------------------------
bool LumpSearchSpr(int id, CIF_HEAD *ch, int *size, void **v)
{
    LUMP_HEAD lump;
    PADDING padding;
    int i = 0;
    bool found;
    FILE *in = fopen("data/data.pl0", "rb");

    if (in == 0)
    {
        LogIt("Could not open lump");
        return false;
    }

    while(!feof(in))
    {
        found = true;
        i = 0;
        fread(&lump, sizeof(LUMP_HEAD), 1, in);

        if (id != lump.id)
            found = false;


        switch(lump.type)//read next head
        {
            case 0:
                fread(ch, sizeof(CIF_HEAD), 1, in);
            break;
            case 1:
            case 2:
                fread(&padding, sizeof(PADDING), 1, in);
            break;
            default:;
            break;
        }

        if (found)//found name id, get data
        {
            LoadToMem(in, lump.size, v);
            fclose(in);
            *size = lump.size;

            if (v == NULL)
            {
                LogIt("Not enough space to load to memory");
                return false;
            }

            return true;
        }
        else
            fseek(in, lump.size, SEEK_CUR);//go through data to next lump
    }

    fclose(in);

    return false;
}
//-----------------------------------------------------------------------------
bool LoadSprite(int id, SPRITE *final, SDL_Renderer *rend)
{
	//SDL_Texture *temp = 0;
	SDL_Surface *temp2 = 0;
	//SDL_Surface *z_temp = 0;
	CIF_HEAD hd;
	SDL_RWops *rwop = 0;//so sdl can convert to a surface
    int size;
    void *data = 0; //for loading to memory

    //searches for sprite data in lump and loads into memory
	if (!LumpSearchSpr(id, &hd, &size, &data))
    {
        LogIt("Could not load following data");

        char converted[12];
        _itoa(id, converted, 10);//

        LogIt(converted);

        return false;
    }

    rwop = SDL_RWFromMem(data, size); //send data to read/write ops
    temp2 = IMG_LoadPNG_RW(rwop); //load image from memory

    if (temp2 == NULL)
    {
        LogIt("Sprite Loading error");
        LogIt(SDL_GetError());

        return false;
    }

	//kill the pink bg
    //SDL_SetColorKey(temp2, (SDL_SRCCOLORKEY|SDL_RLEACCEL), *(Uint8*)temp2->pixels);
	SDL_SetColorKey(temp2, SDL_TRUE, SDL_MapRGB(temp2->format, 0xFF, 0, 0xFF));
	SDL_SetSurfaceRLE(temp2, 1);
	final->spr = SDL_CreateTextureFromSurface(rend, temp2);
	//temp = SDL_DisplayFormat(temp2); //convert to display format
	SDL_FreeSurface(temp2);

	if (final->spr == 0)
    {
		LogIt("Unable to convert surface");
		LogIt(SDL_GetError());

		return false;
	}

    /*if (scr_off.scale)//scale up
    {
        z_temp = ZoomSurface(temp);
        final->spr = z_temp;//temp;
    }
    else*/
        //final->spr = temp; //normal scale


    //free these since they're wasting space now
    SDL_FreeRW(rwop);
    free(data);

    //now put the rest of data in
    final->w = hd.w;
    final->h = hd.h;
    final->mw = hd.mw;
    final->mh = hd.mh;
    final->frames = hd.images;

	return true;
}
//-----------------------------------------------------------------------------
void MusicExtract(FILE *fp, int size)
{
    FILE *out = NULL;
    char *v = NULL;

    //create temp file since sdl has no functions for loading music from memory
    out = fopen("data/temp.xm", "wb");

    v = reinterpret_cast<char*>(malloc(size));
    fread(v, size, 1, fp);

    fwrite(v, size, 1, out);

    free(v);

    fclose(out);

    return;
}
//-----------------------------------------------------------------------------
bool LumpSearchSnd(int id, int *size, void **v)
{
    LUMP_HEAD lump;
    CIF_HEAD ch;
    PADDING padding;
    bool found;
    FILE *in = fopen("data/data.pl0", "rb");


    if (in == NULL)
    {
        LogIt("Could not open lump");
        return false;
    }

    while(!feof(in))
    {
        found = true;
        fread(&lump, sizeof(LUMP_HEAD), 1, in);

        if (id != lump.id)
            found = false;

        switch(lump.type)//read next head
        {
            case 0:
                fread(&ch, sizeof(CIF_HEAD), 1, in);
            break;
            case 1:
            case 2:
                fread(&padding, sizeof(PADDING), 1, in);
            break;
            default:;
            break;
        }

        if (found)//found name id, get data
        {
            switch(lump.type)
            {
                case 0:
                break;
                case 1:
                    LoadToMem(in, lump.size, v);
                    fclose(in);
                    *size = lump.size;

                    if (v == NULL)
                    {
                        LogIt("Not enough space to load to memory");
                        return false;
                    }

                    return true;
                break;
                case 2:
                    MusicExtract(in, lump.size);
                    fclose(in);
                    return true;
                break;
                default:;
                break;
            }
        }
        else
            fseek(in, lump.size, SEEK_CUR);//go through data to next lump
    }

    fclose(in);

    return false;
}
//-----------------------------------------------------------------------------
bool LoadSound(int id, SOUND *final)
{
	SDL_RWops *rwop = NULL;
    int size;
    void *data = NULL; //for loading to memory

    //searches for sprite data in lump and loads into memory
	if (!LumpSearchSnd(id, &size, &data))
    {
        LogIt("Could not load following data");

        char converted[12];
        _itoa(id, converted, 10);

        LogIt(converted);

        return false;
    }

    rwop = SDL_RWFromMem(data, size);
    final->s = Mix_LoadWAV_RW(rwop, 0);

    if (final->s == NULL)
    {
        LogIt("Loading error:");
        LogIt(SDL_GetError());

        return false;
    }

    //free these since they're wasting space now
    SDL_FreeRW(rwop);
    free(data);

	return true;
}
//-----------------------------------------------------------------------------
bool LoadMusic(int id, MUSIC *final)
{
    int size;

    //searches for sprite data in lump and loads into memory
	if (!LumpSearchSnd(id, &size, NULL))
    {
        LogIt("Could not load following data");

        char converted[12];
        _itoa(id, converted, 10);

        LogIt(converted);

        return false;
    }

    final->m = Mix_LoadMUS("data/temp.xm"); //load temp file

    if (final->m == NULL)
    {
        LogIt("Loading error:");
        LogIt(SDL_GetError());

        return false;
    }

    remove("data/temp.xm");

	return true;
}
//-----------------------------------------------------------------------------











