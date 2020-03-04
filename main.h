#ifndef MAIN_H
#define MAIN_H

#include "../Library/loadpng.h"
#include "../Library/process_image.h"

#include <GL/glut.h>

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
int Level=1;
float x,y;
Image Img_Path_Save[2][4];
Image *Img_Path[8];

int Map_Path[MAX_Y][MAX_X];
int Map[MAX_Y][MAX_X];

Rect Rct_Map[MAX_Y][MAX_X];

enum GAME_STATE {
    GAME_STT
};

enum PATH{
	WALL,
	SPACE,
	EXIT
};

enum DIRECTION{
	UP,
	RIGHT,
	DOWN,
	LEFT
};

struct s_Point{
	int x,y;
};

s_Point Drt_Offset[]={{0,-1},{1,0},{0,1},{-1,0}};

void Reload_Translate();

class c_Player{
public:
	static Image Img;
	static float Offset0[];
	static float Offset1[];
	static void Init_Image();
	
	int x,y, Stt,Drt;
	float xf,yf,xfbg,yfbg,*o,obg;
	bool Is_Move, Can_Move;
	Rect Rct;
	
	void Init(int x, int y){
		this->x=x;
		this->y=y;
		Is_Move=false;
		xf=x*TILE_SIZE;
		yf=y*TILE_SIZE;
		xfbg=xf;
		yfbg=yf;
		Update_Rect();
	}
	bool Check_Move(){
		int x1=x+Drt_Offset[Drt].x;
		int y1=y+Drt_Offset[Drt].y;
		if (Map[y1][x1]!=WALL)
			return true;
		return false;
	}
	void Move(int Drt){
		if (!Is_Move){
			Is_Move=true;
			this->Drt=Drt;
			Stt=0;
			if (Check_Move()){
				Can_Move=true;
				o=Offset0;
				obg=8.0f;
			}else{
				Can_Move=false;
				o=Offset1;
				obg=0.0f;
			}
		}
	}
	void Update_Rect(){
		Rct.Left=xf;
		Rct.Right=xf+TILE_SIZE;
		Rct.Bottom=yf;
		Rct.Top=yf+TILE_SIZE;
	}
	void Draw(){
		Map_Texture(&Img);
		Draw_Rect(&Rct);
	}
	void Action(){
		if (Is_Move){
			if (Stt<6){
				xf+=Drt_Offset[Drt].x**o;
				yf+=Drt_Offset[Drt].y**o;
				xfbg+=Drt_Offset[Drt].x*obg;
				yfbg+=Drt_Offset[Drt].y*obg;
				Stt++;
				o++;
			}else{
				if (Can_Move){
					x+=Drt_Offset[Drt].x;
					y+=Drt_Offset[Drt].y;
				}
				Is_Move=false;
			}
			Update_Rect();
			Reload_Translate();
		}
	}
};

float c_Player::Offset0[]={24.0f,16.0f,8.0f,0.0f,0.0f,0.0f};
float c_Player::Offset1[]={24.0f,-16.0f,-8.0f,0.0f,0.0f,0.0f};
Image c_Player::Img;

void c_Player::Init_Image(){
	unsigned char Color_Shadow[]={0,0,0,255};
	Image Img_Tmp;
	Load_Texture(&Img_Tmp,"Images/Player.png");
	Create_Image(&Img,TILE_SIZE,TILE_SIZE);
	Mix_Image_Color(&Img,&Img_Tmp,PIXEL_SIZE,PIXEL_SIZE,Color_Shadow);
	Mix_Image(&Img,&Img_Tmp,0,0);
	Delete_Image(&Img_Tmp);
}

c_Player Player;

class Enemy{
public:
	int x,y;	
};

class Enemy_0:public Enemy{
	
};

// Prototype

// main.h

// Function_Pointer

// Variable

// including all referenced .c files, you don't need to compile all of them
#include "afunc.cpp"
#include "init.cpp"

#endif
