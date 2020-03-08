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
int Level = 1;
float x, y;
Image Img_Path_Save[2][4];
Image *Img_Path[8];

int Map_Path[MAX_Y][MAX_X];
int Map[MAX_Y][MAX_X];

Rect Rct_Map[MAX_Y][MAX_X];

int Enemy_Stt;

enum GAME_STATE {
    GAME_STT
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

enum AXIS{
    VERTICAL,
    HORIZONTAL
};

enum TURN {
    TURN_PLAYER,
    TURN_ENEMY
};

enum MOVE_STATE{
	CAN_MOVE,
	OBSTACLE_WALL,
	OBSTACLE_ENEMY
};

int Turn = TURN_PLAYER;

struct s_Point {
    int x, y;
};

s_Point Drt_Offset[] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
GLubyte Color_Shadow[] = {0, 0, 0, 255};

float Offset_Forward[] = {24.0f, 16.0f, 8.0f, 0.0f, 0.0f, 0.0f};
float Offset_Back[] = {24.0f, -16.0f, -8.0f, 0.0f, 0.0f, 0.0f};

void Reload_Translate();
void Create_Image_Shadow(Image *in, Image *out);
void Hit_Enemy(int x, int y);
int Heuristic(int &x, int &y);

class c_Player {
public:
    static Image Img_Save;
    static void Init_Image();

    int x, y, Stt, Drt;
    float xf, yf, xfbg, yfbg, *o, obg;
    bool Is_Move;
    int Move_Stt;
    Image *Img;
    Rect Rct;

    void Init(int x, int y);
    void Move(int Drt);
    void Update_Rect();
    void Draw();
    void Update();
};

Image c_Player::Img_Save;

void c_Player::Init_Image() {
    Image Img_Tmp;
    Load_Texture(&Img_Tmp, "Images/Player.png");
    Create_Image_Shadow(&Img_Tmp, &Img_Save);
    Delete_Image(&Img_Tmp);
}

c_Player Player;

class c_Enemy {
public:
    int x, y, Stt, Drt;
    float xf, yf;
    Image *Img;
    Rect Rct;

    c_Enemy(int x, int y) {
        this->x = x;
        this->y = y;
        this->xf = x * TILE_SIZE;
        this->yf = y * TILE_SIZE;
        this->Stt = 0;
    }
    virtual void Draw() {
        Map_Texture(Img);
        Draw_Rect(&Rct);
    }
    virtual void Action() {}
    virtual void Update() {}
};

class c_Enemy_Wall: public c_Enemy {
public:
    static Image Img_Save;
    static float Img_Offset;
    static void Init_Image();

    c_Enemy_Wall(int x, int y): c_Enemy(x, y) {
        Img = &Img_Save;
        Rct.Left = xf + Img_Offset;
        Rct.Right = Rct.Left + Img->w;
        Rct.Bottom = yf + Img_Offset;
        Rct.Top = Rct.Bottom + Img->h;
    }
};

Image c_Enemy_Wall::Img_Save;
float c_Enemy_Wall::Img_Offset;

void c_Enemy_Wall::Init_Image() {
    Image Img_Tmp;
    Load_Texture(&Img_Tmp, "Images/Wall.png");
    Create_Image_Shadow(&Img_Tmp, &Img_Save);
    Delete_Image(&Img_Tmp);
    Img_Offset = (TILE_SIZE - Img_Save.w) / 2;
}

class c_Enemy_Stand_1: public c_Enemy {
public:
    static Image Img_Save[4];
    static float Img_Offset;
    static void Init_Image();

    int Drt;

    c_Enemy_Stand_1(int x, int y, int Drt): c_Enemy(x, y) {
        this->Drt = Drt;
        Img = &Img_Save[Drt];
        Rct.Left = xf + Img_Offset;
        Rct.Right = Rct.Left + Img->w;
        Rct.Bottom = yf + Img_Offset;
        Rct.Top = Rct.Bottom + Img->h;
    }
};

Image c_Enemy_Stand_1::Img_Save[4];
float c_Enemy_Stand_1::Img_Offset;

void c_Enemy_Stand_1::Init_Image() {
    Image Img_Tmp, Img_Tmp_2;
    Load_Texture(&Img_Tmp, "Images/Enemy_Stand_1.png");
    Create_Image_Shadow(&Img_Tmp, &Img_Save[UP]);
    Rotate_Left(&Img_Tmp, &Img_Tmp_2);
    Create_Image_Shadow(&Img_Tmp_2, &Img_Save[LEFT]);
    Delete_Image(&Img_Tmp_2);
    Rotate_Right(&Img_Tmp, &Img_Tmp_2);
    Create_Image_Shadow(&Img_Tmp_2, &Img_Save[RIGHT]);
    Delete_Image(&Img_Tmp_2);
    Rotate_180(&Img_Tmp, &Img_Tmp_2);
    Create_Image_Shadow(&Img_Tmp_2, &Img_Save[DOWN]);
    Delete_Image(&Img_Tmp_2);
    Img_Offset = (TILE_SIZE - Img_Save[UP].w) / 2;
    Delete_Image(&Img_Tmp);
}

class c_Enemy_Stand_2: public c_Enemy {
public:
    static Image Img_Save[2];
    static float Img_Offset;
    static void Init_Image();

