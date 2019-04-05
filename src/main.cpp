#include <GL/freeglut.h>
#include <climits>
#include <cmath>
#include <fstream>
#include <iostream>
using namespace std;

float* x;
float* y;
float* z;

int* t1;
int* t2;
int* t3;

int num_vertices;
int num_triangles;

float radians_five = 0.0872665;

float camera_angle = 0;

float eye_x = 0.0;
float eye_y = 5.0;
float eye_z = 0.0;
float look_x = eye_x + 100 * sin(camera_angle);
float look_y = 0.0;
float look_z = eye_z - 100 * cos(camera_angle);

float tower_height = 35.0;
float wall_height = 30.0;
float wall_thickness = 6.0;

float spaceship_height = 20.0;

/**
 * @brief Loads the OFF mesh file.
 *
 * @param fname
 */
void loadMeshFile(const char* fname)
{
    ifstream fp_in;
    int num, ne;

    fp_in.open(fname, ios::in);
    if (!fp_in.is_open()) {
        cout << "Error opening mesh file" << endl;
        exit(1);
    }

    fp_in.ignore(INT_MAX, '\n'); // ignore first line
    fp_in >> num_vertices >> num_triangles >>
        ne; // read number of vertices, polygons, edges

    x = new float[num_vertices]; // create arrays
    y = new float[num_vertices];
    z = new float[num_vertices];

    t1 = new int[num_triangles];
    t2 = new int[num_triangles];
    t3 = new int[num_triangles];

    for (int i = 0; i < num_vertices; i++) // read vertex list
        fp_in >> x[i] >> y[i] >> z[i];

    for (int i = 0; i < num_triangles; i++) // read polygon list
    {
        fp_in >> num >> t1[i] >> t2[i] >> t3[i];
        if (num != 3) {
            cout << "ERROR: Polygon with index " << i << " is not a triangle."
                 << endl; // not a triangle!!
            exit(1);
        }
    }

    fp_in.close();
    cout << " File successfully read." << endl;
}

/**
 * @brief Calculates the normal vector of a triangle.
 *
 * @param tindx
 */
void normal(int tindx)
{
    float x1 = x[t1[tindx]], x2 = x[t2[tindx]], x3 = x[t3[tindx]];
    float y1 = y[t1[tindx]], y2 = y[t2[tindx]], y3 = y[t3[tindx]];
    float z1 = z[t1[tindx]], z2 = z[t2[tindx]], z3 = z[t3[tindx]];
    float nx, ny, nz;
    nx = y1 * (z2 - z3) + y2 * (z3 - z1) + y3 * (z1 - z2);
    ny = z1 * (x2 - x3) + z2 * (x3 - x1) + z3 * (x1 - x2);
    nz = x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2);
    glNormal3f(nx, ny, nz);
}

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
    switch (key) {
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
    glColor3f(0., 0.5, 0.); // Floor colour

    for (int i = -100; i <= 100; i++) {
        glBegin(GL_LINES); // A set of grid lines on the xz-plane
        glVertex3f(-100, -0.75, i);
        glVertex3f(100, -0.75, i);
        glVertex3f(i, -0.75, -100);
        glVertex3f(i, -0.75, 100);
        glEnd();
    }
}

/**
 * @brief Draws the cannon.
 *
 */
void drawCannon()
{
    glColor3f(0.4, 0.5, 0.4);

    glBegin(GL_TRIANGLES);
    for (int tindx = 0; tindx < num_triangles; tindx++) {
        normal(tindx);
        glVertex3d(x[t1[tindx]], y[t1[tindx]], z[t1[tindx]]);
        glVertex3d(x[t2[tindx]], y[t2[tindx]], z[t2[tindx]]);
        glVertex3d(x[t3[tindx]], y[t3[tindx]], z[t3[tindx]]);
    }
    glEnd();
}

/**
 * @brief Draws the castle, and it's constituent supports.
 *
 */
void cannon()
{
    glPushMatrix();
    // Global transitions
    glTranslatef(0.0, 0.0, 20.0);
    glRotatef(-90.0, 0.0, 1.0, 0.0);
    glScalef(0.1, 0.1, 0.1);

    // Cannon
    glPushMatrix();
    glTranslatef(-20.0, 30.0, 0);
    glRotatef(15.0, 0, 0, 1);
    glTranslatef(20.0, -30.0, 0);
    drawCannon();
    glPopMatrix();

    // Supports
    glPushMatrix();
    glTranslatef(-10.0, 5.0, 17.0);
    glScalef(80.0, 10.0, 6.0);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-20.0, 25.0, 17.0);
    glScalef(40.0, 30.0, 6.0);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-10.0, 5.0, -17.0);
    glScalef(80.0, 10.0, 6.0);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-20.0, 25.0, -17.0);
    glScalef(40.0, 30.0, 6.0);
    glutSolidCube(1);
    glPopMatrix();

    glPopMatrix();
}

