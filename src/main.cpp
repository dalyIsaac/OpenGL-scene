#include "robot.h"
#include <GL/freeglut.h>
#include <climits>
#include <cmath>
#include <fstream>
#include <iostream>
using namespace std;

static double *x_cannon_mesh;
static double *y_cannon_mesh;
static double *z_cannon_mesh;

static int *t1;
static int *t2;
static int *t3;

static int num_vertices;
static int num_triangles;

static double radians_five = 0.0872665;

static double camera_angle = 0;

static double eye_x = 0.0;
static double eye_y = 20.0;
static double eye_z = 100.0;
static double look_x = 0;
static double look_y = 0.0;
static double look_z = 1;

static double tower_height = 35.0;
static double wall_height = 30.0;
static double wall_thickness = 6.0;

static bool spaceship_flying = false;
static double spaceship_height = 20.0;
static double spaceship_altitude = 0.0;

static bool ball_fired = false;
static double ball_x = 0.0;
static double ball_y = 4.8;
static double ball_z = 54.5;

/**
 * @brief Loads the OFF mesh file.
 *
 * @param fname
 */
void loadMeshFile(const char *fname) {
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

  x_cannon_mesh = new double[num_vertices]; // create arrays
  y_cannon_mesh = new double[num_vertices];
  z_cannon_mesh = new double[num_vertices];

  t1 = new int[num_triangles];
  t2 = new int[num_triangles];
  t3 = new int[num_triangles];

  for (int i = 0; i < num_vertices; i++) // read vertex list
    fp_in >> x_cannon_mesh[i] >> y_cannon_mesh[i] >> z_cannon_mesh[i];

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
void normal(int tindx) {
  double x1 = x_cannon_mesh[t1[tindx]], x2 = x_cannon_mesh[t2[tindx]],
         x3 = x_cannon_mesh[t3[tindx]];
  double y1 = y_cannon_mesh[t1[tindx]], y2 = y_cannon_mesh[t2[tindx]],
         y3 = y_cannon_mesh[t3[tindx]];
  double z1 = z_cannon_mesh[t1[tindx]], z2 = z_cannon_mesh[t2[tindx]],
         z3 = z_cannon_mesh[t3[tindx]];
  double nx, ny, nz;
  nx = y1 * (z2 - z3) + y2 * (z3 - z1) + y3 * (z1 - z2);
  ny = z1 * (x2 - x3) + z2 * (x3 - x1) + z3 * (x1 - x2);
  nz = x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2);
  glNormal3d(nx, ny, nz);
}

void myTimer(int value) {
  if (spaceship_flying) {
    spaceship_altitude++;
  }
  if (ball_fired) {
    ball_z++;
    ball_y++;
  }
  for (int i = 0; i < ROBOTS_LENGTH; i++) {
    Robot *r = &(robots[i]);
    r->movement(r);
  }

  glutPostRedisplay();
  glutTimerFunc(50, myTimer, 0);
}

/**
 * @brief Handles keyboard input for special keys.
 *
 * @param key
 * @param x
 * @param y
 */
