#include "main.h"

// Display

void Game_Display_Switch() {
    glLoadIdentity();
    glColor4f(1.0f, 1.0f, 1.0f, (1.0f - Switch_Alpha));
    Game_Display_Func[Switch_Src]();
    glLoadIdentity();
    glColor4f(1.0f, 1.0f, 1.0f, Switch_Alpha);
    Game_Display_Func[Switch_Dst]();
}

void Game_Display_Menu() {
    Map_Texture(&Img_Background);
    Draw_Rect(&Rct_Background);
    for (int i = Item_Begin; i <= Item_End; i++) {
        Map_Texture(&Img_Select_Level[i]);
        Draw_Rect(&Rct_Select_Level[i % ITEM_PER_PAGE]);
    }
    Map_Texture(&Img_Control[0][Control_Stt & (1 - Control_Type)]);
    Draw_Rect(&Rct_Control[0]);
    Map_Texture(&Img_Control[1][Control_Stt & Control_Type]);
    Draw_Rect(&Rct_Control[1]);
    if (Control_Stt == 0) {
        Map_Texture(&Img_Frame_On);
        Draw_Rect(&Rct_Select_Level[Item_Current % ITEM_PER_PAGE]);
    }
}

void Game_Display_Level() {
    Map_Texture(&Img_Level[Level_Current]);
    Draw_Rect(&Rct_Level);
}

void Game_Display_Play() {
    glLoadIdentity();
    glTranslatef(x_Translate, y_Translate, 0.0f);
    for (int i = 0; i < Max_Y; i++) {
        for (int j = 0; j < Max_X; j++) {
            Map_Texture(Img_Path[Map_Path[i][j]]);
            Draw_Rect(&Rct_Map[i][j]);
        }
    }
    for(c_Enemy *enemy : Enemy) {
        enemy->Draw();
    }
    for (c_Particle *particle : Particle)
        particle->Draw();
    if (Player.Is_Alive)
        Player.Draw();
}

void Game_Display_Over() {
    glLoadIdentity();
    glTranslatef(x_Translate, y_Translate, 0.0f);
    for (int i = 0; i < Max_Y; i++) {
        for (int j = 0; j < Max_X; j++) {
            Map_Texture(Img_Path[Map_Path[i][j]]);
            Draw_Rect(&Rct_Map[i][j]);
        }
    }
    for(c_Enemy *enemy : Enemy) {
        enemy->Draw();
    }
    for (c_Particle *particle : Particle)
        particle->Draw();
    if (Player.Is_Alive)
        Player.Draw();
}

void Game_Display_Reset() {
    Map_Texture(&c_Player::Img_Save);
    Draw_Rect(&Rct_Player);
}

void Game_Display_Win() {
	Game_Display_Play();
}

// Process

void Game_Process_Switch() {
    if (Switch_Alpha <= 1.0f) {
        Switch_Alpha += 0.03f;
    } else{
    	Game_State = Switch_Dst;
    	Game_Action_Func[Game_State]();
	}
    Game_Process_Func[Switch_Src]();
}

void Game_Process_Menu() {
	glutPostRedisplay();
}

void Game_Process_Level() {
    
    glutPostRedisplay();
}

void Game_Process_Play() {
    if (Turn == TURN_PLAYER)
        Player.Update();
    else {
        for(c_Enemy *enemy : Enemy) {
            enemy->Update();
        }
        Enemy_Stt++;
        if (Enemy_Stt == 6)
            Turn = TURN_PLAYER;
    }
    std::vector<c_Particle *>::iterator i = Particle.begin();
    while (i != Particle.end()) {
        (*i)->Update();
        if ((*i)->Check_Outside()) {
            // delete first
            i = Particle.erase(i);
        } else
            i++;
    }
    glutPostRedisplay();
}

void Game_Process_Over() {
    if (Turn == TURN_PLAYER)
        Player.Update();
    else {
        for(c_Enemy *enemy : Enemy) {
            enemy->Update();
        }
        Enemy_Stt++;
        if (Enemy_Stt == 6)
            Turn = TURN_PLAYER;
    }
    std::vector<c_Particle *>::iterator i = Particle.begin();
    while (i != Particle.end()) {
        (*i)->Update();
        if ((*i)->Check_Outside()) {
            // delete first
            i = Particle.erase(i);
        } else
            i++;
    }
    if (Game_State != GAME_SWITCH) {
        if (Game_Timer < 20) {
            Game_Timer++;
        } else {
            Game_Timer = 0;
            Switch_Alpha = 0.0f;
            Switch_Src = Game_State;
            Switch_Dst = GAME_RESET;
            Game_State = GAME_SWITCH;
        }
    }
    glutPostRedisplay();
}

