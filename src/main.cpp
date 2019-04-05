#include <cmath>
#include <GL/freeglut.h>

float radians_five = 0.0872665;

float camera_angle = 0;

float eye_x = 0.0;
float eye_y = 40.0;
float eye_z = 70.0;
float look_x = eye_x + 100 * sin(camera_angle);
float look_y = 0.0;
float look_z = eye_z - 100 * cos(camera_angle);

float tower_height = 35.0;
float wall_height = 30.0;
float wall_thickness = 6.0;

void myTimer(int value)
{
    camera_angle++;
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
        eye_x += 0.5 * sin(camera_angle);
        eye_z -= 0.5 * cos(camera_angle);
        break;
    case GLUT_KEY_DOWN:
        eye_x -= 0.5 * sin(camera_angle);
        eye_z += 0.5 * cos(camera_angle);
        break;
    case GLUT_KEY_LEFT:
        camera_angle -= radians_five;
        break;
    case GLUT_KEY_RIGHT:
        camera_angle += radians_five;
        break;
    case GLUT_KEY_PAGE_UP:
        eye_y++;
        break;
    case GLUT_KEY_PAGE_DOWN:
        eye_y--;
        break;
    default:
        break;
    }

    look_x = eye_x + 100 * sin(camera_angle);
    look_z = eye_z - 100 * cos(camera_angle);

    glutPostRedisplay();
}

/**
 * @brief Draws a grid of lines on the floor plane.
 * 
 */
void drawFloor()
{
    glColor3f(0., 0.5, 0.); //Floor colour

    for (int i = -100; i <= 100; i++)
    {
        glBegin(GL_LINES); //A set of grid lines on the xz-plane
        glVertex3f(-100, -0.75, i);
        glVertex3f(100, -0.75, i);
        glVertex3f(i, -0.75, -100);
        glVertex3f(i, -0.75, 100);
        glEnd();
    }
}

void castle()
{
    // Left column
    glPushMatrix();
    glTranslatef(20.0, 0.0, 0.0);
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    glutSolidCylinder(5.0, tower_height, 100.0, 100.0);
    glPopMatrix();

    // Left tower roof
    glPushMatrix();
    glTranslatef(20.0, tower_height, 0.0);
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    glutSolidCone(7.0, 10.0, 100.0, 100.0);
    glPopMatrix();

    // Right column
    glPushMatrix();
    glTranslatef(-20.0, 0.0, 0.0);
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    glutSolidCylinder(5.0, tower_height, 100.0, 100.0);
    glPopMatrix();

    // Right tower roof
    glPushMatrix();
    glTranslatef(-20.0, tower_height, 0.0);
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    glutSolidCone(7.0, 10.0, 100.0, 100.0);
    glPopMatrix();

    // Left front wall
    glPushMatrix();
    glTranslatef(-10.0, wall_height / 2, 0.0);
    glScalef(10.0, wall_height, wall_thickness);
    glutSolidCube(1.0);
    glPopMatrix();

    // Right front wall
    glPushMatrix();
    glTranslatef(10.0, wall_height / 2, 0.0);
    glScalef(10.0, wall_height, wall_thickness);
    glutSolidCube(1.0);
    glPopMatrix();

    // Front wall top
    glPushMatrix();
    glTranslatef(0.0, 3 * wall_height / 4, 0.0);
    glScalef(10.0, wall_height / 2, wall_thickness);
    glutSolidCube(1.0);
    glPopMatrix();
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

    castle();

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
    glutInitWindowSize(1000, 1000);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("COSC363 Assignment 1");
    glutSpecialFunc(special);
    // glutTimerFunc(50, myTimer, 0);
    initialize();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
