#include "Player.h"

#include <math.h>
#include <vector>
//extern double zz;
//extern int flag;



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