/**
 * @brief Draws a castle tower, and moves it to the given location.
 *
 * @param x Translation distance in the x-direction.
 * @param y Translation distance in the y-direction.
 * @param z Translation distance in the z-direction.
 */
void castleTower(GLfloat x = 0.0, GLfloat y = 0.0, GLfloat z = 0.0)
{
    glPushMatrix();
    glTranslatef(x, y, z);

    // Tower base
    glPushMatrix();
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    glutSolidCylinder(5.0, tower_height, 100.0, 100.0);
    glPopMatrix();

    // Tower root
    glPushMatrix();
    glTranslatef(0.0, tower_height, 0.0);
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    glutSolidCone(7.0, 10.0, 100.0, 100.0);
    glPopMatrix();

    glPopMatrix();
}

/**
 * @brief Draws a castle wall, and moves it to the given location, for a given
 * rotation.
 *
 * @param translate_x Translation distance in the x-direction.
 * @param translate_y Translation distance in the y-direction.
 * @param translate_z Translation distance in the z-direction.
 * @param angle The amount to rotate the wall by.
 * @param rotate_x x-component of the rotation vector.
 * @param rotate_y y-component of the rotation vector.
 * @param rotate_z z-component of the rotation vector.
 */
void castleWall(GLfloat translate_x = 0.0, GLfloat translate_y = 0.0,
                GLfloat translate_z = 0.0, GLfloat angle = 0.0,
                GLfloat rotate_x = 0.0, GLfloat rotate_y = 0.0,
                GLfloat rotate_z = 0.0)
{
    glPushMatrix();
    glTranslatef(translate_x, translate_y, translate_z);
    glRotatef(angle, rotate_x, rotate_y, rotate_z);

    // Left front wall
    glPushMatrix();
    glTranslatef(-12.5, wall_height / 2, 0.0);
    glScalef(15.0, wall_height, wall_thickness);
    glutSolidCube(1.0);
    glPopMatrix();

    // Right front wall
    glPushMatrix();
    glTranslatef(12.5, wall_height / 2, 0.0);
    glScalef(15.0, wall_height, wall_thickness);
    glutSolidCube(1.0);
    glPopMatrix();

    // Front wall top
    glPushMatrix();
    glTranslatef(0.0, 3 * wall_height / 4, 0.0);
    glScalef(10.0, wall_height / 2, wall_thickness);
    glutSolidCube(1.0);
    glPopMatrix();

    glPopMatrix();
}

/**
 * @brief Draws the castle.
 *
 */
void castle()
{
    // Right front tower
    castleTower(20.0);
    // Front
    castleWall();
    // Left front tower
    castleTower(-20.0);

    // Right back tower
    castleTower(20.0, 0.0, -40.0);
    // Back
    castleWall(0.0, 0.0, -40.0);
    // Left back tower
    castleTower(-20.0, 0.0, -40.0);

    // Right side wall
    castleWall(20.0, 0.0, -20.0, 90.0, 0.0, 1.0, 0.0);

    // Left side wall
    castleWall(-20.0, 0.0, -20.0, 90.0, 0.0, 1.0, 0.0);
}

void fin(GLfloat translate_x = 0.0, GLfloat translate_y = 0.0,
         GLfloat translate_z = 0.0, GLfloat angle = 0.0, GLfloat rotate_x = 0.0,
         GLfloat rotate_y = 0.0, GLfloat rotate_z = 0.0)
{
    glPushMatrix();
    // glRotatef(angle, rotate_x, rotate_y, rotate_z);
    glRotatef(angle, rotate_x, rotate_y, rotate_z);
    glTranslatef(translate_x, translate_y, translate_z);
    glBegin(GL_TRIANGLE_STRIP);
    glVertex3f(0.0, 9.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(5.0, 0.0, 0.0);
    glEnd();
    glPopMatrix();
}

/**
 * @brief Draws the spaceship.
 *
 */
void spaceship()
{
    glPushMatrix();
    glTranslatef(0.0, 0.0, -20.0);

    // Fuselage
    glPushMatrix();
    glTranslatef(0.0, 4.0, 0.0);
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    glutSolidCylinder(2.0, spaceship_height, 100.0, 100.0);
    glPopMatrix();

    // Tower root
    glPushMatrix();
    glTranslatef(0.0, 4.0 + spaceship_height, 0.0);
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    glutSolidCone(2.0, 6.0, 100.0, 100.0);
    glPopMatrix();

    glPushMatrix();
    fin(2.0);
    fin(2.0, 0.0, 0.0, 120.0, 0.0, 1.0, 0.0);
    fin(2.0, 0.0, 0.0, 240.0, 0.0, 1.0, 0.0);
    glPopMatrix();

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
    cannon();
    spaceship();

    glFlush();
}

/**
 * @brief Initializes OpenGL states.
 *
 */
void initialize(void)
{
    loadMeshFile("models/Cannon.off");

    // Background color
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    // Enables OpenGL states
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
int main(int argc, char** argv)
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
