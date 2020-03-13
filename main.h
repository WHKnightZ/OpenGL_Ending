#ifndef MAIN_H
#define MAIN_H

#include "../Library/loadpng.h"
#include "../Library/process_image.h"

#include <GL/glut.h>

#include <vector>

#include "../Library/gl_texture.h"

/*
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
*/

#define WIDTH 600
#define HEIGHT 600
#define CENTER_X 300.0f
#define CENTER_Y 300.0f

#define MAX_X 30
#define MAX_Y 30

#define TILE_SIZE 48
#define TILE_HALF 24
#define PIXEL_SIZE 6

#define INTERVAL 25

int POS_X, POS_Y;
int Max_X, Max_Y;
int Level = 3;
float x_Translate, y_Translate;
Image Img_Path_Save[2][4];
Image *Img_Path[8];

int Map_Path[MAX_Y][MAX_X];
int Map[MAX_Y][MAX_X];

Rect Rct_Map[MAX_Y][MAX_X];

int Enemy_Stt;

enum GAME_STATE {
    GAME_STT_MENU,
    GAME_STT_PLAY,
    GAME_STT_OVER
};

enum PATH {
    WALL,
    SPACE,
    EXIT
};

enum DIRECTION {
    UP,
    RIGHT,
    DOWN,
    LEFT
};

enum AXIS {
    VERTICAL,
    HORIZONTAL
};

enum TURN {
    TURN_PLAYER,
    TURN_ENEMY
};

enum MOVE_STATE {
    CAN_MOVE,
    OBSTACLE_WALL,
    OBSTACLE_ENEMY
};

int Turn = TURN_PLAYER;

struct s_Point {
    int x, y;
    void Reload(int x, int y) {
        this->x = x;
        this->y = y;
    }
};

s_Point Drt_Offset[] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
GLubyte Color_Shadow[] = {0, 0, 0, 255};

float Offset_Forward[] = {24.0f, 16.0f, 8.0f, 0.0f, 0.0f, 0.0f};
float Offset_Back[] = {24.0f, -16.0f, -8.0f, 0.0f, 0.0f, 0.0f};

bool Mark[MAX_Y][MAX_X];
s_Point Queue[MAX_Y * MAX_X];
int Queue_Current, Queue_Last;
int Drt_Back[MAX_Y][MAX_X];
int Drt_Reverse[] = {DOWN, LEFT, UP, RIGHT};
int Drt_Loop_Prev[] = {LEFT, UP, RIGHT, DOWN};
int Drt_Loop_Next[] = {RIGHT, DOWN, LEFT, UP};

void Reload_Translate();
void Create_Image_Shadow(Image *in, Image *out);
void Hit_Enemy(int x, int y, int Drt);
int Check_Move(int x, int y);
int Heuristic(int x, int y);
void Swap(int &x, int &y);

class c_Unit {
public:
    int x, y;
    float xf, yf, Offset;
    Rect Rct;
    Image *Img;
    int Stt, Drt;

    virtual void Update_Rect() {
        Rct.Left = xf + Offset;
        Rct.Right = Rct.Left + Img->w;
        Rct.Bottom = yf + Offset;
        Rct.Top = Rct.Bottom + Img->h;
    }
    virtual void Draw() {
        Map_Texture(Img);
        Draw_Rect(&Rct);
    }
};

class c_Particle: public c_Unit {
public:
    static Image Img_Save;
    static void Init_Image();
    static void Create_Explode(int x, int y, int Drt);
    bool Check_Outside();

    float vx, vy;

    c_Particle(float x, float y, int Drt) {
        Offset = 0.0f;
        xf = x;
        yf = y;
        float Velocity= rand()%14/2.0f + 5.0f;
        vx = Drt_Offset[Drt].x * Velocity;
        vy = Drt_Offset[Drt].y * Velocity;
        Img = &Img_Save;
    }
    void Update() {
        xf += vx;
        yf += vy;
        Update_Rect();
    }
};

std::vector<c_Particle *> Particle;

Image c_Particle::Img_Save;

void c_Particle::Init_Image() {
    Image Img_Tmp;
    Load_Texture(&Img_Tmp, "Images/Pixel.png");
    Create_Image_Shadow(&Img_Tmp, &Img_Save);
    Delete_Image(&Img_Tmp);
}

