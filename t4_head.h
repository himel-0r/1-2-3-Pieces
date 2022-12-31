#include <bits/stdc++.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_timer.h>

using namespace std;

const int N = 4;

SDL_Rect *rectselect4(SDL_Event &tevent, ttt box[N][N])
{
    int x, y;
    SDL_GetMouseState (&x, &y);

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (x >= box[i][j].r.x && x <= (box[i][j].r.x + 150))
            {
                if (y >= box[i][j].r.y && y <= (box[i][j].r.y + 150))
                {
                    return &box[i][j].r;
                }
            }
        }
    }

    return nullptr;
}

int count4(ttt box[N][N])
{
    int a = 0;

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (box[i][j].used)
                a++;
        }
    }

    return a;
}

int check_if_win4(ttt box[N][N])
{
    for (int i = 0; i < N; i++)
    {
        if (box[i][0].who == box[i][1].who && box[i][1].who == box[i][2].who && box[i][0].who == box[i][3].who)
        {
            return (box[i][0].who + 1);
        }
    }
    for (int j = 0; j < N; j++)
    {
        if (box[0][j].who == box[1][j].who && box[1][j].who == box[2][j].who && box[1][j].who == box[3][j].who)
        {
            return (box[0][j].who + 1);
        }
    }

    if (box[0][0].who == box[1][1].who && box[0][0].who == box[2][2].who && box[0][0].who == box[3][3].who)
    {
        return (box[0][0].who + 1);
    }
    if (box[0][3].who == box[1][2].who && box[1][2].who == box[2][1].who && box[3][0].who == box[0][3].who)
    {
        return (box[1][2].who + 1);
    }
    if (count4(box) == 16)
    {
        return 3; // tie;
    }
    return 0;
}