void Game_Process_Reset() {
    glutPostRedisplay();
}

void Game_Process_Win() {
	if (Game_State==GAME_WIN){
		if (Game_Timer<20){
			Game_Timer++;
		}else{
			Level_Current++;
			Game_Timer=0;
			Switch_Alpha = 0.0f;
	        Switch_Src = Game_State;
	        Switch_Dst = GAME_LEVEL;
	        Game_State = GAME_SWITCH;
		}
	}
	glutPostRedisplay();
}

// Action

void Game_Action_Switch(){	
}

void Game_Action_Menu(){
	
}

void Game_Action_Level(){
	for (auto i:Enemy)
		delete i;
	Enemy.clear();
    Load_Map();
    Reload_Translate();
    Switch_Alpha = 0.0f;
    Switch_Src = Game_State;
    Switch_Dst = GAME_PLAY;
    Game_State = GAME_SWITCH;
}

void Game_Action_Play(){
	
}

void Game_Action_Over(){
	
}

void Game_Action_Reset(){
	for (auto i:Enemy)
		delete i;
    Enemy.clear();
    Particle.clear();
    Load_Map();
    Reload_Translate();
    Switch_Alpha = 0.0f;
    Switch_Src = Game_State;
    Switch_Dst = GAME_PLAY;
    Game_State = GAME_SWITCH;
}

void Game_Action_Win(){
	
}

// Keyboard

void Game_Keyboard_None(GLubyte &key) {
}

void Game_Keyboard_Menu(GLubyte &key) {
    switch(key) {
    case 13:
        if (Control_Stt == 1) {
            if (Control_Type == 0) {
                Page_Current--;
                if (Page_Current < 0)
                    Page_Current = Page_Max;
                Reload_Page();
                Item_Current = Item_Begin;
            } else {
                Page_Current++;
                if (Page_Current > Page_Max)
                    Page_Current = 0;
                Reload_Page();
                Item_Current = Item_Begin;
            }
            glutPostRedisplay();
        } else {
            Level_Current = Item_Current;
            Switch_Alpha = 0.0f;
            Switch_Src = Game_State;
            Switch_Dst = GAME_LEVEL;
            Game_State = GAME_SWITCH;
        }
        break;
    }
}

void Game_Keyboard_Play(GLubyte &key) {
    switch(key) {
    case 27:
        break;
    }
}

// Special

void Game_Special_None(int &key) {
}

void Game_Special_Menu(int &key) {
    switch (key) {
    case GLUT_KEY_LEFT:
        if (Control_Stt != 1) {
            Item_Current--;
            if (Item_Current < Item_Begin)
                Item_Current = Item_End;
        } else {
            Control_Type = 1 - Control_Type;
        }
        glutPostRedisplay();
        break;
    case GLUT_KEY_RIGHT:
        if (Control_Stt != 1) {
            Item_Current++;
            if (Item_Current > Item_End)
                Item_Current = Item_Begin;
        } else {
            Control_Type = 1 - Control_Type;
        }
        glutPostRedisplay();
        break;
    case GLUT_KEY_UP:
        if (Control_Stt != 1) {
            if (Item_Current - ITEM_PER_ROW < Item_Begin) {
                Control_Stt = 1;
                Control_Type = 0;
            } else {
                Item_Current -= ITEM_PER_ROW;
            }
        } else {
            Control_Stt = 0;
            Item_Current = ITEM_PER_ROW - Item_Current % ITEM_PER_ROW;
            Item_Current = (Item_End - Item_Begin + Item_Current) / ITEM_PER_ROW * ITEM_PER_ROW - Item_Current + Item_Begin;
        }
        glutPostRedisplay();
        break;
    case GLUT_KEY_DOWN:
        if (Control_Stt != 1) {
            if (Item_Current + ITEM_PER_ROW > Item_End) {
                Control_Stt = 1;
                Control_Type = 0;
                Item_Current = Item_Current % ITEM_PER_ROW + Item_Begin;
            } else {
                Item_Current += ITEM_PER_ROW;
            }
        } else {
            Control_Stt = 0;
        }
        glutPostRedisplay();
        break;
    }
}

void Game_Special_Play(int &key) {
    switch (key) {
    case GLUT_KEY_LEFT:
        Player.Move(LEFT);
        break;
    case GLUT_KEY_RIGHT:
        Player.Move(RIGHT);
        break;
    case GLUT_KEY_UP:
        Player.Move(UP);
        break;
    case GLUT_KEY_DOWN:
        Player.Move(DOWN);
        break;
    }
}
