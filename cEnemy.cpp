#include "main.h"

// Enemy Wall

c_Enemy_Wall::c_Enemy_Wall(int x, int y): c_Enemy(x, y) {
    Img = &Img_Save;
    Rct.Left = xf + Img_Offset;
    Rct.Right = Rct.Left + Img->w;
    Rct.Bottom = yf + Img_Offset;
    Rct.Top = Rct.Bottom + Img->h;
}

void c_Enemy_Wall::Init_Image() {
    Image Img_Tmp;
    Load_Texture(&Img_Tmp, "Images/Wall.png");
    Create_Image_Shadow(&Img_Tmp, &Img_Save);
    Delete_Image(&Img_Tmp);
    Img_Offset = (TILE_SIZE - Img_Save.w) / 2;
}

// Enemy Stand 1

c_Enemy_Stand_1::c_Enemy_Stand_1(int x, int y, int Drt): c_Enemy(x, y) {
    this->Drt = Drt;
    Img = &Img_Save[Drt];
    Rct.Left = xf + Img_Offset;
    Rct.Right = Rct.Left + Img->w;
    Rct.Bottom = yf + Img_Offset;
    Rct.Top = Rct.Bottom + Img->h;
}

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

// Enemy Stand 2

c_Enemy_Stand_2::c_Enemy_Stand_2(int x, int y, int Drt): c_Enemy(x, y) {
    this->Drt = Drt;
    Img = &Img_Save[Drt];
    Rct.Left = xf + Img_Offset;
    Rct.Right = Rct.Left + Img->w;
    Rct.Bottom = yf + Img_Offset;
    Rct.Top = Rct.Bottom + Img->h;
}

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

// Enemy Move 1

c_Enemy_Move_1::c_Enemy_Move_1(int x, int y, int Drt): c_Enemy(x, y) {
    this->Drt = Drt;
    Img = &Img_Save[Drt];
    Rct.Left = xf + Img_Offset;
    Rct.Right = Rct.Left + Img->w;
    Rct.Bottom = yf + Img_Offset;
    Rct.Top = Rct.Bottom + Img->h;
    Is_Move = Is_Rotate = false;
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
                if (Check_Move(x2, y2) == CAN_MOVE) {
                    Queue_Last++;
                    Queue[Queue_Last].Reload(x2, y2);
                    Drt_Back[y2][x2] = Drt2;
                }
            }
        }
    }
    return false;
}

void c_Enemy_Move_1::Action() {
    Is_Move = Is_Rotate = false;
    int Tmp = Drt_Find[0];
    Drt_Find[0] = Drt_Find[Drt];
    Drt_Find[Drt] = Tmp;
    if (BFS()) {
        if (Drt_Next != Drt) {
            Is_Rotate = true;
        } else {
            x += Drt_Offset[Drt].x;
            y += Drt_Offset[Drt].y;
            Is_Move = true;
        }
    } else {
//        Drt_Next = Drt_Find[0];
//        int H = Heuristic(x + Drt_Offset[Drt_Next].x, y + Drt_Offset[Drt_Next].y), H2;
//        for (int i = 1; i < 4; i++) {
//            H2 = Heuristic(x + Drt_Offset[Drt_Find[i]].x, y + Drt_Offset[Drt_Find[i]].y);
//            if (H > H2) {
//                H = H2;
//                Drt_Next = Drt_Find[i];
//            }
//        }
//        if (Drt_Next != Drt)
//            Is_Rotate = true;
    }
    Tmp = Drt_Find[0];
    Drt_Find[0] = Drt_Find[Drt];
    Drt_Find[Drt] = Tmp;
}

void c_Enemy_Move_1::Update_Rect() {
    Rct.Left = xf + Img_Offset;
    Rct.Right = Rct.Left + Img->w;
    Rct.Bottom = yf + Img_Offset;
    Rct.Top = Rct.Bottom + Img->h;
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

    Img_Rotate_Save[UP][DOWN] = Img_Save[RIGHT];
    Img_Rotate_Save[DOWN][UP] = Img_Save[LEFT];
    Img_Rotate_Save[RIGHT][LEFT] = Img_Save[DOWN];
    Img_Rotate_Save[LEFT][RIGHT] = Img_Save[UP];

}

// Enemy Move 2

void c_Enemy_Move_2::Init_Image() {
    Image Img_Tmp;
    Load_Texture(&Img_Tmp, "Images/Wall.png");
    Create_Image(&Img_Save, Img_Tmp.w + PIXEL_SIZE, Img_Tmp.h + PIXEL_SIZE);
    Mix_Image_Color(&Img_Save, &Img_Tmp, PIXEL_SIZE, PIXEL_SIZE, Color_Shadow);
    Mix_Image(&Img_Save, &Img_Tmp, 0, 0);
    Delete_Image(&Img_Tmp);
    Img_Offset = (TILE_SIZE - Img_Save.w) / 2;
}

// Enemy Move 4

void c_Enemy_Move_4::Init_Image() {
    Image Img_Tmp;
    Load_Texture(&Img_Tmp, "Images/Wall.png");
    Create_Image(&Img_Save, Img_Tmp.w + PIXEL_SIZE, Img_Tmp.h + PIXEL_SIZE);
    Mix_Image_Color(&Img_Save, &Img_Tmp, PIXEL_SIZE, PIXEL_SIZE, Color_Shadow);
    Mix_Image(&Img_Save, &Img_Tmp, 0, 0);
    Delete_Image(&Img_Tmp);
    Img_Offset = (TILE_SIZE - Img_Save.w) / 2;
}

// Factory