void special(int key, int x, int y) {
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
 * @brief Handles keyboard input for non-special keys.
 *
 * @param key
 * @param x
 * @param y
 */
void keyboard(unsigned char key, int x, int y) {
  switch (key) {
  case 's':
  case 'S':
    spaceship_flying = true;
    break;
  case 'c':
  case 'C':
    ball_fired = true;
    break;
  default:
    break;
  }
}

/**
 * @brief Draws a grid of lines on the floor plane.
 *
 */
void drawFloor(void) {
  glColor3d(0., 0.5, 0.); // Floor colour

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
void drawCannon(void) {
  glColor3d(0.4, 0.5, 0.4);

  glBegin(GL_TRIANGLES);
  for (int tindx = 0; tindx < num_triangles; tindx++) {
    normal(tindx);
    glVertex3d(x_cannon_mesh[t1[tindx]], y_cannon_mesh[t1[tindx]],
               z_cannon_mesh[t1[tindx]]);
    glVertex3d(x_cannon_mesh[t2[tindx]], y_cannon_mesh[t2[tindx]],
               z_cannon_mesh[t2[tindx]]);
    glVertex3d(x_cannon_mesh[t3[tindx]], y_cannon_mesh[t3[tindx]],
               z_cannon_mesh[t3[tindx]]);
  }
  glEnd();
}

void cannonBall(void) {
  glPushMatrix();
  glTranslated(ball_x, ball_y, ball_z);
  glutSolidSphere(0.7, 36, 18);
  glPopMatrix();
}

/**
 * @brief Draws the castle, and it's constituent supports.
 *
 */
void cannon(void) {
  glPushMatrix();
  // Global transitions
  glTranslated(0.0, 0.0, 50.0);
  glRotated(-90.0, 0.0, 1.0, 0.0);
  glScaled(0.1, 0.1, 0.1);

  // Cannon
  glPushMatrix();
  glTranslated(-20.0, 30.0, 0);
  glRotated(15.0, 0, 0, 1);
  glTranslated(20.0, -30.0, 0);
  drawCannon();
  glPopMatrix();

  // Supports
  glPushMatrix();
  glTranslated(-10.0, 5.0, 17.0);
  glScaled(80.0, 10.0, 6.0);
  glutSolidCube(1);
  glPopMatrix();

  glPushMatrix();
  glTranslated(-20.0, 25.0, 17.0);
  glScaled(40.0, 30.0, 6.0);
  glutSolidCube(1);
  glPopMatrix();

  glPushMatrix();
  glTranslated(-10.0, 5.0, -17.0);
  glScaled(80.0, 10.0, 6.0);
  glutSolidCube(1);
  glPopMatrix();

  glPushMatrix();
  glTranslated(-20.0, 25.0, -17.0);
  glScaled(40.0, 30.0, 6.0);
  glutSolidCube(1);
  glPopMatrix();

  glPopMatrix();

  cannonBall();
}

/**
 * @brief Draws a castle tower, and moves it to the given location.
 *
 * @param x Translation distance in the x-direction.
 * @param y Translation distance in the y-direction.
 * @param z Translation distance in the z-direction.
 */
void castleTower(GLdouble x = 0.0, GLdouble y = 0.0, GLdouble z = 0.0) {
  glPushMatrix();
  glTranslated(x, y, z);

  // Tower base
  glPushMatrix();
  glRotated(-90.0, 1.0, 0.0, 0.0);
  glutSolidCylinder(5.0, tower_height, 100.0, 100.0);
  glPopMatrix();

  // Tower root
  glPushMatrix();
  glTranslated(0.0, tower_height, 0.0);
  glRotated(-90.0, 1.0, 0.0, 0.0);
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
void castleWall(GLdouble translate_x = 0.0, GLdouble translate_y = 0.0,
                GLdouble translate_z = 0.0, GLdouble angle = 0.0,
                GLdouble rotate_x = 0.0, GLdouble rotate_y = 0.0,
                GLdouble rotate_z = 0.0) {
  double half_height = wall_height / 2;
  double half_thick = wall_thickness / 2;
  glPushMatrix();
  glTranslated(translate_x, translate_y, translate_z);
  glRotated(angle, rotate_x, rotate_y, rotate_z);

  // Front wall
  glBegin(GL_QUAD_STRIP);
  glNormal3d(0.0, 0.0, 1.0);
  glVertex3d(-20.0, 0.0, half_thick);
  glVertex3d(-5.0, 0.0, half_thick);
  glVertex3d(-20.0, wall_height, half_thick);
  glVertex3d(-5.0, half_height, half_thick);
  glVertex3d(20.0, wall_height, half_thick);
  glVertex3d(5.0, half_height, half_thick);
  glVertex3d(20.0, 0.0, half_thick);
  glVertex3d(5.0, 0.0, half_thick);
  glEnd();

  // Back wall
  glBegin(GL_QUAD_STRIP);
  glNormal3d(0.0, 0.0, -1.0);
  glVertex3d(-20.0, 0.0, -half_thick);
  glVertex3d(-5.0, 0.0, -half_thick);
  glVertex3d(-20.0, wall_height, -half_thick);
  glVertex3d(-5.0, half_height, -half_thick);
  glVertex3d(20.0, wall_height, -half_thick);
  glVertex3d(5.0, half_height, -half_thick);
  glVertex3d(20.0, 0.0, -half_thick);
  glVertex3d(5.0, 0.0, -half_thick);
  glEnd();

  // Passageway
  glBegin(GL_QUAD_STRIP);
  glNormal3d(1.0, 0.0, 0.0);
  glVertex3d(-5, 0.0, half_thick);
  glVertex3d(-5, 0.0, -half_thick);
  glVertex3d(-5, half_height, half_thick);
  glVertex3d(-5, half_height, -half_thick);
  glNormal3d(0.0, -1.0, 0.0);
  glVertex3d(5, half_height, half_thick);
  glVertex3d(5, half_height, -half_thick);
  glNormal3d(-1.0, 0.0, 0.0);
  glVertex3d(5, 0.0, half_thick);
  glVertex3d(5, 0.0, -half_thick);
  glEnd();

  // Top of wall
  glBegin(GL_QUAD_STRIP);
  glNormal3d(0.0, 1.0, 0.0);
  glVertex3d(-20.0, wall_height, half_thick);
  glVertex3d(-20.0, wall_height, -half_thick);
  glVertex3d(20.0, wall_height, half_thick);
  glVertex3d(20.0, wall_height, -half_thick);
  glEnd();
  glPopMatrix();
}

/**
 * @brief Draws the castle.
 *
 */
void castle(void) {
  // Right front tower
  castleTower(20.0, 0.0, 20.0);
  // Front
  castleWall(0.0, 0.0, 20.0);
  // Left front tower
  castleTower(-20.0, 0.0, 20.0);

  // Right back tower
  castleTower(20.0, 0.0, -20.0);
  // Back
  castleWall(0.0, 0.0, -20.0);
  // Left back tower
  castleTower(-20.0, 0.0, -20.0);

  // Right side wall
  castleWall(20.0, 0.0, 0.0, 90.0, 0.0, 1.0, 0.0);

  // Left side wall
  castleWall(-20.0, 0.0, 0.0, 90.0, 0.0, 1.0, 0.0);
}

/**
 * @brief Draws a rocket fin.
 *
 * @param translate_x
 * @param translate_y
 * @param translate_z
 * @param angle
 * @param rotate_x
 * @param rotate_y
 * @param rotate_z
 */
void fin(GLdouble translate_x = 0.0, GLdouble translate_y = 0.0,
         GLdouble translate_z = 0.0, GLdouble angle = 0.0,
         GLdouble rotate_x = 0.0, GLdouble rotate_y = 0.0,
         GLdouble rotate_z = 0.0) {
  glPushMatrix();
  // glRotated(angle, rotate_x, rotate_y, rotate_z);
  glRotated(angle, rotate_x, rotate_y, rotate_z);
  glTranslated(translate_x, translate_y, translate_z);
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
void spaceship(void) {
  glPushMatrix();
  glTranslated(0.0, spaceship_altitude, 0.0);

  // Fuselage
  glPushMatrix();
  glTranslated(0.0, 4.0, 0.0);
  glRotated(-90.0, 1.0, 0.0, 0.0);
  glutSolidCylinder(2.0, spaceship_height, 100.0, 100.0);
  glPopMatrix();

  // Tower root
  glPushMatrix();
  glTranslated(0.0, 4.0 + spaceship_height, 0.0);
  glRotated(-90.0, 1.0, 0.0, 0.0);
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
void display(void) {
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
  for (int i = 0; i < ROBOTS_LENGTH; i++) {
    Robot r = robots[i];
    draw_robot(r);
  }

  glFlush();
}

/**
 * @brief Initializes OpenGL states.
 *
 */
void initialize(void) {
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
int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_DEPTH);
  glutInitWindowSize(1000, 1000);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("COSC363 Assignment 1");
  glutSpecialFunc(special);
  glutKeyboardFunc(keyboard);
  glutTimerFunc(50, myTimer, 0);
  initialize();
  glutDisplayFunc(display);
  glutMainLoop();
  return 0;
}
