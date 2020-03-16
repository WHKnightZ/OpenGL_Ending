#ifndef MAIN_H
#define MAIN_H

#include "../Library/loadpng.h"
#include "../Library/process_image.h"

#include <GL/glut.h>

#include <vector>
#include <algorithm>

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

#define ITEM_PER_ROW 4
#define ITEM_PER_COLUMN 5
#define ITEM_PER_PAGE 20

#define MAX_X 30
#define MAX_Y 30

#define TILE_SIZE 48
#define TILE_HALF 24
#define PIXEL_SIZE 6

#define INTERVAL 25

// Enemy stand 2 dap. player nhu the nao??, ro rang la 2 unit
// sao enemy_move_2 lai chay ra ngoai duoc, tai sao 2 enemy cung dap dc player??
// player chet roi co can hit ko?

int POS_X, POS_Y;
int Max_X, Max_Y;
int Level_Current = 4, Max_Level;
float x_Translate, y_Translate;
Image Img_Path_Save[2][4];
Image *Img_Path[8];

int Map_Path[MAX_Y][MAX_X];
int Map[MAX_Y][MAX_X];

Rect Rct_Map[MAX_Y][MAX_X];

int Game_State;
int Enemy_Stt;

enum GAME_STATE {
    GAME_SWITCH,
    GAME_MENU,
    GAME_LEVEL,
    GAME_PLAY,
    GAME_OVER,
    GAME_RESET,
    GAME_WIN
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

// Menu

GLubyte Color_Menu_Background[] = {26, 30, 38, 255};
GLubyte Color_Select_Level[] = {214, 232, 255, 255};

Image Img_Background;
Image Img_Level[60];
Image Img_Select_Level[60], Img_Frame_On;
Image Img_Control[2][2];
Rect Rct_Background = {0, WIDTH, 0, HEIGHT};
Rect Rct_Level, Rct_Player;
Rect Rct_Select_Level[ITEM_PER_PAGE];
Rect Rct_Control[2];
int Item_Begin, Item_End;
int Item_Current = 0;
int Control_Stt = 0, Control_Type = 0;
int Page_Current = 0, Page_Max;
int Switch_Src, Switch_Dst;
float Switch_Alpha;

// Game

s_Point Drt_Offset[] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
GLubyte Color_Shadow[] = {0, 0, 0, 255};

float Offset_Forward[] = {24.0f, 16.0f, 8.0f, 0.0f, 0.0f, 0.0f};
float Offset_Back[] = {24.0f, -16.0f, -8.0f, 0.0f, 0.0f, 0.0f};

int Game_Timer = 0;

bool Mark[MAX_Y][MAX_X];
s_Point Queue[MAX_Y * MAX_X];
int Queue_Current, Queue_Last;
int Drt_Back[MAX_Y][MAX_X];
int Drt_Reverse[] = {DOWN, LEFT, UP, RIGHT};
int Drt_Loop_Prev[] = {LEFT, UP, RIGHT, DOWN};
int Drt_Loop_Next[] = {RIGHT, DOWN, LEFT, UP};

void Load_Map();
void Reload_Translate();
void Create_Image_Shadow(Image *in, Image *out);
void Hit_Enemy(int x, int y, int Drt);
int Check_Move(int x, int y);
int Heuristic(int x, int y);
void Swap(int &x, int &y);
void Reload_Page();

class c_Unit {
public:
    int x, y;
    float xf, yf, Offset;
    Rect Rct;
    Image *Img;
    int Stt, Drt;
    bool Is_Move, Is_Alive;
    int Move_Stt;

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

    float vx, vy;

    c_Particle(float x, float y, int Drt);
    void Update();
    bool Check_Outside();
};

std::vector<c_Particle *> Particle;

Image c_Particle::Img_Save;

class c_Player: public c_Unit {
public:
    static Image Img_Save;
    static void Init_Image();
    
    float xfbg, yfbg, *o, obg;

    void Init(int x, int y);
    void Move(int Drt);
    void Update();
};

Image c_Player::Img_Save;

c_Player Player;

class c_Enemy: public c_Unit {
public:
    int Heu;