    int Drt;

    c_Enemy_Stand_2(int x, int y, int Drt): c_Enemy(x, y) {
        this->Drt = Drt;
        Img = &Img_Save[Drt];
        Rct.Left = xf + Img_Offset;
        Rct.Right = Rct.Left + Img->w;
        Rct.Bottom = yf + Img_Offset;
        Rct.Top = Rct.Bottom + Img->h;
    }
};

Image c_Enemy_Stand_2::Img_Save[2];
float c_Enemy_Stand_2::Img_Offset;

void c_Enemy_Stand_2::Init_Image() {
    Image Img_Tmp, Img_Tmp_2;
    Load_Texture(&Img_Tmp, "Images/Enemy_Stand_2.png");
    Create_Image_Shadow(&Img_Tmp, &Img_Save[VERTICAL]);
    Rotate_Right(&Img_Tmp, &Img_Tmp_2);
    Create_Image_Shadow(&Img_Tmp_2, &Img_Save[HORIZONTAL]);
    Delete_Image(&Img_Tmp_2);
    Img_Offset = (TILE_SIZE - Img_Save[VERTICAL].w) / 2;
    Delete_Image(&Img_Tmp);
}

class c_Enemy_Move_1: public c_Enemy {
public:
    static Image Img_Save[4];
    static Image Img_Rotate_Save[4][4];
    static float Img_Offset;
    static void Init_Image();

    int Drt, Drt_Next;
    bool Is_Move,Is_Rotate;

    c_Enemy_Move_1(int x, int y, int Drt): c_Enemy(x, y) {
        this->Drt = Drt;
        Img = &Img_Save[Drt];
        Rct.Left = xf + Img_Offset;
        Rct.Right = Rct.Left + Img->w;
        Rct.Bottom = yf + Img_Offset;
        Rct.Top = Rct.Bottom + Img->h;
        Is_Move=Is_Rotate=false;
    }
    
