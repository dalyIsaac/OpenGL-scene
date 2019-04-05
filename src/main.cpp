#include <cmath>
#include <GL/freeglut.h>

int cam_hgt = 10;
float theta = 0;

void myTimer(int value)
{
    theta++;
    glutPostRedisplay();
    glutTimerFunc(50, myTimer, 0);
}

/**
 * @brief Handles keyboard callbacks.
 * 
 * @param key 
 * @param x 
 * @param y 
 */
void special(int key, int x, int y)
{
    if (key == GLUT_KEY_UP)
    {
        cam_hgt++;
    }
    else if (key == GLUT_KEY_DOWN)
    {
        cam_hgt--;
    }
    glutPostRedisplay();
}

/**
 * @brief Draws a grid of lines on the floor plane.
 * 
 */
void drawFloor()
{
    glColor3f(0., 0.5, 0.); //Floor colour

    for (int i = -50; i <= 50; i++)
    {
        glBegin(GL_LINES); //A set of grid lines on the xz-plane
        glVertex3f(-50, -0.75, i);
        glVertex3f(50, -0.75, i);
        glVertex3f(i, -0.75, -50);
        glVertex3f(i, -0.75, 50);
        glEnd();
    }
}

/**
 * @brief Contains function calls for generating the scene.
 * 
 */
void display(void)
{
    float lightPosition[4] = {0., 10., 10., 1.0};

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Camera position and orientation
    gluLookAt(12 * sin(theta), cam_hgt, 12 * cos(theta), 0, 0, 0, 0, 1, 0);

    // Sets the light's position
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    // Disables lighting when drawing the floor
    glDisable(GL_LIGHTING);
    drawFloor();

    // Enables lighting when drawing the scene
    glEnable(GL_LIGHTING);
    glColor3f(0.0, 1.0, 1.0);
    glutSolidTeapot(1.0);

    glFlush();
}

/**
 * @brief Initializes OpenGL states.
 * 
 */
void initialize(void)
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glFrustum(-5.0, 5.0, -5.0, 5.0, 10.0, 1000.0);
}

/**
 * @brief Initializes the glut window, and registers callbacks.
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("COSC363 Assignment 1");
    glutSpecialFunc(special);
    // glutTimerFunc(50, myTimer, 0);
    initialize();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
