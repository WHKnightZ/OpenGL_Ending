#include "main.h"

void Swap(int &x, int &y) {
    int Tmp = x;
    x = y;
    y = Tmp;
}

int Abs(int x) {
    return x > 0 ? x : -x;
}

bool Check_Wall_Outside(int &x, int &y) {
    if (x == 0 || y == 0 || x == Max_X - 1 || y == Max_Y - 1)
        return true;
    if (Map[y - 1][x] == -1 || Map[y][x + 1] == -1 || Map[y + 1][x] == -1 || Map[y][x - 1] == -1)
        return true;
    if (Map[y - 1][x - 1] == -1 || Map[y - 1][x + 1] == -1 || Map[y + 1][x + 1] == -1 || Map[y + 1][x - 1] == -1)
        return true;
    return false;
}

void Init_Image_Path() {
    unsigned char Color_BG[2][4] = {{40, 40, 40, 255}, {58, 58, 58, 255}};
    unsigned char Color_Wall[] = {155, 155, 155, 255};
    unsigned char Color_Shadow[] = {0, 0, 0, 255};
    Image Img_Wall, Img_Pixel, Img_Exit, Img_Shadow;
    Load_Texture(&Img_Wall, "Images/Wall.png");
    Load_Texture(&Img_Pixel, "Images/Pixel.png");
    Load_Texture(&Img_Exit, "Images/Exit.png");
    for (int i = 0; i < 4; i++) {
        Create_Image_Color(&Img_Path_Save[0][i], TILE_SIZE, TILE_SIZE, Color_BG[0]);
        Create_Image_Color(&Img_Path_Save[1][i], TILE_SIZE, TILE_SIZE, Color_BG[1]);
    }
    int Start, Start_Shadow;
    Start = (TILE_SIZE - (Img_Wall.w + PIXEL_SIZE)) / 2;
    Start_Shadow = Start + PIXEL_SIZE;
    Create_Image_Color(&Img_Shadow, Img_Wall.w, Img_Wall.h, Color_Shadow);
    for (int i = 0; i < 2; i++) {
        Mix_Image_Color(&Img_Path_Save[i][1], &Img_Shadow, Start_Shadow, Start_Shadow, Color_Shadow);
        Mix_Image_Color(&Img_Path_Save[i][1], &Img_Wall, Start, Start, Color_Wall);
        Mix_Image_Color(&Img_Path_Save[i][1], &Img_Pixel, Start + 2 * PIXEL_SIZE, Start + 2 * PIXEL_SIZE, Color_Wall);
        Mix_Image_Color(&Img_Path_Save[i][2], &Img_Wall, Start_Shadow, Start_Shadow, Color_Shadow);
        Mix_Image_Color(&Img_Path_Save[i][2], &Img_Wall, Start, Start, Color_Wall);
        Mix_Image_Color(&Img_Path_Save[i][3], &Img_Exit, PIXEL_SIZE, PIXEL_SIZE, Color_Shadow);
        Mix_Image(&Img_Path_Save[i][3], &Img_Exit, 0, 0);
    }
    for (int i = 0; i < 8; i++)
        Img_Path[i] = &Img_Path_Save[i / 4][i % 4];
    Delete_Image(&Img_Wall);
    Delete_Image(&Img_Pixel);
    Delete_Image(&Img_Exit);
    Delete_Image(&Img_Shadow);
}

void Create_Image_Shadow(Image *in, Image *out) {
    Create_Image(out, in->w + PIXEL_SIZE, in->h + PIXEL_SIZE);
    Mix_Image_Color(out, in, PIXEL_SIZE, PIXEL_SIZE, Color_Shadow);
    Mix_Image(out, in, 0, 0);
}

int Check_Move(int x, int y) {
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

void Hit_Enemy(int x, int y, int Drt) {
    std::vector<c_Enemy *>::iterator i = Enemy.begin();
    while (i != Enemy.end()) {
        if ((*i)->x == x && (*i)->y == y) {
            c_Particle::Create_Explode(x, y, Drt);
            // delete first
            i = Enemy.erase(i);
            return;
        } else
            i++;
    }
}

void Load_Map() {
    char Str[30];
    sprintf(Str, "Maps/%02d.txt", Level);
    FILE *f = fopen(Str, "r");
    int n;
    fscanf(f, "%d%d", &Max_X, &Max_Y);
    for (int i = 0; i < Max_Y; i++) {
        for (int j = 0; j < Max_X; j++) {
            fscanf(f, "%d", &Map[i][j]);
            switch(Map[i][j]) {
            case 1:
                n = 0;
                break;
            case 0:
                if (Check_Wall_Outside(j, i))
                    n = 1;
                else
                    n = 2;
                break;
            case 2:
                n = 3;
            }
            if (Map[i][j] != -1)
                Map_Path[i][j] = ((i + j) % 2) * 4 + n;
            else
                Map_Path[i][j] = -1;
        }
    }
    int Enemy_Count, Enemy_Type, x, y, Drt;
    fscanf(f, "%d%d", &x, &y);
    Player.Init(x, y);
    fscanf(f, "%d", &Enemy_Count);
    for (int i = 0; i < Enemy_Count; i++) {
        fscanf(f, "%d", &Enemy_Type);
        switch(Enemy_Type) {
        case 0:
            fscanf(f, "%d%d", &x, &y);
            Enemy.push_back(new c_Enemy_Wall(x, y));
            break;
        case 1:
            fscanf(f, "%d%d%d", &x, &y, &Drt);
            Enemy.push_back(new c_Enemy_Stand_1(x, y, Drt));
            break;
        case 2:
            fscanf(f, "%d%d%d", &x, &y, &Drt);
            Enemy.push_back(new c_Enemy_Stand_2(x, y, Drt));
            break;
        case 3:
            fscanf(f, "%d%d%d", &x, &y, &Drt);
            Enemy.push_back(new c_Enemy_Move_1(x, y, Drt));
            break;
        case 4:
            fscanf(f, "%d%d%d", &x, &y, &Drt);
            Enemy.push_back(new c_Enemy_Move_2(x, y, Drt));
            break;
        case 5:
            fscanf(f, "%d%d", &x, &y);
            Enemy.push_back(new c_Enemy_Move_4(x, y));
            break;
        }
    }
    fclose(f);
}

void Reload_Translate() {
    x = CENTER_X - (Player.xfbg + TILE_HALF);
    y = CENTER_Y - (Player.yfbg + TILE_HALF);
}

int Heuristic(int x, int y) {
    return Abs(x - Player.x) + Abs(y - Player.y);
}

void Init_Game() {
    Rect *p;
    for (int i = 0; i < MAX_Y; i++) {
        for (int j = 0; j < MAX_X; j++) {
            p = &Rct_Map[i][j];
            p->Left = j * TILE_SIZE;
            p->Right = p->Left + TILE_SIZE;
            p->Bottom = i * TILE_SIZE;
            p->Top = p->Bottom + TILE_SIZE;
        }
    }
    Init_Image_Path();

    c_Particle::Init_Image();
    c_Player::Init_Image();
    c_Enemy_Wall::Init_Image();
    c_Enemy_Stand_1::Init_Image();
    c_Enemy_Stand_2::Init_Image();
    c_Enemy_Move_1::Init_Image();
    c_Enemy_Move_2::Init_Image();
    c_Enemy_Move_4::Init_Image();

    Load_Map();

    Reload_Translate();
}

void Init_GL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, WIDTH, HEIGHT);
    gluOrtho2D(0, WIDTH, HEIGHT, 0);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glutIgnoreKeyRepeat(GL_TRUE);
    glEnable(GL_TEXTURE_2D);
}
