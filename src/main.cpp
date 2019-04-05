#include <cmath>
#include <GL/freeglut.h>

float radians_five = 0.0872665;

float cameraAngle = 0;

float eye_x = 0.0;
float eye_y = 2.0;
float eye_z = 20.0;
float look_x = eye_x + 100 * sin(cameraAngle);
float look_y = 0.0;
float look_z = eye_z - 100 * cos(cameraAngle);

void myTimer(int value)
{
    cameraAngle++;
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
    switch (key)
    {
    case GLUT_KEY_UP:
        // eye_y++;
        eye_x += 0.1 * sin(cameraAngle);
        eye_z -= 0.1 * cos(cameraAngle);
        break;
    case GLUT_KEY_DOWN:
        // eye_y--;
        eye_x -= 0.1 * sin(cameraAngle);
        eye_z += 0.1 * cos(cameraAngle);
        break;
    case GLUT_KEY_LEFT:
        cameraAngle -= radians_five;
        break;
    case GLUT_KEY_RIGHT:
        cameraAngle += radians_five;
        break;
    default:
        break;
    }

    look_x = eye_x + 100 * sin(cameraAngle);
    look_z = eye_z - 100 * cos(cameraAngle);

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
    gluLookAt(eye_x, eye_y, eye_z, look_x, look_y, look_z, 0, 1, 0);

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
