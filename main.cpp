#include "main.h"

void Display() {
    glClear(GL_COLOR_BUFFER_BIT);
    Game_Display_Func[Game_State]();
    glutSwapBuffers();
}

void Resize(int x, int y) {
    glutPositionWindow(POS_X, POS_Y);
    glutReshapeWindow(WIDTH, HEIGHT);
}

void Keyboard(GLubyte key, int x, int y) {
    Game_Keyboard_Func[Game_State](key);
}

void Special(int key, int x, int y) {
    Game_Special_Func[Game_State](key);
}

void Timer(int value) {
    Game_Process_Func[Game_State]();
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
    glutKeyboardFunc(Keyboard);
    glutSpecialFunc(Special);
    glutTimerFunc(0, Timer, 0);
    glutMainLoop();
    return 0;
}
