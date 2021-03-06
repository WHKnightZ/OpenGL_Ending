#include "main.h"

bool c_Enemy::Check_Move(int x, int y) {
    if (Map[y][x] != SPACE)
        return false;
    std::vector<c_Enemy *>::iterator i = Enemy.begin();
    while (i != Enemy.end()) {
        if ((*i)->x == x && (*i)->y == y)
            return false;
        i++;
    }
    return true;
}

// Enemy Wall

c_Enemy_Wall::c_Enemy_Wall(int x, int y): c_Enemy(x, y) {
    Offset = Img_Offset;
    Img = &Img_Save;
    Update_Rect();
}

void c_Enemy_Wall::Init_Image() {
    Image Img_Tmp;
    Load_Texture(&Img_Tmp, "Images/Game/Wall.png");
    Create_Image_Shadow(&Img_Tmp, &Img_Save);
    Delete_Image(&Img_Tmp);
    Img_Offset = (TILE_SIZE - Img_Save.w) / 2;
}

// Enemy Stand 1

c_Enemy_Stand_1::c_Enemy_Stand_1(int x, int y, int Drt): c_Enemy(x, y) {
    Is_Move = false;
    x1 = x + Drt_Offset[Drt].x;
    y1 = y + Drt_Offset[Drt].y;
    this->Drt = Drt;
    Offset = Img_Offset;
    Img = &Img_Save[Drt];
    Update_Rect();
}

bool c_Enemy_Stand_1::Action() {
    if (x1 == Player.x && y1 == Player.y) {
        Is_Move = true;
        Player.Game_Over(x1, y1, Drt);
        return true;
    }
    return false;
}

void c_Enemy_Stand_1::Update() {
    if (Is_Move) {
        if (Enemy_Stt < 6) {
            xf += Drt_Offset[Drt].x * Offset_Back[Enemy_Stt];
            yf += Drt_Offset[Drt].y * Offset_Back[Enemy_Stt];
        } else {
            Is_Move = false;
        }
        Update_Rect();
    }
}

