#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_timer.h>
#include "Platform.h"

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
Player::Player(double x, double y, int width, int height) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    onPlatform = false;
}
double Player::getX() {
    return x;
}

double Player::getY() {
    return y;
}

void Player::setX(int x) {
    this->x = x;
}

void Player::setY(int y) {
    this->y = y;
}

int Player::getWidth() {
    return width;
}

int Player::getHeight() {
    return height;
}

bool Player::isOnGround() {
    return onPlatform;
}

bool Player::isOnPlatform() {
    return onPlatform;
}

void Player::setOnPlatform(bool result) {
    onPlatform = result;
}

void Player::setVelocity(double x, double y) {
    velocity = (Vector2){x, y};
}

Vector2 Player::getVelocity() {
    return velocity;
}


Platform platforms[8] = {{0}, {1}, {2}, {3}, {4}, {5}, {6}, {7}};
// clouds cloud[4]={{0}, {1}, {2}, {3}};
Player player(platforms[0].getX() + platforms[0].getWidth() / 2 - 26 / 2, platforms[0].getY() - player.getHeight(), 26, 32);




#endif
