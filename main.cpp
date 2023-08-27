
#include "Player.h"
#include "Platform.h"
#include "ttt.h"
#include "t4_head.h"
#include "variabl.h"

#include <iostream>

using namespace std;

SDL_Window *window;
SDL_Rect homeSprite_rect;
Platform platforms[8] = {{0}, {1}, {2}, {3}, {4}, {5}, {6}, {7}};
Player player(platforms[0].getX() + platforms[0].getWidth() / 2 - 26 / 2, platforms[0].getY() - player.getHeight(), 26, 32);

void Platform::updatePosition()
{
    x += zz;

    coinX = x + width / 2 - 24 / 2;
    coinY = y - 24 - 5;

    if (x > screenWidth)
    {
        y = rand() % 440 + 220;
        x = 0 - width;

        int coinInt = rand() % 8;

        if (coinInt == 0)
        {
            hasCoin = false;
            hasbomb = true;
        }
        else
        {
            hasCoin = true;
            hasbomb = false;
        }
    }
}
void Player::updatePosition()
{
    x += velocity.x;
    y += velocity.y;

    if (isOnGround() == true)
    {
        if (flag == 1)
        {
            flag = 0;
        }
        else
        {
            velocity = (Vector2){zz, 0};
        }
    }
    else
    {
        velocity.y += gravity;
    }

    if (x < 0)
        velocity.x *= -1;

    if (x + width > 1200)
        velocity.x *= -1;
}

int LoadHighScore()
{
    FILE *scorefile = fopen("highscore.bin", "rb");

    if (!scorefile)
        return 0;

    int ret;
    fread(&ret, sizeof(int), 1, scorefile);
    fclose(scorefile);

    return ret;
}

void SaveHighScore(int val)
{
    FILE *scorefile = fopen("highscore.bin", "wb");

    fwrite(&val, sizeof(int), 1, scorefile);
    fclose(scorefile);
}

void addScore(int amount)
{
    scoreInt += amount;

    if (scoreInt < 10)
        sprintf(score, "00%d", scoreInt);
    else if (scoreInt < 100)
        sprintf(score, "0%d", scoreInt);
    else
        sprintf(score, "%d", scoreInt);

    if (scoreInt > highscoreInt)
    {
        highscoreInt = scoreInt;
        sprintf(highscore, "BEST: %d", highscoreInt);
        if (highscoreInt < 10)
            sprintf(hs, "00%d", highscoreInt);
        else if (highscoreInt < 100)
            sprintf(hs, "0%d", highscoreInt);
        else
            sprintf(hs, "%d", highscoreInt);
    }
    zz += 0.005;
}

void resetScore()
{
    scoreInt = 0;
    sprintf(score, "00%d", scoreInt);
    SaveHighScore(highscoreInt);
}

void resetGame()
{
    resetScore();

    for (int i = 0; i < 8; i++)
        platforms[i] = Platform(i);

    player.setVelocity(0, 1);
    player.setX(platforms[0].getX() + platforms[0].getWidth() / 2 - 26 / 2);
    player.setY(platforms[0].getY() - player.getHeight());
    zz = 1;
    cnt = 0;
}

void Draw_Font(SDL_Renderer *renderer, const char *str, int x, int y, int width, int height, int size, SDL_Color color)
{
    TTF_Font *font = TTF_OpenFont("resources/font.otf", size);

    SDL_Surface *message_surf = TTF_RenderText_Blended(font, str, color);
    SDL_Texture *Message = SDL_CreateTextureFromSurface(renderer, message_surf);
    SDL_Rect Message_rect = {x, y, width, height};
    SDL_RenderCopy(renderer, Message, NULL, &Message_rect);

    SDL_DestroyTexture(Message);
    SDL_FreeSurface(message_surf);
    TTF_CloseFont(font);
}
int checkPlayerCollision()
{
    bool onPlatform = false;

    for (int i = 0; i < 8; i++)
    {
        if (platforms[i].getHasCoin() && player.getX() + player.getWidth() - 3 > platforms[i].getCoinX() && player.getX() + 7 < platforms[i].getCoinX() + 24 && player.getY() + player.getHeight() - 3 > platforms[i].getCoinY() && player.getY() + 3 < platforms[i].getCoinY() + 24)
        {
            addScore(1);
            platforms[i].setHasCoin(false);
            playCoinFX = true;
        }
        if (platforms[i].getHasbomb() && player.getX() + player.getWidth() - 3 > platforms[i].getCoinX() && player.getX() + 7 < platforms[i].getCoinX() + 24 && player.getY() + player.getHeight() - 3 > platforms[i].getCoinY() && player.getY() + 3 < platforms[i].getCoinY() + 24)
        {

            platforms[i].setHasbomb(false);
            return i;
        }

        if (player.getX() + 1 < platforms[i].getX() + platforms[i].getWidth() && player.getX() + player.getWidth() > platforms[i].getX() && player.getY() + player.getHeight() >= platforms[i].getY() && player.getY() < platforms[i].getY() + platforms[i].getHeight())
        {
            if (player.getY() > platforms[i].getY() + platforms[i].getHeight() / 2)
            {
                player.setVelocity(player.getVelocity().x, player.getVelocity().y);
            }
            else if (player.getY() + player.getHeight() < platforms[i].getY() + platforms[i].getHeight())
            {
                onPlatform = true;
                player.setY(platforms[i].getY() - player.getHeight());
                player.setY(player.getY() + 1);
            }
        }
    }
    player.setOnPlatform(onPlatform);
    return -1;
}
int *clicked(SDL_Event &e, ttt box[3][3])
{
    int x, y;
    SDL_GetMouseState(&x, &y);

    for (int i = 0; i < 3; i++)
        arr[i] = -1;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (x >= box[i][j].r.x && x <= (box[i][j].r.x + 200))
            {
                if (y >= box[i][j].r.y && y <= (box[i][j].r.y + 200))
                {
                    if (box[i][j].used == false)
                    {
                        int p = (count(box)) % 2;
                        box[i][j].used = true;

                        arr[0] = p;
                        arr[1] = i;
                        arr[2] = j;

                        return arr;
                    }
                }
            }
        }
    }
    return arr;
}
int *clicked4(SDL_Event &e, ttt box[N][N])
{
    int x, y;
    SDL_GetMouseState(&x, &y);

    for (int i = 0; i < N; i++)
        arr[i] = -1;

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (x >= box[i][j].r.x && x <= (box[i][j].r.x + 150))
            {
                if (y >= box[i][j].r.y && y <= (box[i][j].r.y + 150))
                {
                    if (box[i][j].used == false)
                    {
                        int p = (count4(box)) % 2;
                        box[i][j].used = true;

                        arr[0] = p;
                        arr[1] = i;
                        arr[2] = j;

                        return arr;
                    }
                }
            }
        }
    }
    return arr;
}