    void Action(){
    	Drt_Next=Drt+1;
    	if (Drt_Next==4)
    		Drt_Next=0;
    	Is_Rotate=true;
	}
	void Update(){
		if (Is_Rotate){
			if (Enemy_Stt==3){
				Img=&Img_Rotate_Save[Drt][Drt_Next];
			}else if (Enemy_Stt==5){
				Drt=Drt_Next;
				Img=&Img_Save[Drt];
			}
		}
	}
};

Image c_Enemy_Move_1::Img_Save[4];
Image c_Enemy_Move_1::Img_Rotate_Save[4][4];
float c_Enemy_Move_1::Img_Offset;

void c_Enemy_Move_1::Init_Image() {
    Image Img_Tmp, Img_Tmp_2;
    Load_Texture(&Img_Tmp, "Images/Enemy_Move_1.png");
    Create_Image_Shadow(&Img_Tmp, &Img_Save[UP]);
    Rotate_Left(&Img_Tmp, &Img_Tmp_2);
    Create_Image_Shadow(&Img_Tmp_2, &Img_Save[LEFT]);
    Delete_Image(&Img_Tmp_2);
    Rotate_Right(&Img_Tmp, &Img_Tmp_2);
    Create_Image_Shadow(&Img_Tmp_2, &Img_Save[RIGHT]);
    Delete_Image(&Img_Tmp_2);
    Rotate_180(&Img_Tmp, &Img_Tmp_2);
    Create_Image_Shadow(&Img_Tmp_2, &Img_Save[DOWN]);
    Delete_Image(&Img_Tmp_2);
    Img_Offset = (TILE_SIZE - Img_Save[VERTICAL].w) / 2;
    Delete_Image(&Img_Tmp);

    Load_Texture(&Img_Tmp, "Images/Enemy_Move_1_Rotate.png");
    Create_Image_Shadow(&Img_Tmp, &Img_Rotate_Save[UP][RIGHT]);
    Rotate_Left(&Img_Tmp, &Img_Tmp_2);
    Create_Image_Shadow(&Img_Tmp_2, &Img_Rotate_Save[LEFT][UP]);
    Delete_Image(&Img_Tmp_2);
    Rotate_Right(&Img_Tmp, &Img_Tmp_2);
    Create_Image_Shadow(&Img_Tmp_2, &Img_Rotate_Save[RIGHT][DOWN]);
    Delete_Image(&Img_Tmp_2);
    Rotate_180(&Img_Tmp, &Img_Tmp_2);
    Create_Image_Shadow(&Img_Tmp_2, &Img_Rotate_Save[DOWN][LEFT]);
    Delete_Image(&Img_Tmp_2);
    Delete_Image(&Img_Tmp);
    Img_Rotate_Save[RIGHT][UP] = Img_Rotate_Save[UP][RIGHT];
    Img_Rotate_Save[UP][LEFT] = Img_Rotate_Save[LEFT][UP];
    Img_Rotate_Save[DOWN][RIGHT] = Img_Rotate_Save[RIGHT][DOWN];
    Img_Rotate_Save[LEFT][DOWN] = Img_Rotate_Save[DOWN][LEFT];

}

class c_Enemy_Move_2: public c_Enemy {
public:
    static Image Img_Save;
    static float Img_Offset;
    static void Init_Image();
};

Image c_Enemy_Move_2::Img_Save;
float c_Enemy_Move_2::Img_Offset;

void c_Enemy_Move_2::Init_Image() {
    Image Img_Tmp;
    Load_Texture(&Img_Tmp, "Images/Wall.png");
    Create_Image(&Img_Save, Img_Tmp.w + PIXEL_SIZE, Img_Tmp.h + PIXEL_SIZE);
    Mix_Image_Color(&Img_Save, &Img_Tmp, PIXEL_SIZE, PIXEL_SIZE, Color_Shadow);
    Mix_Image(&Img_Save, &Img_Tmp, 0, 0);
    Delete_Image(&Img_Tmp);
    Img_Offset = (TILE_SIZE - Img_Save.w) / 2;
}

class c_Enemy_Move_4: public c_Enemy {
public:
    static Image Img_Save;
    static float Img_Offset;
    static void Init_Image();
};

Image c_Enemy_Move_4::Img_Save;
float c_Enemy_Move_4::Img_Offset;

void c_Enemy_Move_4::Init_Image() {
    Image Img_Tmp;
    Load_Texture(&Img_Tmp, "Images/Wall.png");
    Create_Image(&Img_Save, Img_Tmp.w + PIXEL_SIZE, Img_Tmp.h + PIXEL_SIZE);
    Mix_Image_Color(&Img_Save, &Img_Tmp, PIXEL_SIZE, PIXEL_SIZE, Color_Shadow);
    Mix_Image(&Img_Save, &Img_Tmp, 0, 0);
    Delete_Image(&Img_Tmp);
    Img_Offset = (TILE_SIZE - Img_Save.w) / 2;
}

template<class T>
class c_Enemy_Factory: public c_Enemy {
public:
    static Image Img_Save;
    static float Img_Offset;
    static void Init_Image();
};

//Image c_Enemy_Factory::Img_Save;
//float c_Enemy_Factory::Img_Offset;

//void c_Enemy_Factory::Init_Image(){
//	Image Img_Tmp;
//    Load_Texture(&Img_Tmp, "Images/Wall.png");
//    Create_Image(&Img_Save, Img_Tmp.w+PIXEL_SIZE, Img_Tmp.h+PIXEL_SIZE);
//    Mix_Image_Color(&Img_Save, &Img_Tmp, PIXEL_SIZE, PIXEL_SIZE, Color_Shadow);
//    Mix_Image(&Img_Save, &Img_Tmp, 0, 0);
//    Delete_Image(&Img_Tmp);
//    Img_Offset=(TILE_SIZE-Img_Save.w)/2;
//}

std::vector<c_Enemy *> Enemy;

// Prototype

// main.h

// Function_Pointer

// Variable

// including all referenced .c files, you don't need to compile all of them
#include "afunc.cpp"
#include "init.cpp"
#include "cPlayer.cpp"
#include "cEnemy.cpp"

#endif
