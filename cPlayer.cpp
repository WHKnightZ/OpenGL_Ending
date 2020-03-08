#include "main.h"

void c_Player::Init(int x, int y) {
    this->x = x;
    this->y = y;
    Is_Move = false;
    xf = x * TILE_SIZE;
    yf = y * TILE_SIZE;
    xfbg = xf;
    yfbg = yf;
    Img = &Img_Save;
    Update_Rect();
}

void c_Player::Move(int Drt) {
    if (!Is_Move) {
        Is_Move = true;
        this->Drt = Drt;
        Stt = 0;
        int x_Next=x+Drt_Offset[Drt].x,y_Next=y+Drt_Offset[Drt].y;
        Move_Stt=Check_Move(x_Next,y_Next);
        if (Move_Stt!=CAN_MOVE) {
            if (Move_Stt==OBSTACLE_ENEMY)
                Hit_Enemy(x_Next, y_Next);
            o = Offset_Back;
            obg = 0.0f;
        } else {
            o = Offset_Forward;
            obg = 8.0f;
        }
    }
}

void c_Player::Update_Rect() {
    Rct.Left = xf;
    Rct.Right = xf + TILE_SIZE;
    Rct.Bottom = yf;
    Rct.Top = yf + TILE_SIZE;
}

void c_Player::Draw() {
    Map_Texture(Img);
    Draw_Rect(&Rct);
}

void c_Player::Update() {
    if (Is_Move) {
        if (Stt < 6) {
            xf += Drt_Offset[Drt].x**o;
            yf += Drt_Offset[Drt].y**o;
            xfbg += Drt_Offset[Drt].x * obg;
            yfbg += Drt_Offset[Drt].y * obg;
            Stt++;
            o++;
        } else {
            if (Move_Stt==CAN_MOVE) {
                x += Drt_Offset[Drt].x;
                y += Drt_Offset[Drt].y;
            }
			if (Move_Stt!=OBSTACLE_WALL){
            	Turn = TURN_ENEMY;
	            Enemy_Stt = 0;
	            for(c_Enemy *enemy : Enemy) {
	                enemy->Action();
	            }
			}
            Is_Move = false;
        }
        Update_Rect();
        Reload_Translate();
    }
}
