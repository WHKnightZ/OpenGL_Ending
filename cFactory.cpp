#include "main.h"

c_Factory::c_Factory(int x, int y, int Drt): c_Enemy(x, y) {
    Heu = 100;
    Img = &Img_Save;
    Offset = Img_Offset;
    this->Drt = Drt;
    Update_Rect();
}

void c_Factory::Init_Image() {
    Image Img_Tmp;
    Load_Texture(&Img_Tmp, "Images/Factory.png");
    Create_Image_Shadow(&Img_Tmp, &Img_Save);
    Delete_Image(&Img_Tmp);
    Img_Offset = (TILE_SIZE - Img_Save.w) / 2;
}

// Factory_Move_1

c_Factory_Move_1::c_Factory_Move_1(int x, int y, int Drt): c_Factory(x, y, Drt) {
}

void c_Factory_Move_1::Action() {
    int x_Next = x + Drt_Offset[Drt].x, y_Next = y + Drt_Offset[Drt].y;
    if (Check_Move(x_Next, y_Next) == CAN_MOVE) {
        c_Enemy_Move_1 *e = new c_Enemy_Move_1(x, y, Drt);
        e->Is_Move = true;
        e->x = x_Next;
        e->y = y_Next;
        Enemy_Spawn.push_back(e);
    }
}

// Factory_Move_2

c_Factory_Move_2::c_Factory_Move_2(int x, int y, int Drt): c_Factory(x, y, Drt) {
}

void c_Factory_Move_2::Action() {
    int x_Next = x + Drt_Offset[Drt].x, y_Next = y + Drt_Offset[Drt].y;
    if (Check_Move(x_Next, y_Next) == CAN_MOVE) {
        c_Enemy_Move_2 *e = new c_Enemy_Move_2(x, y, Drt);
        e->Is_Move = true;
        e->x = x_Next;
        e->y = y_Next;
        Enemy_Spawn.push_back(e);
    }
}

// Factory_Move_4

c_Factory_Move_4::c_Factory_Move_4(int x, int y, int Drt): c_Factory(x, y, Drt) {
}

void c_Factory_Move_4::Action() {
    int x_Next = x + Drt_Offset[Drt].x, y_Next = y + Drt_Offset[Drt].y;
    if (Check_Move(x_Next, y_Next) == CAN_MOVE) {
        c_Enemy_Move_4 *e = new c_Enemy_Move_4(x, y, Drt);
        e->Is_Move = true;
        e->x = x_Next;
        e->y = y_Next;
        Enemy_Spawn.push_back(e);
    }
}
