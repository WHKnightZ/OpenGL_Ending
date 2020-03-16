#include "main.h"

void Swap(int &x, int &y) {
    int Tmp = x;
    x = y;
    y = Tmp;
}

int Abs(int x) {
    return x > 0 ? x : -x;
}

// Menu

void Reload_Page() {
    Item_Begin = Page_Current * ITEM_PER_PAGE;
    Item_End = (Page_Current == Page_Max ? Max_Level : (Item_Begin + ITEM_PER_PAGE - 1));
}

void Init_Menu() {
    Level_Current = 0;
    char Str[40];
    FILE *f;
    while(true) {
        sprintf(Str, "Maps/%02d.txt", Level_Current);
        f = fopen(Str, "r");
        if (f == NULL)
            break;
        Level_Current++;
        fclose(f);
    }
    Max_Level = Level_Current - 1;
    Page_Max = Max_Level / ITEM_PER_PAGE;
    Image Img_Num[10];
    for(int i = 0; i < 10; i++) {
        sprintf(Str, "Images/Menu/%d.png", i);
        Load_Texture(&Img_Num[i], Str);
    }
    Create_Image_Color(&Img_Background, 1, 1, Color_Menu_Background);
    Image Img_Frame_Off;
    Load_Texture(&Img_Frame_Off, "Images/Menu/Frame_Off.png");
    Load_Texture(&Img_Frame_On, "Images/Menu/Frame_On.png");
    int a, b;
    int Start_X = 2 * PIXEL_SIZE, Start_X_2 = 6 * PIXEL_SIZE, Start_Y = Start_X;
    int Level_Width = 11 * PIXEL_SIZE, Level_Height = 5 * PIXEL_SIZE;
    for (int i = 0; i <= Max_Level; i++) {
        Create_Image(&Img_Level[i], Level_Width, Level_Height);
        a = i / 10;
        b = i % 10;
        Mix_Image(&Img_Level[i], &Img_Num[a], 0, 0);
        Mix_Image(&Img_Level[i], &Img_Num[b], Start_X_2, 0);
        Clone_Image(&Img_Frame_Off, &Img_Select_Level[i]);
        Mix_Image(&Img_Select_Level[i], &Img_Level[i], Start_X, Start_Y);
    }
    Rect *p;
    float Item_Width = Img_Frame_Off.w, Item_Height = Img_Frame_Off.h;
    float Item_Width_Extra = Item_Width + PIXEL_SIZE, Item_Height_Extra = Item_Height + PIXEL_SIZE;
    float Full_Width = Item_Width_Extra * ITEM_PER_ROW - PIXEL_SIZE;
    float Full_Height = Item_Height_Extra * ITEM_PER_COLUMN - PIXEL_SIZE;
    float Begin_X = (WIDTH - Full_Width) / 2;
    float Begin_Y = (HEIGHT - Full_Height) / 2 - 10.0f;
    for (int i = 0; i < ITEM_PER_PAGE; i++) {
        p = &Rct_Select_Level[i];
        a = i % ITEM_PER_ROW;
        b = i / ITEM_PER_ROW;
        p->Left = Begin_X + a * Item_Width_Extra;
        p->Right = p->Left + Item_Width;
        p->Bottom = Begin_Y + b * Item_Height_Extra;
        p->Top = p->Bottom + Item_Height;
    }
    Load_Texture(&Img_Control[0][0], "Images/Menu/Prev_Off.png");
    Load_Texture(&Img_Control[0][1], "Images/Menu/Prev_On.png");
    Load_Texture(&Img_Control[1][0], "Images/Menu/Next_Off.png");
    Load_Texture(&Img_Control[1][1], "Images/Menu/Next_On.png");
    Rct_Control[0].Bottom = Rct_Control[1].Bottom = Begin_Y + Full_Height + 3 * PIXEL_SIZE;
    Rct_Control[0].Top = Rct_Control[1].Top = Rct_Control[0].Bottom + Img_Control[0][0].h;
    Rct_Control[0].Left = CENTER_X - Full_Width / 4 - Img_Control[0][0].w / 2;
    Rct_Control[0].Right = Rct_Control[0].Left + Img_Control[0][0].w;
    Rct_Control[1].Left = CENTER_X + Full_Width / 4 - Img_Control[0][0].w / 2;
    Rct_Control[1].Right = Rct_Control[1].Left + Img_Control[0][0].w;

    for (int i = 0; i < 10; i++)
        Delete_Image(&Img_Num[i]);
    Reload_Page();
    Rct_Level.Left = (WIDTH - Img_Level[0].w) / 2;
    Rct_Level.Right = Rct_Level.Left + Img_Level[0].w;
    Rct_Level.Bottom = (HEIGHT - Img_Level[0].h) / 2;
    Rct_Level.Top = Rct_Level.Bottom + Img_Level[0].h;
}

// Game

void Game_Over(int &x, int &y, int &Drt){
	c_Particle::Create_Explode(x, y, Drt);
	Player.Is_Alive=false;
	Game_Timer = 0;
    Game_State = GAME_OVER;
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
    sprintf(Str, "Maps/%02d.txt", Level_Current);
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
            fscanf(f, "%d%d%d", &x, &y, &Drt);
            Enemy.push_back(new c_Enemy_Move_4(x, y, Drt));
            break;
        case 6:
        	fscanf(f, "%d%d%d", &x, &y, &Drt);
            Enemy.push_back(new c_Factory_Move_1(x, y, Drt));
            break;
        case 7:
        	fscanf(f, "%d%d%d", &x, &y, &Drt);
            Enemy.push_back(new c_Factory_Move_2(x, y, Drt));
            break;
        case 8:
        	fscanf(f, "%d%d%d", &x, &y, &Drt);
            Enemy.push_back(new c_Factory_Move_4(x, y, Drt));
            break;
        }
    }
    fclose(f);
}

void Reload_Translate() {
    x_Translate = CENTER_X - (int)(Player.xfbg + TILE_HALF);
    y_Translate = CENTER_Y - (int)(Player.yfbg + TILE_HALF);
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
    c_Factory::Init_Image();
    Rct_Player.Left = CENTER_X - c_Player::Img_Save.w / 2;
    Rct_Player.Right = Rct_Player.Left + c_Player::Img_Save.w;
    Rct_Player.Bottom = CENTER_Y - c_Player::Img_Save.h / 2;
    Rct_Player.Top = Rct_Player.Bottom + c_Player::Img_Save.h;
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

    Init_Menu();
    Game_State = GAME_MENU;
}