void c_Enemy_Stand_1::Init_Image() {
    Image Img_Tmp, Img_Tmp_2;
    Load_Texture(&Img_Tmp, "Images/Game/Enemy_Stand_1.png");
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

// Enemy Stand 2

c_Enemy_Stand_2::c_Enemy_Stand_2(int x, int y, int Drt): c_Enemy(x, y) {
    Is_Move = false;
    x1 = x + Drt_Offset[Drt].x;
    y1 = y + Drt_Offset[Drt].y;
    x2 = x - Drt_Offset[Drt].x;
    y2 = y - Drt_Offset[Drt].y;
    this->Drt = Drt;
    Offset = Img_Offset;
    Img = &Img_Save[Drt];
    Update_Rect();
}

bool c_Enemy_Stand_2::Action() {
    if (x1 == Player.x && y1 == Player.y) {
        Drt_Next = Drt;
        Player.Game_Over(x1, y1, Drt_Next);
        Is_Move = true;
        return true;
    }
    if (x2 == Player.x && y2 == Player.y) {
        Drt_Next = Drt_Reverse[Drt];
        Is_Move = true;
        Player.Game_Over(x2, y2, Drt_Next);
        return true;
    }
    return false;
}

void c_Enemy_Stand_2::Update() {
    if (Is_Move) {
        if (Enemy_Stt < 6) {
            xf += Drt_Offset[Drt_Next].x * Offset_Back[Enemy_Stt];
            yf += Drt_Offset[Drt_Next].y * Offset_Back[Enemy_Stt];
        } else {
            Is_Move = false;
        }
        Update_Rect();
    }
}

void c_Enemy_Stand_2::Init_Image() {
    Image Img_Tmp, Img_Tmp_2;
    Load_Texture(&Img_Tmp, "Images/Game/Enemy_Stand_2.png");
    Create_Image_Shadow(&Img_Tmp, &Img_Save[VERTICAL]);
    Rotate_Right(&Img_Tmp, &Img_Tmp_2);
    Create_Image_Shadow(&Img_Tmp_2, &Img_Save[HORIZONTAL]);
    Delete_Image(&Img_Tmp_2);
    Img_Offset = (TILE_SIZE - Img_Save[VERTICAL].w) / 2;
    Delete_Image(&Img_Tmp);
}

// Enemy Move 1

c_Enemy_Move_1::c_Enemy_Move_1(int x, int y, int Drt): c_Enemy(x, y) {
    this->Drt = Drt;
    Is_Move = Is_Rotate = false;
    Offset = Img_Offset;
    Img = &Img_Save[Drt];
    Update_Rect();
}

bool c_Enemy_Move_1::BFS() {
    for (int i = 0; i < Max_Y; i++)
        for (int j = 0; j < Max_X; j++)
            Mark[i][j] = false;
    Queue_Current = Queue_Last = 0;
    Queue[Queue_Current].Reload(x, y);
    int x1, y1, x2, y2, Drt2;
    while (Queue_Current <= Queue_Last) {
        x1 = Queue[Queue_Current].x;
        y1 = Queue[Queue_Current].y;
        Queue_Current++;
        for (int i = 0; i < Drt_Max; i++) {
            Drt2 = Drt_Find[i];
            x2 = x1 + Drt_Offset[Drt2].x;
            y2 = y1 + Drt_Offset[Drt2].y;
            if (x2 == Player.x && y2 == Player.y) {
                Drt_Back[y2][x2] = Drt2;
                while (x2 != x || y2 != y) {
                    Drt_Next = Drt_Back[y2][x2];
                    x2 -= Drt_Offset[Drt_Next].x;
                    y2 -= Drt_Offset[Drt_Next].y;
                }
                return true;
            }
            if (!Mark[y2][x2]) {
                Mark[y2][x2] = true;
                if (Check_Move(x2, y2)) {
                    Queue_Last++;
                    Queue[Queue_Last].Reload(x2, y2);
                    Drt_Back[y2][x2] = Drt2;
                }
            }
        }
    }
    return false;
}

bool c_Enemy_Move_1::Action() {
    Is_Move = Is_Rotate = false;
    Swap(Drt_Find[0], Drt_Find[Drt]);
    if (BFS()) {
        if (Drt_Next != Drt) {
            Is_Rotate = true;
        } else {
            x += Drt_Offset[Drt].x;
            y += Drt_Offset[Drt].y;
            Heu = Heuristic(x, y);
            if (x == Player.x && y == Player.y) {
                Player.Game_Over(x, y, Drt);
            }
            Is_Move = true;
        }
    } else {
        bool Can_Move = false;
        int H, Min = 99;
        int x_Next, y_Next;
        for (int i = 0; i < Drt_Max; i++) {
            x_Next = x + Drt_Offset[Drt_Find[i]].x;
            y_Next = y + Drt_Offset[Drt_Find[i]].y;
            H = Heuristic(x_Next, y_Next);
            if (Min > H && Check_Move(x_Next, y_Next)) {
                Can_Move = true;
                Min = H;
                Drt_Next = Drt_Find[i];
            }
        }
        if (Can_Move) {
            if (Drt_Next != Drt)
                Is_Rotate = true;
            else {
                x += Drt_Offset[Drt].x;
                y += Drt_Offset[Drt].y;
                Heu = Heuristic(x, y);
                Is_Move = true;
            }
        }
    }
    Swap(Drt_Find[0], Drt_Find[Drt]);
    return (Is_Move || Is_Rotate);
}

void c_Enemy_Move_1::Update() {
    if (Is_Rotate) {
        if (Enemy_Stt == 3) {
            Img = &Img_Rotate_Save[Drt][Drt_Next];
        } else if (Enemy_Stt == 5) {
            Drt = Drt_Next;
            Img = &Img_Save[Drt];
        }
    } else if (Is_Move) {
        xf += Drt_Offset[Drt].x * Offset_Forward[Enemy_Stt];
        yf += Drt_Offset[Drt].y * Offset_Forward[Enemy_Stt];
        Update_Rect();
    }
}

void c_Enemy_Move_1::Init_Image() {
    Image Img_Tmp, Img_Tmp_2;
    Load_Texture(&Img_Tmp, "Images/Game/Enemy_Move_1.png");
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

    Load_Texture(&Img_Tmp, "Images/Game/Enemy_Move_1_Rotate.png");
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

    Img_Rotate_Save[UP][DOWN] = Img_Save[RIGHT];
    Img_Rotate_Save[DOWN][UP] = Img_Save[LEFT];
    Img_Rotate_Save[RIGHT][LEFT] = Img_Save[DOWN];
    Img_Rotate_Save[LEFT][RIGHT] = Img_Save[UP];

}

// Enemy Move 2

c_Enemy_Move_2::c_Enemy_Move_2(int x, int y, int Drt): c_Enemy(x, y) {
    this->Drt = Drt;
    Is_Move = false;
    Offset = Img_Offset;
    Img = &Img_Save[Drt];
    Update_Rect();
}

bool c_Enemy_Move_2::Action() {
    int Axis = Drt_Map_Axis[Drt];
    int Drt2 = Drt_Map[Drt];
    Is_Move = false;
    Swap(Drt_Find[Axis][0], Drt_Find[Axis][Drt2]);
    int H, Min = 99;
    int x_Next, y_Next;
    for (int i = 0; i < Drt_Max; i++) {
        x_Next = x + Drt_Offset[Drt_Find[Axis][i]].x;
        y_Next = y + Drt_Offset[Drt_Find[Axis][i]].y;
        if (Check_Move(x_Next, y_Next)) {
            Is_Move = true;
            H = Heuristic(x_Next, y_Next);
            if (H < Min) {
                Min = H;
                Drt = Drt_Find[Axis][i];
            }
        }
    }
    if (Is_Move) {
        x += Drt_Offset[Drt].x;
        y += Drt_Offset[Drt].y;
        if (x == Player.x && y == Player.y) {
            Player.Game_Over(x, y, Drt);
        }
        Heu = Heuristic(x, y);
    }
    Swap(Drt_Find[Axis][0], Drt_Find[Axis][Drt2]);
    return Is_Move;
}

void c_Enemy_Move_2::Update() {
    if (Is_Move) {
        xf += Drt_Offset[Drt].x * Offset_Forward[Enemy_Stt];
        yf += Drt_Offset[Drt].y * Offset_Forward[Enemy_Stt];
        Update_Rect();
    }
}

void c_Enemy_Move_2::Init_Image() {
    Image Img_Tmp, Img_Tmp_2;
    Load_Texture(&Img_Tmp, "Images/Game/Enemy_Move_2.png");
    Create_Image_Shadow(&Img_Tmp, &Img_Save[UP]);
    Img_Save[DOWN] = Img_Save[UP];
    Rotate_Right(&Img_Tmp, &Img_Tmp_2);
    Create_Image_Shadow(&Img_Tmp_2, &Img_Save[RIGHT]);
    Img_Save[LEFT] = Img_Save[RIGHT];
    Delete_Image(&Img_Tmp_2);
    Delete_Image(&Img_Tmp);
    Img_Offset = (TILE_SIZE - Img_Save[UP].w) / 2;
}

// Enemy Move 4

c_Enemy_Move_4::c_Enemy_Move_4(int x, int y, int Drt): c_Enemy(x, y) {
    this->Drt = Drt;
    Is_Move = false;
    Offset = Img_Offset;
    Img = &Img_Save;
    Update_Rect();
}

bool c_Enemy_Move_4::BFS() {
    for (int i = 0; i < Max_Y; i++)
        for (int j = 0; j < Max_X; j++)
            Mark[i][j] = false;
    Queue_Current = Queue_Last = 0;
    Queue[Queue_Current].Reload(x, y);
    int x1, y1, x2, y2, Drt2;
    while (Queue_Current <= Queue_Last) {
        x1 = Queue[Queue_Current].x;
        y1 = Queue[Queue_Current].y;
        Queue_Current++;
        for (int i = 0; i < Drt_Max; i++) {
            Drt2 = Drt_Find[i];
            x2 = x1 + Drt_Offset[Drt2].x;
            y2 = y1 + Drt_Offset[Drt2].y;
            if (x2 == Player.x && y2 == Player.y) {
                Drt_Back[y2][x2] = Drt2;
                while (x2 != x || y2 != y) {
                    Drt = Drt_Back[y2][x2];
                    x2 -= Drt_Offset[Drt].x;
                    y2 -= Drt_Offset[Drt].y;
                }
                return true;
            }
            if (!Mark[y2][x2]) {
                Mark[y2][x2] = true;
                if (Check_Move(x2, y2)) {
                    Queue_Last++;
                    Queue[Queue_Last].Reload(x2, y2);
                    Drt_Back[y2][x2] = Drt2;
                }
            }
        }
    }
    return false;
}

bool c_Enemy_Move_4::Action() {
    Is_Move = false;
    Swap(Drt_Find[0], Drt_Find[Drt]);
    if (BFS()) {
        x += Drt_Offset[Drt].x;
        y += Drt_Offset[Drt].y;
        Heu = Heuristic(x, y);
        if (x == Player.x && y == Player.y) {
            Player.Game_Over(x, y, Drt);
        }
        Is_Move = true;
    } else {
        bool Can_Move = false;
        int H, Min = 99;
        int x_Next, y_Next;
        for (int i = 0; i < Drt_Max; i++) {
            x_Next = x + Drt_Offset[Drt_Find[i]].x;
            y_Next = y + Drt_Offset[Drt_Find[i]].y;
            H = Heuristic(x_Next, y_Next);
            if (Min > H && Check_Move(x_Next, y_Next)) {
                Can_Move = true;
                Min = H;
                Drt = Drt_Find[i];
            }
        }
        if (Can_Move) {
            x += Drt_Offset[Drt].x;
            y += Drt_Offset[Drt].y;
            Heu = Heuristic(x, y);
            Is_Move = true;
        }
    }
    Swap(Drt_Find[0], Drt_Find[Drt]);
    return Is_Move;
}

void c_Enemy_Move_4::Update() {
    if (Is_Move) {
        xf += Drt_Offset[Drt].x * Offset_Forward[Enemy_Stt];
        yf += Drt_Offset[Drt].y * Offset_Forward[Enemy_Stt];
        Update_Rect();
    }
}

void c_Enemy_Move_4::Init_Image() {
    Image Img_Tmp;
    Load_Texture(&Img_Tmp, "Images/Game/Enemy_Move_4.png");
    Create_Image_Shadow(&Img_Tmp, &Img_Save);
    Delete_Image(&Img_Tmp);
    Img_Offset = (TILE_SIZE - Img_Save.w) / 2;
}