void c_Particle::Create_Explode(int x, int y, int Drt) {
    int Drt_Save = Drt;
    float xf = x * TILE_SIZE + TILE_HALF, yf = y * TILE_SIZE + TILE_HALF;
    float Offset_1, Offset_2;
    Offset_1 = Drt_Offset[Drt].x * 8.0f;
    Offset_2 = Drt_Offset[Drt].y * 8.0f;
    float i1;
    for (int i = 0; i < 4; i++) {
    	i1=i-1.5f;
        Particle.push_back(new c_Particle(xf + Offset_2 * i1, yf + Offset_1 * i1, Drt));
    }
    Drt = Drt_Loop_Prev[Drt_Save];
    Offset_1 = Drt_Offset[Drt].x * 8.0f;
    Offset_2 = Drt_Offset[Drt].y * 8.0f;
    for (int i = 0; i < 4; i++) {
    	i1=i-1.5f;
        Particle.push_back(new c_Particle(xf + Offset_2 * i1, yf + Offset_1 * i1, Drt));
    }
    Drt = Drt_Loop_Next[Drt_Save];
    Offset_1 = Drt_Offset[Drt].x * 8.0f;
    Offset_2 = Drt_Offset[Drt].y * 8.0f;
    for (int i = 0; i < 4; i++) {
    	i1=i-1.5f;
        Particle.push_back(new c_Particle(xf - Offset_2 * i1, yf - Offset_1 * i1, Drt));
    }
}

bool c_Particle::Check_Outside() {
	return (Rct.Right<-x_Translate||Rct.Left>WIDTH-x_Translate||Rct.Top<-y_Translate||Rct.Bottom>HEIGHT-y_Translate);
}

class c_Player: public c_Unit {
public:
    static Image Img_Save;
    static void Init_Image();

    float xfbg, yfbg, *o, obg;
    bool Is_Move, Is_Alive;
    int Move_Stt;

    void Init(int x, int y);
    void Move(int Drt);
    void Update();
};

Image c_Player::Img_Save;

c_Player Player;

class c_Enemy: public c_Unit {
public:
    c_Enemy(int x, int y) {
        this->x = x;
        this->y = y;
        this->xf = x * TILE_SIZE;
        this->yf = y * TILE_SIZE;
        this->Stt = 0;
    }
    virtual void Action() {}
    virtual void Update() {}
};

class c_Enemy_Wall: public c_Enemy {
public:
    static Image Img_Save;
    static float Img_Offset;
    static void Init_Image();

    c_Enemy_Wall(int x, int y);
};

Image c_Enemy_Wall::Img_Save;
float c_Enemy_Wall::Img_Offset;

class c_Enemy_Stand_1: public c_Enemy {
public:
    static Image Img_Save[4];
    static float Img_Offset;
    static void Init_Image();

    c_Enemy_Stand_1(int x, int y, int Drt);
};

Image c_Enemy_Stand_1::Img_Save[4];
float c_Enemy_Stand_1::Img_Offset;

class c_Enemy_Stand_2: public c_Enemy {
public:
    static Image Img_Save[2];
    static float Img_Offset;
    static void Init_Image();

    c_Enemy_Stand_2(int x, int y, int Drt);
};

Image c_Enemy_Stand_2::Img_Save[2];
float c_Enemy_Stand_2::Img_Offset;

class c_Enemy_Move_1: public c_Enemy {
public:
    static Image Img_Save[4];
    static Image Img_Rotate_Save[4][4];
    static float Img_Offset;
    static int Drt_Find[4];
    static int Drt_Max;
    static void Init_Image();

    int Drt, Drt_Next;
    bool Is_Move, Is_Rotate;

    c_Enemy_Move_1(int x, int y, int Drt);
    bool BFS();
    void Action();
    void Update();
};

Image c_Enemy_Move_1::Img_Save[4];
Image c_Enemy_Move_1::Img_Rotate_Save[4][4];
float c_Enemy_Move_1::Img_Offset;
int c_Enemy_Move_1::Drt_Find[4] = {UP, RIGHT, DOWN, LEFT};
int c_Enemy_Move_1::Drt_Max = 4;

class c_Enemy_Move_2: public c_Enemy {
public:
    static Image Img_Save[4];
    static float Img_Offset;
    static int Drt_Find[2][2];
    static int Drt_Max;
    static int Drt_Map[4];
    static int Drt_Map_Axis[4];
    static void Init_Image();

    bool Is_Move;

