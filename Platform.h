#ifndef PLATFORM_H
#define PLATFORM_H


class Platform {
private:
    double x;
    double y;
    int width;
    int height;
    bool hasCoin;
    bool hasbomb;
    int coinX;
    int coinY;
    int bombX;
    int bombY;
    
public:
    Platform(int index);
    double getX();
    double getY();
    int getWidth();
    int getHeight();
    bool getHasCoin();
    bool getHasbomb();
    void setHasCoin(bool value);
    void setHasbomb(bool value);
    int getCoinX();
    int getCoinY();
    int getbombX();
    int getbombY();
    void updatePosition();
    void updateposition();
    
};
Platform::Platform(int index) {
    width = 100;
    height = 32;
    
    y = rand()% 440 + 220;
    x = 0 -(index * 150);
    
    int coinInt = rand()% 4;
    if(index == 0){
    	hasCoin = false;
        hasbomb = false;
    }
    else if (coinInt == 0 ){
        hasCoin = false;
        hasbomb = true;
    }
    else{
        hasCoin = true;
        hasbomb = false;
    }
    
    coinX = x - 24 - 5;
    bombX = x - 24 - 5;
    coinY = y + width/2 - 24/2;
    bombY = y + width/2 - 24/2;
}
/*Platform::clouds(int index) {
    width = 60;
    height = 40;
    
    y = rand()% 200 + 20;
    x = 0 -(index * 200);
    
}*/

double Platform::getX() {
    return x;
}

double Platform::getY() {
    return y;
}

int Platform::getWidth() {
    return width;
}

int Platform::getHeight() {
    return height;
}

bool Platform::getHasCoin() {
    return hasCoin;
}
bool Platform::getHasbomb() {
    return hasbomb;
}

void Platform::setHasCoin(bool value) {
    hasCoin = value;
}
void Platform::setHasbomb(bool value) {
    hasbomb = value;
}

int Platform::getCoinX() {
    return coinX;
}

int Platform::getCoinY() {
    return coinY;
}

int Platform::getbombX() {
    return bombX;
}

int Platform::getbombY() {
    return bombY;
}

#endif