    c_Enemy(int x, int y) {
        this->x = x;
        this->y = y;
        this->xf = x * TILE_SIZE;
        this->yf = y * TILE_SIZE;
        this->Stt = 0;
        Heu = Heuristic(x, y);
    }
    virtual void Action() {}
    virtual void Update() {}
};

bool Enemy_Compare(c_Enemy *e1, c_Enemy *e2) {
    return e1->Heu < e2->Heu;
}

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

	int x1,y1;
	
    c_Enemy_Stand_1(int x, int y, int Drt);
    void Action();
    void Update();
};

Image c_Enemy_Stand_1::Img_Save[4];
float c_Enemy_Stand_1::Img_Offset;

class c_Enemy_Stand_2: public c_Enemy {
public:
    static Image Img_Save[2];
    static float Img_Offset;
    static void Init_Image();
    
    int x1,y1,x2,y2;
    int Drt_Next;

    c_Enemy_Stand_2(int x, int y, int Drt);
    void Action();
    void Update();
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

    int Drt_Next;
    bool Is_Rotate;

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

    c_Factory(int x, int y, int Drt);
};

Image c_Factory::Img_Save;
float c_Factory::Img_Offset;

std::vector<c_Enemy *> Enemy;
std::vector<c_Enemy *> Enemy_Spawn;

class c_Factory_Move_1: public c_Factory {
public:
    c_Factory_Move_1(int x, int y, int Drt);
    void Action();
};

class c_Factory_Move_2: public c_Factory {
public:
    c_Factory_Move_2(int x, int y, int Drt);
    void Action();
};

class c_Factory_Move_4: public c_Factory {
public:
    c_Factory_Move_4(int x, int y, int Drt);
    void Action();
};

// Prototype

// main.h
// afunc.h
void Game_Display_Switch();
void Game_Display_Menu();
void Game_Display_Level();
void Game_Display_Play();
void Game_Display_Over();
void Game_Display_Reset();
void Game_Display_Win();
void Game_Process_Switch();
void Game_Process_Menu();
void Game_Process_Level();
void Game_Process_Play();
void Game_Process_Over();
void Game_Process_Reset();
void Game_Process_Win();
void Game_Action_Switch();
void Game_Action_Menu();
void Game_Action_Level();
void Game_Action_Play();
void Game_Action_Over();
void Game_Action_Reset();
void Game_Action_Win();
void Game_Keyboard_None(GLubyte &key);
void Game_Keyboard_Menu(GLubyte &key);
void Game_Keyboard_Play(GLubyte &key);
void Game_Special_None(int &key);
void Game_Special_Menu(int &key);
void Game_Special_Play(int &key);
// Function_Pointer

// Variable

void (*Game_Display_Func[])() = {Game_Display_Switch, Game_Display_Menu, Game_Display_Level, Game_Display_Play, Game_Display_Over, Game_Display_Reset, Game_Display_Win};
void (*Game_Process_Func[])() = {Game_Process_Switch, Game_Process_Menu, Game_Process_Level, Game_Process_Play, Game_Process_Over, Game_Process_Reset, Game_Process_Win};
void (*Game_Action_Func[])()={Game_Action_Switch,Game_Action_Menu,Game_Action_Level,Game_Action_Play,Game_Action_Over,Game_Action_Reset,Game_Action_Win};
void (*Game_Keyboard_Func[])(GLubyte &key) = {Game_Keyboard_None, Game_Keyboard_Menu, Game_Keyboard_None, Game_Keyboard_Play, Game_Keyboard_None,Game_Keyboard_None, Game_Keyboard_None};
void (*Game_Special_Func[])(int &key) = {Game_Special_None, Game_Special_Menu, Game_Special_None, Game_Special_Play, Game_Special_None,Game_Special_None, Game_Special_None};

// including all referenced .c files, you don't need to compile all of them
#include "afunc.cpp"
#include "init.cpp"
#include "cParticle.cpp"
#include "cPlayer.cpp"
#include "cEnemy.cpp"
#include "cFactory.cpp"

#endif
