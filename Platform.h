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



#endif