    c_Enemy_Move_2(int x, int y, int Drt);
    void Action();
    void Update();
};

Image c_Enemy_Move_2::Img_Save[4];
float c_Enemy_Move_2::Img_Offset;
int c_Enemy_Move_2::Drt_Find[2][2] = {{UP, DOWN}, {RIGHT, LEFT}};
int c_Enemy_Move_2::Drt_Max = 2;
int c_Enemy_Move_2::Drt_Map[4] = {0, 0, 1, 1};
int c_Enemy_Move_2::Drt_Map_Axis[4] = {VERTICAL, HORIZONTAL, VERTICAL, HORIZONTAL};

class c_Enemy_Move_4: public c_Enemy {
public:
    static Image Img_Save;
    static float Img_Offset;
    static int Drt_Find[4];
    static int Drt_Max;
    static void Init_Image();

    bool Is_Move;

    c_Enemy_Move_4(int x, int y, int Drt);
    bool BFS();
    void Action();
    void Update();
};

Image c_Enemy_Move_4::Img_Save;
float c_Enemy_Move_4::Img_Offset;
int c_Enemy_Move_4::Drt_Find[4] = {UP, RIGHT, DOWN, LEFT};
int c_Enemy_Move_4::Drt_Max = 4;

class c_Factory: public c_Enemy {
public:
    static Image Img_Save;
    static float Img_Offset;
    static void Init_Image();

    bool Is_Create;

    c_Factory(int x, int y, int Drt): c_Enemy(x, y) {
        Img = &Img_Save;
        Offset = Img_Offset;
        this->Drt = Drt;
        Update_Rect();
    }
};

Image c_Factory::Img_Save;
float c_Factory::Img_Offset;

void c_Factory::Init_Image() {
    Image Img_Tmp;
    Load_Texture(&Img_Tmp, "Images/Factory.png");
    Create_Image_Shadow(&Img_Tmp, &Img_Save);
    Delete_Image(&Img_Tmp);
    Img_Offset = (TILE_SIZE - Img_Save.w) / 2;
}

std::vector<c_Enemy *> Enemy;
std::vector<c_Enemy *> Enemy_Create;

class c_Factory_Move_1: public c_Factory {
public:
	
    c_Factory_Move_1(int x, int y, int Drt): c_Factory(x, y, Drt) {
    }
    void Action() {
        int x_Next = x + Drt_Offset[Drt].x, y_Next = y + Drt_Offset[Drt].y;
        if (Check_Move(x_Next, y_Next) == CAN_MOVE) {
            c_Enemy_Move_1 *e = new c_Enemy_Move_1(x, y, Drt);
            e->Is_Move = true;
            e->x = x_Next;
            e->y = y_Next;
            Enemy_Create.push_back(e);
        }
    }
    void Update() {

    }
};

class c_Factory_Move_2: public c_Factory {
public:
	
    c_Factory_Move_2(int x, int y, int Drt): c_Factory(x, y, Drt) {
    }
    void Action() {
        int x_Next = x + Drt_Offset[Drt].x, y_Next = y + Drt_Offset[Drt].y;
        if (Check_Move(x_Next, y_Next) == CAN_MOVE) {
            c_Enemy_Move_2 *e = new c_Enemy_Move_2(x, y, Drt);
            e->Is_Move = true;
            e->x = x_Next;
            e->y = y_Next;
            Enemy_Create.push_back(e);
        }
    }
    void Update() {

    }
};

class c_Factory_Move_4: public c_Factory {
public:
	
    c_Factory_Move_4(int x, int y, int Drt): c_Factory(x, y, Drt) {
    }
    void Action() {
        int x_Next = x + Drt_Offset[Drt].x, y_Next = y + Drt_Offset[Drt].y;
        if (Check_Move(x_Next, y_Next) == CAN_MOVE) {
            c_Enemy_Move_4 *e = new c_Enemy_Move_4(x, y, Drt);
            e->Is_Move = true;
            e->x = x_Next;
            e->y = y_Next;
            Enemy_Create.push_back(e);
        }
    }
    void Update() {

    }
};

// Prototype

// main.h

// Function_Pointer

// Variable

// including all referenced .c files, you don't need to compile all of them
#include "afunc.cpp"
#include "init.cpp"
#include "cPlayer.cpp"
#include "cEnemy.cpp"
#include "cFactory.cpp"

#endif
