#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_timer.h>

typedef struct {
    double x;
    double y;
} Vector2;

class Player {
private:
    double x;
    double y;
    int width;
    int height;
    bool onPlatform;
    Vector2 velocity;
    
public:
    Player(double x, double y, int width, int height);
    double getX();
    double getY();
    void setX(int x);
    void setY(int y);
    int getWidth();
    int getHeight();
    bool isOnGround();
    bool isOnPlatform();
    void setOnPlatform(bool result);
    void setVelocity(double x, double y);
    Vector2 getVelocity();
    void updatePosition();
};

// clouds cloud[4]={{0}, {1}, {2}, {3}};




#endif