int main(int argc, char **argv)
{
    srand(time(NULL));

    resetScore();
    sprintf(highscore, "BEST: %d", highscoreInt);
    if (highscoreInt < 10)
        sprintf(hs, "00%d", highscoreInt);
    else if (highscoreInt < 100)
        sprintf(hs, "0%d", highscoreInt);
    else
        sprintf(hs, "%d", highscoreInt);

    SDL_Init(SDL_INIT_EVERYTHING);

    TTF_Init();

    window = SDL_CreateWindow(
        "1-2-3 Pieces",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        screenWidth, screenHeight,
        SDL_WINDOW_OPENGL);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_Surface *egg = IMG_Load("resources/splash_egg1.png");
    SDL_SetWindowIcon(window, egg);

    Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 4096);
    // SetMasterVolume(0.3f);

    SDL_Surface *playerSprite_surf = IMG_Load("resources/png1.png");
    SDL_Surface *lavaSprite_surf = IMG_Load("resources/lava.png");
    SDL_Surface *platformSprite_surf = IMG_Load("resources/platform.png");
    SDL_Surface *coinSprite_surf = IMG_Load("resources/coin.png");
    SDL_Surface *scoreBoxSprite_surf = IMG_Load("resources/scorebox.png");
    SDL_Surface *logo_surf = IMG_Load("resources/logo.png");
    SDL_Surface *splashEggSprite_surf = IMG_Load("resources/splash_egg.png");
    SDL_Surface *exit_s = IMG_Load("resources/exit.png");
    SDL_Surface *bombSprite_surf = IMG_Load("resources/bomb.png");
    SDL_Surface *homeSprite_surf = IMG_Load("resources/home.png");
    SDL_Surface *titleSprite_surf = IMG_Load("resources/title_2.png");
    SDL_Surface *titleSprite_surf1 = IMG_Load("resources/title.png");
    SDL_Surface *blue_circle_surf = IMG_Load("resources/wooden_O.png");
    SDL_Surface *red_cross_surf = IMG_Load("resources/wooden_X.png");
    SDL_Surface *background = IMG_Load("resources/wood_bg.png");
    SDL_Surface *ttt_S = IMG_Load("resources/ttt.png");
    SDL_Surface *wh_S = IMG_Load("resources/wh.png");
    SDL_Surface *t4_S = IMG_Load("resources/t_4.png");
    SDL_Surface *blue_turn_surf = IMG_Load("resources/o__turn.png");
    SDL_Surface *red_turn_surf = IMG_Load("resources/x__turn.png");
    SDL_Surface *eggfry_cover_surf = IMG_Load("resources/eggfry_cover.png");
    SDL_Surface *ttt3_cover_surf = IMG_Load("resources/t_3_cover.png");
    SDL_Surface *ttt4_cover_surf = IMG_Load("resources/t_4_cover.png");
    SDL_Surface *info_cover_surf = IMG_Load("resources/info_icon.png");
    SDL_Surface *credit_surf = IMG_Load("resources/credit2.png");
    SDL_Surface *exit_cover_surf = IMG_Load("resources/exit_1.png");
    SDL_Surface *goh_surf = IMG_Load("resources/goh.png");
    SDL_Surface *gon_surf = IMG_Load("resources/gon.png");
    SDL_Surface *rtry_surf = IMG_Load("resources/rtry.png");
    SDL_Surface *quit_surf = IMG_Load("resources/quit.png");
    SDL_Surface *info_surf = IMG_Load("resources/info1.png");
    SDL_Surface *cc_surf = IMG_Load("resources/cc.png");

    SDL_Texture *playerSprite = SDL_CreateTextureFromSurface(renderer, playerSprite_surf);
    SDL_Texture *lavaSprite = SDL_CreateTextureFromSurface(renderer, lavaSprite_surf);
    SDL_Texture *platformSprite = SDL_CreateTextureFromSurface(renderer, platformSprite_surf);
    SDL_Texture *coinSprite = SDL_CreateTextureFromSurface(renderer, coinSprite_surf);
    SDL_Texture *scoreBoxSprite = SDL_CreateTextureFromSurface(renderer, scoreBoxSprite_surf);
    SDL_Texture *logo = SDL_CreateTextureFromSurface(renderer, logo_surf);
    SDL_Texture *splashEggSprite = SDL_CreateTextureFromSurface(renderer, splashEggSprite_surf);
    SDL_Texture *exit = SDL_CreateTextureFromSurface(renderer, exit_s);
    SDL_Texture *bomb1Sprite = IMG_LoadTexture(renderer, "resources/bomb1.gif");
    SDL_Texture *bombSprite = SDL_CreateTextureFromSurface(renderer, bombSprite_surf);
    SDL_Texture *homeSprite = SDL_CreateTextureFromSurface(renderer, homeSprite_surf);
    SDL_Texture *title = SDL_CreateTextureFromSurface(renderer, titleSprite_surf);
    SDL_Texture *title1 = SDL_CreateTextureFromSurface(renderer, titleSprite_surf1);
    SDL_Texture *eggSprite = SDL_CreateTextureFromSurface(renderer, egg);
    SDL_Texture *blue_circle = SDL_CreateTextureFromSurface(renderer, blue_circle_surf);
    SDL_Texture *red_cross = SDL_CreateTextureFromSurface(renderer, red_cross_surf);
    SDL_Texture *bg = SDL_CreateTextureFromSurface(renderer, background);
    SDL_Texture *ttt = SDL_CreateTextureFromSurface(renderer, ttt_S);
    SDL_Texture *wh = SDL_CreateTextureFromSurface(renderer, wh_S);
    SDL_Texture *t4 = SDL_CreateTextureFromSurface(renderer, t4_S);
    SDL_Texture *red_turn = SDL_CreateTextureFromSurface(renderer, red_turn_surf);
    SDL_Texture *blue_turn = SDL_CreateTextureFromSurface(renderer, blue_turn_surf);
    SDL_Texture *eggfry_cover = SDL_CreateTextureFromSurface(renderer, eggfry_cover_surf);
    SDL_Texture *ttt3_cover = SDL_CreateTextureFromSurface(renderer, ttt3_cover_surf);
    SDL_Texture *ttt4_cover = SDL_CreateTextureFromSurface(renderer, ttt4_cover_surf);
    SDL_Texture *info_cover = SDL_CreateTextureFromSurface(renderer, info_cover_surf);
    SDL_Texture *credit_cover = SDL_CreateTextureFromSurface(renderer, credit_surf);
    SDL_Texture *exit_cover = SDL_CreateTextureFromSurface(renderer, exit_cover_surf);
    SDL_Texture *goh = SDL_CreateTextureFromSurface(renderer, goh_surf);
    SDL_Texture *gon = SDL_CreateTextureFromSurface(renderer, gon_surf);
    SDL_Texture *rtry = SDL_CreateTextureFromSurface(renderer, rtry_surf);
    SDL_Texture *qut = SDL_CreateTextureFromSurface(renderer, quit_surf);
    SDL_Texture *info = SDL_CreateTextureFromSurface(renderer, info_surf);
    SDL_Texture *cc = SDL_CreateTextureFromSurface(renderer, cc_surf);

    Mix_Chunk *fxLaunch = Mix_LoadWAV("resources/launch.wav");
    Mix_Chunk *fxClick = Mix_LoadWAV("resources/click.wav");
    Mix_Chunk *fxDeath = Mix_LoadWAV("resources/die.wav");
    Mix_Chunk *fxCoin = Mix_LoadWAV("resources/coin.wav");
    Mix_Chunk *fxSplash = Mix_LoadWAV("resources/splash.wav");
    Mix_Chunk *fxSelect = Mix_LoadWAV("resources/select.wav");

    bool quit = false;
    bool mouse_down = false;

    int mouse_x, mouse_y;

    while (!quit)
    {
        SDL_Event e;
        bool mouse_released = false;
        bool mouse_pressed = false;

        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
            case SDL_QUIT:
            {
                quit = true;
            }
            break;
            case SDL_MOUSEBUTTONDOWN:
            {
                mouse_down = true;
                mouse_pressed = true;
            }
            break;
            case SDL_MOUSEBUTTONUP:
            {
                mouse_down = false;
                mouse_released = true;
            }
            break;
            }
        }

        SDL_PumpEvents();
        SDL_GetMouseState(&mouse_x, &mouse_y);

        // TODO: Vsync instead
        SDL_Delay(12);
        if (TITLEScreen)
        {
            if (SplashTimer > 120)
            {
                SDL_Rect title_rect = {0, 0, 1200, 800};
                SDL_RenderCopy(renderer, title, NULL, &title_rect);
                //SDL_RenderPresent(renderer);

                /////////
                if (mouse_x >= 405 && mouse_x <= 795 && mouse_y >= 150 && mouse_y <= 280)
                {
                    // egg
                    SDL_Rect eggcoverboro = {385, 130, 430, 170};
                    SDL_RenderCopy(renderer, eggfry_cover, NULL, &eggcoverboro);
                    //SDL_RenderPresent(renderer);
                }
                if (mouse_x >= 405 && mouse_x <= 795 && mouse_y >= 345 && mouse_y <= 470)
                {
                    // t3
                    SDL_Rect t3onekboro = {385, 325, 430, 165};
                    SDL_RenderCopy(renderer, ttt3_cover, NULL, &t3onekboro);
                    //SDL_RenderPresent(renderer);
                }
                if (mouse_x >= 405 && mouse_x <= 795 && mouse_y >= 520 && mouse_y <= 655)
                {
                    // t4
                    SDL_Rect t4onekboro = {385, 500, 430, 175};
                    SDL_RenderCopy(renderer, ttt4_cover, NULL, &t4onekboro);
                    //SDL_RenderPresent(renderer);
                }
                if (mouse_x >= 1045 && mouse_x <= 1140 && mouse_y >= 60 && mouse_y <= 135)
                {
                    // credit
                    SDL_Rect creditvbohutboro = {1045, 50, 100, 100};
                    SDL_RenderCopy(renderer, credit_cover, NULL, &creditvbohutboro);
                    //SDL_RenderPresent(renderer);
                }
                if (mouse_x >= 50 && mouse_x <= 125 && mouse_y >= 695 && mouse_y <= 765)
                {
                    // info
                    SDL_Rect amarinfoonekboro = {40, 685, 95, 90};
                    SDL_RenderCopy(renderer, info_cover, NULL, &amarinfoonekboro);
                    //SDL_RenderPresent(renderer);
                }
                if (mouse_x >= 1055 && mouse_x <= 1135 && mouse_y >= 680 && mouse_y <= 720)
                {
                    // exit
                    SDL_Rect amardorjaonekboro = {1055, 650, 100, 80};
                    SDL_RenderCopy(renderer, exit_cover, NULL, &amardorjaonekboro);
                    //SDL_RenderPresent(renderer);
                }
                /////////

                SDL_RenderPresent(renderer);

                if (mouse_pressed)
                {
                    Mix_PlayChannel(-1, fxSelect, 0);

                    mouseDownX = mouse_x;
                    mouseDownY = mouse_y;

                    // cout << mouseDownX << " " << mouseDownY << endl;

                    if (mouseDownX >= 405 && mouseDownX <= 795 && mouseDownY >= 150 && mouseDownY <= 280)
                    {
                        // EGG FRY
                        cout << "EGG FRY\n";
                        TITLEScreen = false;
                        gameover = 1;
                        titleScreen = true;
                        playCoinFX = false;
                        zz = 1;
                        cnt = 0;
                        flag = 0;
                        mouseDownX = 0;
                        mouseDownY = 0;
                        lavaY = screenHeight - 32;
                        timer = 0;
                        splashTimer = 0;

                        firstTime = true;
                        playedSplash = false;
                        playedSelect = false;
                        mouse_down = false;
                        mouse_released = false;
                        mouse_pressed = false;
                        go = 0;
                    }
                    else if (mouseDownX >= 405 && mouseDownX <= 795 && mouseDownY >= 345 && mouseDownY <= 470)
                    {
                        // TTT
                        cout << "TTT \n";
                        gameover = 2;
                        TITLEScreen = false;
                        mouse_down = false;
                        mouse_released = false;
                        mouse_pressed = false;
                    }
                    else if (mouseDownX >= 405 && mouseDownX <= 795 && mouseDownY >= 520 && mouseDownY <= 655)
                    {
                        // TT 4
                        cout << "TTT-4 \n";
                        gameover = 3;
                        TITLEScreen = false;
                        mouse_down = false;
                        mouse_released = false;
                        mouse_pressed = false;
                    }
                    else if (mouseDownX >= 1045 && mouseDownX <= 1140 && mouseDownY >= 60 && mouseDownY <= 135)
                    {
                        // Credits
                        cout << "Credits\n";
                        gameover = 5;
                        TITLEScreen = false;
                        mouse_down = false;
                        mouse_released = false;
                        mouse_pressed = false;
                        SDL_RenderClear(renderer);
                    }
                    else if (mouseDownX >= 50 && mouseDownX <= 125 && mouseDownY >= 695 && mouseDownY <= 765)
                    {
                        // INFO
                        cout << "INFO\n";
                        gameover = 4;
                        TITLEScreen = false;
                        mouse_down = false;
                        mouse_released = false;
                        mouse_pressed = false;
                        SDL_RenderClear(renderer);
                    }
                    else if (mouseDownX >= 1055 && mouseDownX <= 1135 && mouseDownY >= 680 && mouseDownY <= 720)
                    {
                        quit = true;
                        return 0;
                    }
                }
            }

            else
            {

                if (bl2 == false)
                {
                    SDL_Rect title_rect1 = {0, 0, 1200, 800};
                    SDL_RenderCopy(renderer, title1, NULL, &title_rect1);
                    SDL_RenderPresent(renderer);
                    bl2 = true;
                }

                Draw_Font(renderer, "ONE TWO THREE PIECE", screenWidth / 2 - 100, screenHeight / 2 + 3, 262, 52, 120, {213, 128, 90});

                SDL_Rect eggSprite_rect = {screenWidth / 2 - 30, screenHeight / 2 - 16 - 93, 120, 120};
                SDL_RenderCopy(renderer, eggSprite, NULL, &eggSprite_rect);

                SDL_RenderPresent(renderer);

                SplashTimer += 1;
            }
        }
        else
        {
            if (gameover == 1)
            {
                if (titleScreen)
                {
                    if (splashTimer > 120)
                    {
                        if (!playedSelect)
                        {
                            Mix_PlayChannel(-1, fxSelect, 0);
                            playedSelect = true;
                        }

                        SDL_SetRenderDrawColor(renderer, 0.93 * 255, 0.89 * 255, 0.8 * 255, 1.0 * 255);
                        SDL_RenderClear(renderer);

                        SDL_Rect logo_rect = {screenWidth / 2 - 200, screenHeight / 2 - 130, 400, 200};
                        SDL_RenderCopy(renderer, logo, NULL, &logo_rect);

                        Draw_Font(renderer, highscore, screenWidth / 2 - 37, screenHeight / 2 + 10, 74, 32, 32, {0, 0, 0});
                        Draw_Font(renderer, "CLICK TO BEGIN", screenWidth / 2 - 134, screenHeight / 2 + 50, 268, 32, 32, {178, 150, 125});

                        SDL_RenderPresent(renderer);

                        if (mouse_pressed)
                        {
                            Mix_PlayChannel(-1, fxSelect, 0);
                            titleScreen = false;
                            mouseDownX = mouse_x;
                            mouseDownY = mouse_y;
                        }
                    }
                    else
                    {
                        if (!playedSplash)
                        {
                            Mix_PlayChannel(-1, fxSelect, 0);
                            playedSplash = true;
                        }

                        SDL_SetRenderDrawColor(renderer, 0.93 * 255, 0.89 * 255, 0.8 * 255, 1.0 * 255);
                        SDL_RenderClear(renderer);

                        Draw_Font(renderer, "ONE PIECE", screenWidth / 2 - 54, screenHeight / 2 + 3, 108, 32, 32, {213, 128, 90});

                        SDL_Rect splashEggSprite_rect = {screenWidth / 2 - 16, screenHeight / 2 - 16 - 23, 32, 32};
                        SDL_RenderCopy(renderer, splashEggSprite, NULL, &splashEggSprite_rect);

                        SDL_RenderPresent(renderer);

                        splashTimer += 1;
                    }
                }
                else
                {
                    if (go == 0)
                    {

                        if (firstTime == true)
                        {
                            SDL_Delay(500);
                            resetGame();
                        }
                        if (playCoinFX)
                        {
                            Mix_PlayChannel(-1, fxCoin, 0);
                            playCoinFX = false;
                        }

                        if (mouse_pressed && player.isOnGround())
                        {
                            Mix_PlayChannel(-1, fxClick, 0);
                            mouseDownX = mouse_x;
                            mouseDownY = mouse_y;
                            if (mouseDownX >= 1100 && mouseDownX <= 1170 && mouseDownY >= 17 && mouseDownY <= 87)
                            {
                                SDL_Delay(500);
                                TITLEScreen = true;
                                bl1 = false;
                            }
                        }

                        if (mouse_released && player.isOnGround())
                        {
                            if (firstTime)
                            {

                                firstTime = false;
                            }
                            else
                            {
                                Mix_PlayChannel(-1, fxLaunch, 0);

                                if (player.isOnPlatform())
                                    player.setX(player.getX() - 1);

                                double velocityX = mouse_x - mouseDownX;
                                double velocityY = mouse_y - mouseDownY;

                                player.setVelocity((double)velocityX * 0.08, (double)velocityY * 0.08);
                            }
                            flag = 1;
                        }

                        int bomb = checkPlayerCollision();
                        if (bomb != -1)
                        {
                            int i = bomb;
                            SDL_Rect bomb1Sprite_rect = {platforms[i].getCoinX(), platforms[i].getCoinY(), 80, 80};
                            SDL_RenderCopy(renderer, bomb1Sprite, NULL, &bomb1Sprite_rect);
                            // SDL_Delay(3000);
                            Mix_PlayChannel(-1, fxDeath, 0);
                            SDL_Delay(500);
                            go = 1;
                            // resetGame();
                        }
                        player.updatePosition();

                        if (player.getY() > screenHeight)
                        {
                            Mix_PlayChannel(-1, fxDeath, 0);
                            SDL_Delay(500);
                            go = 1;
                            // resetGame();
                        }
                        for (int i = 0; i < 8; i++)
                        {
                            platforms[i].updatePosition();
                        }
                        /*for(int i=0;i<4;i++){
                            cloud[i].updateposition();
                        }*/

                        lavaY = screenHeight - 43 - sin(timer) * 5;
                        timer += 0.05;

                        SDL_SetRenderDrawColor(renderer, 0.95 * 255, 0.91 * 255, 0.835 * 255, 1.0 * 255);
                        SDL_RenderClear(renderer);

                        if (mouse_down && player.isOnGround())
                        {
                            SDL_SetRenderDrawColor(renderer, 178, 150, 125, 255);
                            SDL_RenderDrawLine(
                                renderer,
                                mouseDownX + (player.getX() - mouseDownX) + (player.getWidth() / 2),
                                mouseDownY + (player.getY() - mouseDownY) + (player.getHeight() / 2),
                                mouse_x + (player.getX() - mouseDownX) + (player.getWidth() / 2),
                                mouse_y + (player.getY() - mouseDownY) + (player.getHeight() / 2));
                        }

                        // DrawRectangle(player.getX(), player.getY(), player.getWidth(), player.getHeight(), WHITE);

                        for (int i = 0; i < 8; i++)
                        {
                            // int getX =(int) platforms[i].getX();
                            // int getY =(int) platforms[i].getY();

                            SDL_Rect platformSprite_rect = {(int)platforms[i].getX(), (int)platforms[i].getY(), 100, 32};
                            // SDL_Rect cloudSprite_rect = { cloud[i].getX(), cloud[i].getY(), 60, 40 };
                            SDL_RenderCopy(renderer, platformSprite, NULL, &platformSprite_rect);
                            // SDL_RenderCopy(renderer, cloudSprite, NULL, &cloudSprite_rect);

                            if (platforms[i].getHasCoin())
                            {
                                SDL_Rect coinSprite_rect = {platforms[i].getCoinX(), platforms[i].getCoinY(), 24, 24};
                                SDL_RenderCopy(renderer, coinSprite, NULL, &coinSprite_rect);
                            }
                            if (platforms[i].getHasbomb())
                            {
                                SDL_Rect bombSprite_rect = {platforms[i].getCoinX(), platforms[i].getCoinY(), 40, 40};
                                SDL_RenderCopy(renderer, bombSprite, NULL, &bombSprite_rect);
                            }
                        }
                        // int getX= (int) player.getX();
                        // int getY= (int) player.getY();

                        SDL_Rect playerSprite_rect = {(int)player.getX(), (int)player.getY(), 32, 32};
                        SDL_RenderCopy(renderer, playerSprite, NULL, &playerSprite_rect);

                        SDL_Rect lavaSprite_rect = {-100, (int)lavaY, 1400, 68};
                        SDL_RenderCopy(renderer, lavaSprite, NULL, &lavaSprite_rect);

                        SDL_Rect scoreBoxSprite_rect = {17, 17, 102, 70};
                        SDL_RenderCopy(renderer, scoreBoxSprite, NULL, &scoreBoxSprite_rect);

                        SDL_Rect homeSprite_rect = {1100, 17, 70, 70};
                        SDL_RenderCopy(renderer, homeSprite, NULL, &homeSprite_rect);

                        Draw_Font(renderer, score, 28, 20, 75, 64, 64, {0, 0, 0});
                        Draw_Font(renderer, highscore, 17, 90, 74, 32, 32, {0, 0, 0});

                        SDL_RenderPresent(renderer);
                    }
                    else
                    {
                        // cout<<scoreInt<<" "<<highscoreInt<<endl;
                        if (scoreInt >= highscoreInt)
                        {
                            SDL_Rect goh_rect = {400, 250, 400, 300};
                            SDL_RenderCopy(renderer, goh, NULL, &goh_rect);
                            Draw_Font(renderer, score, 640, 330, 74, 32, 32, {255, 255, 255});
                            //SDL_RenderPresent(renderer);
                        }
                        else
                        {
                            SDL_Rect gon_rect = {400, 250, 400, 300};
                            SDL_RenderCopy(renderer, gon, NULL, &gon_rect);
                            Draw_Font(renderer, score, 640, 297, 74, 32, 32, {255, 255, 255});
                            Draw_Font(renderer, hs, 640, 330, 74, 32, 32, {255, 255, 255});
                            //SDL_RenderPresent(renderer);
                        }
                        if (mouse_x >= 465 && mouse_x <= 582 && mouse_y >= 440 && mouse_y <= 469)
                        {
                            SDL_Rect rtry_rect = {462, 438, 123, 33};
                            SDL_RenderCopy(renderer, rtry, NULL, &rtry_rect);
                            //SDL_RenderPresent(renderer);
                        }
                        else if (mouse_x >= 618 && mouse_x <= 736 && mouse_y >= 440 && mouse_y <= 469)
                        {
                            SDL_Rect qut_rect = {615, 438, 123, 33};
                            SDL_RenderCopy(renderer, qut, NULL, &qut_rect);
                            //SDL_RenderPresent(renderer);
                        }
                        SDL_RenderPresent(renderer);

                        if (mouse_pressed)
                        {
                            mouseDownX = mouse_x;
                            mouseDownY = mouse_y;
                            cout << mouseDownX << " " << mouseDownY << endl;
                            if (mouseDownX >= 465 && mouseDownX <= 582 && mouseDownY >= 440 && mouseDownY <= 469)
                            {
                                SDL_Delay(500);
                                resetGame();
                                go = 0;
                            }
                            else if (mouseDownX >= 618 && mouseDownX <= 736 && mouseDownY >= 440 && mouseDownY <= 469)
                            {
                                SDL_Delay(500);
                                resetGame();
                                TITLEScreen = true;
                                bl1 = false;
                                go = 0;
                            }
                        }
                    }
                }
            }
            else if (gameover == 2)
            {
                // cout << 1 << endl;
                cnn = 0;
                SDL_Rect tspace;
                tspace.x = 100;
                tspace.y = 100;
                tspace.h = 605;
                tspace.w = 605;

                struct ttt box[3][3];

                for (int i = 0; i < 3; i++)
                {
                    for (int j = 0; j < 3; j++)
                    {
                        box[i][j].used = false;
                        box[i][j].x = false;
                        box[i][j].r.h = 195;
                        box[i][j].r.w = 195;
                        box[i][j].r.x = 105 + 200 * j;
                        box[i][j].r.y = 105 + 200 * i;
                        box[i][j].who = 10 + i * 3 + j;
                    }
                }

                SDL_Rect bg_rect = {0, 0, 1200, 800};
                SDL_RenderCopy(renderer, bg, NULL, &bg_rect);

                SDL_Rect home_button = {1110, 710, 65, 65};
                SDL_RenderCopy(renderer, homeSprite, NULL, &home_button);

                SDL_Rect *currentrect = nullptr;
                SDL_Rect *mouserect = nullptr;

                SDL_Rect wh_rect = {100, 100, 605, 605};
                SDL_RenderCopy(renderer, wh, NULL, &wh_rect);
                //SDL_RenderPresent(renderer);

                SDL_SetRenderDrawColor(renderer, 255, 255, 200, 0);
                SDL_RenderFillRect(renderer, &tspace);
                //SDL_RenderPresent(renderer);

                SDL_Rect turn_b = {800, 200, 300, 50};
                SDL_RenderCopy(renderer, blue_turn, NULL, &turn_b);
                //SDL_RenderPresent(renderer);

                SDL_SetRenderDrawColor(renderer, 255, 165, 80, 0);
                for (int i = 0; i < 3; i++)
                {
                    for (int j = 0; j < 3; j++)
                    {
                        SDL_RenderFillRect(renderer, &box[i][j].r);
                        //SDL_RenderPresent(renderer);
                    }
                }
                SDL_RenderPresent(renderer);
                bool running = true;
                SDL_Event event;
                bool mousedown = false;
                while (running == true)
                {

                    while (SDL_PollEvent(&event))
                    {
                        switch (event.type)
                        {
                        case SDL_QUIT:
                            running = false;
                            quit = true;
                            break;
                        case SDL_MOUSEMOTION:
                            currentrect = rectselect(event, box);
                            break;
                        case SDL_MOUSEBUTTONDOWN:
                            mousedown = true;
                        }
                    }

                    if (mousedown)
                    {
                        int *k = clicked(event, box);

                        // cout << k[0] << " " << k[1] << " " << k[2] << endl;

                        switch (k[0])
                        {
                        case 0:
                        {
                            box[k[1]][k[2]].who = 0;
                            Mix_PlayChannel(-1, fxSelect, 0);
                            SDL_Rect blue_circle_rect = {125 + 200 * (k[2]), 125 + 200 * (k[1]), 155, 155};
                            SDL_RenderCopy(renderer, blue_circle, NULL, &blue_circle_rect);
                            SDL_Rect turn_r = {800, 200, 300, 50};
                            SDL_RenderCopy(renderer, red_turn, NULL, &turn_r);
                            SDL_RenderPresent(renderer);
                            break;
                        }
                        case 1:
                        {
                            box[k[1]][k[2]].who = 1;
                            Mix_PlayChannel(-1, fxSelect, 0);
                            SDL_Rect red_cross_rect = {125 + 200 * (k[2]), 125 + 200 * (k[1]), 155, 155};
                            SDL_RenderCopy(renderer, red_cross, NULL, &red_cross_rect);
                            SDL_Rect turn_b = {800, 200, 300, 50};
                            SDL_RenderCopy(renderer, blue_turn, NULL, &turn_b);
                            SDL_RenderPresent(renderer);
                            break;
                        }
                        }
                        mousedown = false;

                        int result = check_if_win(box); // 0=continue, 1=blue, 2=red, 3=tie

                        if (result > 0)
                        {
                            // char str[40];
                            if (result == 1)
                            {
                                // sprintf(str, "Blue");
                                Draw_Font(renderer, "CIRCLE Won!!!", 820, 320, 360, 160, 30, {255, 225, 200});
                            }
                            else if (result == 2)
                            {
                                // sprintf(str, "red");
                                Draw_Font(renderer, "CROSS Won!!!", 820, 320, 360, 160, 30, {255, 225, 200});
                            }
                            else if (result == 3)
                            {
                                // sprintf(str, "draw");
                                Draw_Font(renderer, "DRAW!!!", 820, 320, 360, 160, 30, {255, 225, 200});
                            }

                            SDL_RenderPresent(renderer);

                            SDL_Delay(1000);

                            running = false;
                        }

                        int x, y;
                        SDL_GetMouseState(&x, &y);

                        if (x >= 1110 && x <= 1175 && y >= 710 && y <= 775)
                        {
                            SDL_Delay(500);
                            running = false;
                            TITLEScreen = true;
                            bl1 = false;
                        }
                    }
                }
            }
            else if (gameover == 3)
            {
                SDL_Event event;
                bool running = true;

                SDL_Rect tspace;
                tspace.x = 100;
                tspace.y = 100;
                tspace.h = 605;
                tspace.w = 605;
                bool mousedown = false;

                struct ttt box[N][N];

                for (int i = 0; i < N; i++)
                {
                    for (int j = 0; j < N; j++)
                    {
                        box[i][j].used = false;
                        box[i][j].x = false;
                        box[i][j].r.h = 145;
                        box[i][j].r.w = 145;
                        box[i][j].r.x = 105 + 150 * j;
                        box[i][j].r.y = 105 + 150 * i;
                        box[i][j].who = 10 + i * N + j;
                    }
                }

                SDL_Rect bg_rect = {0, 0, 1200, 800};
                SDL_RenderCopy(renderer, bg, NULL, &bg_rect);
                // SDL_RenderPresent(renderer);

                SDL_Rect home_button = {1110, 710, 65, 65};
                SDL_RenderCopy(renderer, homeSprite, NULL, &home_button);

                SDL_Rect *currentrect = nullptr;
                SDL_Rect *mouserect = nullptr;

                SDL_SetRenderDrawColor(renderer, 255, 255, 200, 0);
                SDL_RenderFillRect(renderer, &tspace);
                // SDL_RenderPresent(renderer);

                SDL_Rect turn_b = {800, 200, 300, 50};
                SDL_RenderCopy(renderer, blue_turn, NULL, &turn_b);
                // SDL_RenderPresent(renderer);

                SDL_SetRenderDrawColor(renderer, 255, 165, 80, 0);
                for (int i = 0; i < N; i++)
                {
                    for (int j = 0; j < N; j++)
                    {
                        std::cerr << box[i][j].r.x << " " << box[i][j].r.y << " " << i << " " << j << "\n";
                        SDL_RenderFillRect(renderer, &box[i][j].r);
                        // SDL_RenderPresent(renderer);
                    }
                }

                SDL_RenderPresent(renderer);
                while (running)
                {
                    while (SDL_PollEvent(&event))
                    {
                        switch (event.type)
                        {
                        case SDL_QUIT:
                            running = false;
                            break;
                        case SDL_MOUSEMOTION:
                            currentrect = rectselect4(event, box);
                            break;
                        case SDL_MOUSEBUTTONDOWN:
                            mousedown = true;
                        }
                    }

                    if (mousedown)
                    {
                        int *k = clicked4(event, box);

                        // cout << k[0] << " " << k[1] << " " << k[2] << endl;

                        switch (k[0])
                        {
                        case 0:
                        {
                            box[k[1]][k[2]].who = 0;
                            Mix_PlayChannel(-1, fxSelect, 0);
                            SDL_Rect blue_circle_rect = {120 + 150 * (k[2]), 120 + 150 * (k[1]), 120, 120};
                            SDL_RenderCopy(renderer, blue_circle, NULL, &blue_circle_rect);
                            SDL_Rect turn_r = {800, 200, 300, 50};
                            SDL_RenderCopy(renderer, red_turn, NULL, &turn_r);
                            SDL_RenderPresent(renderer);
                            break;
                        }
                        case 1:
                        {
                            box[k[1]][k[2]].who = 1;
                            Mix_PlayChannel(-1, fxSelect, 0);
                            SDL_Rect red_cross_rect = {120 + 150 * (k[2]), 120 + 150 * (k[1]), 120, 120};
                            SDL_RenderCopy(renderer, red_cross, NULL, &red_cross_rect);
                            SDL_Rect turn_r = {800, 200, 300, 50};
                            SDL_RenderCopy(renderer, blue_turn, NULL, &turn_r);
                            SDL_RenderPresent(renderer);
                            break;
                        }
                        }
                        mousedown = false;

                        int result = check_if_win4(box); // 0=continue, 1=blue, 2=red, 3=tie

                        if (result > 0)
                        {
                            if (result == 1)
                            {
                                Draw_Font(renderer, "CIRCLE Won!!!", 820, 320, 360, 160, 30, {0, 225, 255});
                            }
                            else if (result == 2)
                            {
                                Draw_Font(renderer, "CROSS Won!!!", 820, 320, 360, 160, 30, {255, 225, 0});
                            }
                            else if (result == 3)
                            {
                                Draw_Font(renderer, "DRAW!!!", 820, 320, 360, 160, 30, {200, 225, 200});
                            }

                            SDL_RenderPresent(renderer);

                            SDL_Delay(1000);
                            running = false;
                        }

                        int x, y;
                        SDL_GetMouseState(&x, &y);

                        if (x >= 1110 && x <= 1175 && y >= 710 && y <= 775)
                        {
                            SDL_Delay(500);
                            running = false;
                            TITLEScreen = true;
                            bl1 = false;
                        }
                    }
                }
            }
            else if (gameover == 4)
            {
                
                
                SDL_Rect info_rect = {0, 0, 1200, 800};
                SDL_RenderCopy(renderer, info, NULL, &info_rect);
                SDL_Rect home_button = {1085, 685, 65, 65};
                SDL_RenderCopy(renderer, homeSprite, NULL, &home_button);
                
                if (mouse_pressed)
                {
                    if (mouse_x >= 1085 && mouse_x <= 1150 && mouse_y >= 685 && mouse_y <= 750)
                    {
                        SDL_Delay(500);
                        TITLEScreen = true;
                        bl1 = false;
                        gameover = 0;
                        credit=0;
                    }
                    mouse_pressed=false;
                }
                SDL_RenderPresent(renderer);
                //SDL_RenderClear(renderer);
            }
            else if (gameover == 5)
            {
                
                SDL_Rect cc_rect = {0, 0, 1200, 800};
                SDL_RenderCopy(renderer, cc, NULL, &cc_rect);
                //SDL_RenderPresent(renderer);
                SDL_Rect home_button = {1090, 685, 65, 65};
                SDL_RenderCopy(renderer, homeSprite, NULL, &home_button);
                
                if (mouse_pressed)
                {
                    if (mouse_x >= 1090 && mouse_x <= 1155 && mouse_y >= 685 && mouse_y <= 750)
                    {
                        SDL_Delay(500);
                        TITLEScreen = true;
                        bl1 = false;
                        gameover = 0;
                        credit=0;
                    }
                    mouse_pressed=false;
                }
                SDL_RenderPresent(renderer);
                
            }
        }
    }

    SDL_DestroyTexture(playerSprite);
    SDL_DestroyTexture(lavaSprite);
    SDL_DestroyTexture(platformSprite);
    SDL_DestroyTexture(coinSprite);
    SDL_DestroyTexture(scoreBoxSprite);
    SDL_DestroyTexture(logo);
    SDL_DestroyTexture(splashEggSprite);
    SDL_DestroyTexture(bomb1Sprite);
    SDL_DestroyTexture(bombSprite);
    SDL_DestroyTexture(homeSprite);
    SDL_DestroyTexture(title);
    SDL_DestroyTexture(title1);
    SDL_DestroyTexture(eggSprite);
    SDL_DestroyTexture(blue_circle);
    SDL_DestroyTexture(red_cross);
    SDL_DestroyTexture(bg);
    SDL_DestroyTexture(ttt);
    SDL_DestroyTexture(wh);
    SDL_DestroyTexture(t4);
    SDL_DestroyTexture(red_turn);
    SDL_DestroyTexture(blue_turn);
    SDL_DestroyTexture(eggfry_cover);
    SDL_DestroyTexture(ttt3_cover);
    SDL_DestroyTexture(ttt4_cover);
    SDL_DestroyTexture(info_cover);
    SDL_DestroyTexture(credit_cover);
    SDL_DestroyTexture(exit_cover);

    SDL_FreeSurface(playerSprite_surf);
    SDL_FreeSurface(lavaSprite_surf);
    SDL_FreeSurface(platformSprite_surf);
    SDL_FreeSurface(coinSprite_surf);
    SDL_FreeSurface(scoreBoxSprite_surf);
    SDL_FreeSurface(logo_surf);
    SDL_FreeSurface(splashEggSprite_surf);
    SDL_FreeSurface(exit_s);
    SDL_FreeSurface(bombSprite_surf);
    SDL_FreeSurface(homeSprite_surf);
    SDL_FreeSurface(titleSprite_surf1);
    SDL_FreeSurface(titleSprite_surf);
    SDL_FreeSurface(blue_circle_surf);
    SDL_FreeSurface(red_cross_surf);
    SDL_FreeSurface(background);
    SDL_FreeSurface(ttt_S);
    SDL_FreeSurface(wh_S);
    SDL_FreeSurface(t4_S);
    SDL_FreeSurface(blue_turn_surf);
    SDL_FreeSurface(red_cross_surf);
    SDL_FreeSurface(eggfry_cover_surf);
    SDL_FreeSurface(ttt3_cover_surf);
    SDL_FreeSurface(ttt4_cover_surf);
    SDL_FreeSurface(info_cover_surf);
    SDL_FreeSurface(credit_surf);
    SDL_FreeSurface(exit_cover_surf);

    Mix_FreeChunk(fxClick);
    Mix_FreeChunk(fxLaunch);
    Mix_FreeChunk(fxDeath);
    Mix_FreeChunk(fxCoin);
    Mix_FreeChunk(fxSplash);
    Mix_FreeChunk(fxSelect);

    Mix_CloseAudio();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}
