const double pi = 3.1415926535897;
const int gravity = 1;
const int screenWidth = 1200;
const int screenHeight = 800;
int LoadHighScore();
int scoreInt = 0;
int highscoreInt = LoadHighScore();
char score[32];
char highscore[32];
char hs[32];

bool titleScreen = true;
bool playCoinFX = false;
int cnt = 0;
int mouseDownX = 0;
int mouseDownY = 0;
double lavaY = screenHeight - 32;
double timer = 0;
double splashTimer = 0;
double SplashTimer = 0;
int gameover = 0;
double zz = 1;
int flag = 0;

bool firstTime = true;
bool playedSplash = false;
bool playedSelect = false;
int mouse_x, mouse_y;
bool mouse_down = false;
bool mouse_released = false;
bool mouse_pressed = false;
bool TITLEScreen = true;
bool bl1 = false;
bool bl2 = false;
int arr[3];
bool amikiekefelesi = false;
int cnn = 0;
int go;

