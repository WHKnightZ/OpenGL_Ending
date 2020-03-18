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
    Load_Texture(&Img_Tmp, "Images/Game/Factory.png");
    Create_Image_Shadow(&Img_Tmp, &Img_Save);
    Delete_Image(&Img_Tmp);
    Img_Offset = (TILE_SIZE - Img_Save.w) / 2;
}

bool c_Factory::Check_Move(int x, int y) {
    if (Map[y][x] != SPACE)
        return false;
    if (x == Player.x && y == Player.y)
        return false;
    std::vector<c_Enemy *>::iterator i = Enemy.begin();
    while (i != Enemy.end()) {
        if ((*i)->x == x && (*i)->y == y)
            return false;
        i++;
    }
    return true;
}

// Factory_Move_1

c_Factory_Move_1::c_Factory_Move_1(int x, int y, int Drt): c_Factory(x, y, Drt) {
}

bool c_Factory_Move_1::Action() {
    bool Check = false;
    int x_Next = x + Drt_Offset[Drt].x, y_Next = y + Drt_Offset[Drt].y;
    if (Check_Move(x_Next, y_Next)) {
        Check = true;
        c_Enemy_Move_1 *e = new c_Enemy_Move_1(x, y, Drt);
        e->Is_Move = true;
        e->x = x_Next;
        e->y = y_Next;
        Enemy_Spawn.push_back(e);
    }
    return Check;
}

// Factory_Move_2

c_Factory_Move_2::c_Factory_Move_2(int x, int y, int Drt): c_Factory(x, y, Drt) {
}

bool c_Factory_Move_2::Action() {
    bool Check = false;
    int x_Next = x + Drt_Offset[Drt].x, y_Next = y + Drt_Offset[Drt].y;
    if (Check_Move(x_Next, y_Next)) {
        Check = true;
        c_Enemy_Move_2 *e = new c_Enemy_Move_2(x, y, Drt);
        e->Is_Move = true;
        e->x = x_Next;
        e->y = y_Next;
        Enemy_Spawn.push_back(e);
    }
    return Check;
}

// Factory_Move_4

c_Factory_Move_4::c_Factory_Move_4(int x, int y, int Drt): c_Factory(x, y, Drt) {
}

bool c_Factory_Move_4::Action() {
    bool Check = false;
    int x_Next = x + Drt_Offset[Drt].x, y_Next = y + Drt_Offset[Drt].y;
    if (Check_Move(x_Next, y_Next)) {
        Check = true;
        c_Enemy_Move_4 *e = new c_Enemy_Move_4(x, y, Drt);
        e->Is_Move = true;
        e->x = x_Next;
        e->y = y_Next;
        Enemy_Spawn.push_back(e);
    }
    return Check;
}
