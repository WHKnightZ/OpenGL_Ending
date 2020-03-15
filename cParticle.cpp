#include "main.h"

c_Particle::c_Particle(float x, float y, int Drt) {
    Offset = 0.0f;
    xf = x;
    yf = y;
    float Velocity = rand() % 14 / 2.0f + 5.0f;
    vx = Drt_Offset[Drt].x * Velocity;
    vy = Drt_Offset[Drt].y * Velocity;
    Img = &Img_Save;
}

void c_Particle::Update() {
    xf += vx;
    yf += vy;
    Update_Rect();
}

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
        i1 = i - 1.5f;
        Particle.push_back(new c_Particle(xf + Offset_2 * i1, yf + Offset_1 * i1, Drt));
    }
    Drt = Drt_Loop_Prev[Drt_Save];
    Offset_1 = Drt_Offset[Drt].x * 8.0f;
    Offset_2 = Drt_Offset[Drt].y * 8.0f;
    for (int i = 0; i < 4; i++) {
        i1 = i - 1.5f;
        Particle.push_back(new c_Particle(xf + Offset_2 * i1, yf + Offset_1 * i1, Drt));
    }
    Drt = Drt_Loop_Next[Drt_Save];
    Offset_1 = Drt_Offset[Drt].x * 8.0f;
    Offset_2 = Drt_Offset[Drt].y * 8.0f;
    for (int i = 0; i < 4; i++) {
        i1 = i - 1.5f;
        Particle.push_back(new c_Particle(xf - Offset_2 * i1, yf - Offset_1 * i1, Drt));
    }
}

bool c_Particle::Check_Outside() {
    return (Rct.Right < -x_Translate || Rct.Left > WIDTH - x_Translate || Rct.Top < -y_Translate || Rct.Bottom > HEIGHT - y_Translate);
}
