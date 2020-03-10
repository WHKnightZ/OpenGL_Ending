#include "main.h"

void Display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(x, y, 0.0f);
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
    Player.Draw();
    glutSwapBuffers();
}

void Resize(int x, int y) {
    glutPositionWindow(POS_X, POS_Y);
    glutReshapeWindow(WIDTH, HEIGHT);
}

void Keyboard(GLubyte key, int x, int y) {
}

void Special(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP:
        Player.Move(UP);
        break;
    case GLUT_KEY_RIGHT:
        Player.Move(RIGHT);
        break;
    case GLUT_KEY_DOWN:
        Player.Move(DOWN);
        break;
    case GLUT_KEY_LEFT:
        Player.Move(LEFT);
        break;
    }
}

void Timer(int value) {
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
        if ((*i)->Check_Outside()){
            i = Particle.erase(i);
        } else
            i++;
    }
    glutPostRedisplay();
    glutTimerFunc(INTERVAL, Timer, 0);
}

int main(int argc, char **argv) {
    Init_Game();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    POS_X = (glutGet(GLUT_SCREEN_WIDTH) - WIDTH) >> 1;
    POS_Y = (glutGet(GLUT_SCREEN_HEIGHT) - HEIGHT) >> 1;
    glutInitWindowPosition(POS_X, POS_Y);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Ending");
    Init_GL();
    glutDisplayFunc(Display);
    glutSpecialFunc(Special);
    glutTimerFunc(0, Timer, 0);
    glutMainLoop();
    return 0;
}
