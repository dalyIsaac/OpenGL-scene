#include "main.h"
#include "cannon.h"
#include "castle.h"
#include "loadTGA.h"
#include "robot.h"
#include "spaceship.h"
#include <GL/freeglut.h>
#include <climits>
#include <cmath>
#include <fstream>
#include <iostream>
using namespace std;

GLuint txId[3]; // Texture ids
GLUquadricObj *q;

float red[4] = {1.0f, 0.16f, 0.16f, 1.0f};
float green[4] = {0.16f, 1.0f, 0.321f, 1.0f};

static double radians_five = 0.0872665;

// static double camera_angle = 0;
static double camera_angle = 1.5707969999999996;

// static double eye_x = 0.0;
// static double eye_y = 20.0;
// static double eye_z = 100.0;
// static double look_x = 0;
// static double look_y = 0.0;
// static double look_z = 1;

static double eye_x = -125.49999999997148;
static double eye_y = 20.0;
static double eye_z = 99.999915512758989;
static double look_x = -99.999915512758989;
static double look_y = 0.0;
static double look_z = 99.999982833269286;

/**
 * @brief Loads the OFF mesh file.
 *
 * @param fname
 */
static void loadMeshFile(const char *fname) {
  ifstream fp_in;
  int num, ne;

  fp_in.open(fname, ios::in);
  if (!fp_in.is_open()) {
    cout << "Error opening mesh file" << endl;
    exit(1);
  }

  fp_in.ignore(INT_MAX, '\n'); // ignore first line
  fp_in >> cannon_num_vertices >> cannon_num_triangles >>
      ne; // read number of vertices, polygons, edges

  x_cannon_mesh = new double[cannon_num_vertices]; // create arrays
  y_cannon_mesh = new double[cannon_num_vertices];
  z_cannon_mesh = new double[cannon_num_vertices];

  cannon_t1 = new int[cannon_num_triangles];
  cannon_t2 = new int[cannon_num_triangles];
  cannon_t3 = new int[cannon_num_triangles];

  for (int i = 0; i < cannon_num_vertices; i++) // read vertex list
    fp_in >> x_cannon_mesh[i] >> y_cannon_mesh[i] >> z_cannon_mesh[i];

  for (int i = 0; i < cannon_num_triangles; i++) // read polygon list
  {
    fp_in >> num >> cannon_t1[i] >> cannon_t2[i] >> cannon_t3[i];
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
 * @brief Loads the brick texture.
 *
 */
static void loadTexture() {
  glGenTextures(2, txId); // Create 2 texture ids

  glBindTexture(GL_TEXTURE_2D, txId[0]);
  loadTGA("textures/wall.tga");
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glBindTexture(GL_TEXTURE_2D, txId[1]);
  loadTGA("textures/tower.tga");
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glBindTexture(GL_TEXTURE_2D, txId[2]);
  loadTGA("textures/roof.tga");
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}

/**
 * @brief Calculates the normal vector of a triangle.
 *
 * @param tindx
 */
void normal(int tindx) {
  double x1 = x_cannon_mesh[cannon_t1[tindx]],
         x2 = x_cannon_mesh[cannon_t2[tindx]],
         x3 = x_cannon_mesh[cannon_t3[tindx]];
  double y1 = y_cannon_mesh[cannon_t1[tindx]],
         y2 = y_cannon_mesh[cannon_t2[tindx]],
         y3 = y_cannon_mesh[cannon_t3[tindx]];
  double z1 = z_cannon_mesh[cannon_t1[tindx]],
         z2 = z_cannon_mesh[cannon_t2[tindx]],
         z3 = z_cannon_mesh[cannon_t3[tindx]];
  double nx, ny, nz;
  nx = y1 * (z2 - z3) + y2 * (z3 - z1) + y3 * (z1 - z2);
  ny = z1 * (x2 - x3) + z2 * (x3 - x1) + z3 * (x1 - x2);
  nz = x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2);
  glNormal3d(nx, ny, nz);
}

static void generalTimer(int value) {
  if (spaceship_flying) {
    spaceship_altitude++;
  }
  if (ball_fired) {
    // ball_z++;
    // ball_y++;
    cannonBallPhysics();
  }
  for (int i = 0; i < ROBOTS_LENGTH; i++) {
    Robot *r = &(robots[i]);
    r->movement(r);
  }

  glutPostRedisplay();
  glutTimerFunc(50, generalTimer, 0);
}

/**
 * @brief Handles keyboard input for special keys.
 *
 * @param key
 * @param x
 * @param y
 */
static void special(int key, int x, int y) {
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
static void keyboard(unsigned char key, int x, int y) {
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
static void floor(void) {
  float white[4] = {1., 1., 1., 1.};
  float black[4] = {0};
  glColor4f(0.7, 0.7, 0.7, 1.0); // The floor is gray in colour
  glNormal3f(0.0, 1.0, 0.0);

  // The floor is made up of several tiny squares on a 500x500 grid. Each square
  // has a unit size.
  glMaterialfv(GL_FRONT, GL_SPECULAR,
               black); // suppresses specular reflections from the floor
  glBegin(GL_QUADS);
  for (int i = -500; i < 500; i++) {
    for (int j = -500; j < 500; j++) {
      glVertex3f(i, 0, j);
      glVertex3f(i, 0, j + 1);
      glVertex3f(i + 1, 0, j + 1);
      glVertex3f(i + 1, 0, j);
    }
  }
  glEnd();
  glMaterialfv(GL_FRONT, GL_SPECULAR, white);
}

/**
 * @brief Contains function calls for generating the scene.
 *
 */
static void display(void) {
  float light_pos[] = {0.0f, 50.0f, 50.0f, 1.0f};
  float spot_pos[] = {-10.0, 14.0, 0.0, 1.0};
  float spot_dir[] = {-10.0, -10.0, 0.0, 0.0};

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // Camera position and orientation
  gluLookAt(eye_x, eye_y, eye_z, look_x, look_y, look_z, 0, 1, 0);

  // Sets the light's position
  glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

  floor();
  castle();
  cannon();
  spaceship();
  for (int i = 0; i < ROBOTS_LENGTH; i++) {
    Robot r = robots[i];
    draw_robot(r);
  }

  glutSwapBuffers();
}

/**
 * @brief Initializes OpenGL states.
 *
 */
static void initialize(void) {
  float grey[4] = {0.2f, 0.2f, 0.2f, 1.0f};
  float white[4] = {1.0f, 1.0f, 1.0f, 1.0f};

  q = gluNewQuadric();
  gluQuadricTexture(q, GL_TRUE);

  loadTexture();
  loadMeshFile("models/Cannon.off");

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  glEnable(GL_LIGHT2);

  //	Define light's ambient, diffuse, specular properties
  glLightfv(GL_LIGHT0, GL_AMBIENT, grey);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
  glLightfv(GL_LIGHT0, GL_SPECULAR, white);

  glLightfv(GL_LIGHT1, GL_AMBIENT, grey);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, red);
  glLightfv(GL_LIGHT1, GL_SPECULAR, red);

  glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 60.0);
  glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 100.0);

  glLightfv(GL_LIGHT2, GL_AMBIENT, grey);
  glLightfv(GL_LIGHT2, GL_DIFFUSE, green);
  glLightfv(GL_LIGHT2, GL_SPECULAR, green);

  glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 60.0);
  glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 100.0);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_NORMALIZE);
  gluQuadricDrawStyle(q, GLU_FILL);
  glClearColor(0.0, 0.0, 0.0, 0.0); // Background colour

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);

  glMaterialfv(GL_FRONT, GL_SPECULAR, white);
  glMaterialf(GL_FRONT, GL_SHININESS, 50);

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
  glutTimerFunc(50, generalTimer, 0);
  glutTimerFunc(500, spaceshipLightsTimer, 0);
  initialize();
  glutDisplayFunc(display);
  glutMainLoop();
  return 0;
}
