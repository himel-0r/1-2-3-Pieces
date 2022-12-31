#include <bits/stdc++.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_timer.h>

using namespace std;

struct ttt
{
    SDL_Rect r;
    bool used;
    bool x;
    int who; // 0 and 1
};

SDL_Rect *rectselect(SDL_Event &tevent, ttt box[3][3])
{
    int x = tevent.button.x;
    int y = tevent.button.y;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (x >= box[i][j].r.x && x <= (box[i][j].r.x + 200))
            {
                if (y >= box[i][j].r.y && y <= (box[i][j].r.y + 200))
                {
                    return &box[i][j].r;
                }
            }
        }
    }

    return nullptr;
}

int count(ttt box[3][3])
{
    int a = 0;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (box[i][j].used)
                a++;
        }
    }

    return a;
}


int check_if_win(ttt box[3][3])
{
    if (count(box) == 9)
    {
        return 3; // tie;
    }
    for (int i = 0; i < 3; i++)
    {
        if (box[i][0].who == box[i][1].who && box[i][1].who == box[i][2].who)
        {
            //cout << i << " row\n";
            return (box[i][0].who + 1);
        }
    }
    for (int j = 0; j < 3; j++)
    {
        if (box[0][j].who == box[1][j].who && box[1][j].who == box[2][j].who)
        {
            //cout << j <<" col\n";
            return (box[0][j].who + 1);
        }
    }

    if (box[0][0].who == box[1][1].who && box[0][0].who == box[2][2].who)
    {
        //cout << "d1\n";
        return (box[0][0].who + 1);
    }
    if (box[0][2].who == box[1][1].who && box[1][1].who == box[2][0].who)
    {
        //cout << "d2\n";
        return (box[1][1].who + 1);
    }
    return 0;
}