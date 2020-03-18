#include "main.h"

void c_Player::Init(int x, int y) {
    Offset = 0.0f;
    Is_Alive = true;
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

void c_Player::Game_Over(int &x, int &y, int &Drt) {
    Play_Sound(Sound_Lose);
    c_Particle::Create_Explode(x, y, Drt);
    this->x = 100;
    this->y = 100;
    Is_Alive = false;
    Game_Timer = 0;
    Game_State = GAME_OVER;
}

void c_Player::Move(int Drt) {
    if (!Is_Move) {
        Is_Move = true;
        this->Drt = Drt;
        Stt = 0;
        int x_Next = x + Drt_Offset[Drt].x, y_Next = y + Drt_Offset[Drt].y;
        Move_Stt = Check_Move(x_Next, y_Next);
        if (Move_Stt != CAN_MOVE) {
            if (Move_Stt == OBSTACLE_ENEMY) {
                Play_Sound(Sound_Hit_Enemy);
                Hit_Enemy(x_Next, y_Next, Drt);
            } else
                Play_Sound(Sound_Hit_Wall);
            o = Offset_Back;
            obg = 0.0f;
        } else {
            Play_Sound(Sound_Move);
            o = Offset_Forward;
            obg = 8.0f;
        }
    }
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
            if (Move_Stt == CAN_MOVE) {
                x += Drt_Offset[Drt].x;
                y += Drt_Offset[Drt].y;
                if (Map[y][x] == EXIT) {
                    Play_Sound(Sound_Win);
                    Level_Current++;
                    Game_Timer = 0;
                    Switch_Alpha = 0.0f;
                    Switch_Src = Game_State;
                    Switch_Dst = GAME_LEVEL;
                    Game_State = GAME_SWITCH;
                }
            }
            if (Move_Stt != OBSTACLE_WALL) {
                bool Check = false;
                std::sort(Enemy.begin(), Enemy.end(), Enemy_Compare);
                for(c_Enemy *enemy : Enemy) {
                    Check |= enemy->Action();
                }
                if (Check) {
                    Turn = TURN_ENEMY;
                    Enemy_Stt = 0;
                    Enemy.insert(Enemy.end(), Enemy_Spawn.begin(), Enemy_Spawn.end());
                    Enemy_Spawn.clear();
                }
            }
            Is_Move = false;
        }
        Update_Rect();
        Reload_Translate();
    }
}

void c_Player::Init_Image() {
    Image Img_Tmp;
    Load_Texture(&Img_Tmp, "Images/Game/Player.png");
    Create_Image_Shadow(&Img_Tmp, &Img_Save);
    Delete_Image(&Img_Tmp);
}

int c_Player::Check_Move(int x, int y) {
    if (Map[y][x] == WALL)
        return OBSTACLE_WALL;
    std::vector<c_Enemy *>::iterator i = Enemy.begin();
    while (i != Enemy.end()) {
        if ((*i)->x == x && (*i)->y == y)
            return OBSTACLE_ENEMY;
        i++;
    }
    return CAN_MOVE;
}
