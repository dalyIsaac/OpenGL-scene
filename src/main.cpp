#include "main.h"
#include "cannon.h"
#include "castle.h"
#include "loadTGA.h"
#include "robot.h"
#include "spaceship.h"
#include "trampoline.h"
#include <GL/freeglut.h>
#include <climits>
#include <cmath>
#include <fstream>
#include <iostream>
using namespace std;

GLuint txId[8];
GLUquadricObj *q;

float red[4] = {1.0f, 0.16f, 0.16f, 1.0f};
float green[4] = {0.16f, 1.0f, 0.321f, 1.0f};
float yellow[4] = {0.921f, 0.85f, 0.0f, 1.0f};

float lx = 0.0f;
float ly = 20.0f;
float lz = 50.0f;
float light_pos[4] = {lx, ly, lz, 1.0f};
float shadowMat[16] = {ly, 0, 0, 0, -lx, 0, -lz, -1, 0, 0, ly, 0, 0, 0, 0, ly};

static double radians_five = 0.0872665;

static float camera_angle = 0;

static float mobile_cam[6] = {0.0, 10.0, 100.0, 0.0, 0.0, 1.0};
static float spaceship_cam[6] = {0.0, 18.0, spaceship_radius, 0.0, 0.0, 45.0};

static bool is_mobile_cam = true;

static float *eye_x = &mobile_cam[0];
static float *eye_y = &mobile_cam[1];
static float *eye_z = &mobile_cam[2];
static float *look_x = &mobile_cam[3];
static float *look_y = &mobile_cam[4];
static float *look_z = &mobile_cam[5];

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
  // read number of vertices, polygons, edges
  fp_in >> cannon_num_vertices >> cannon_num_triangles >> ne;

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
  glGenTextures(8, txId);

  // Wall
  glBindTexture(GL_TEXTURE_2D, txId[0]);
  loadTGA("textures/wall.tga");
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Tower
  glBindTexture(GL_TEXTURE_2D, txId[1]);
  loadTGA("textures/tower.tga");
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Roof
  glBindTexture(GL_TEXTURE_2D, txId[2]);
  loadTGA("textures/roof.tga");
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Skybox - left
  glBindTexture(GL_TEXTURE_2D, txId[3]);
  loadTGA("textures/skybox/right.tga");
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  // Skybox - front
  glBindTexture(GL_TEXTURE_2D, txId[4]);
  loadTGA("textures/skybox/front.tga");
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  // Skybox - right
  glBindTexture(GL_TEXTURE_2D, txId[5]);
  loadTGA("textures/skybox/left.tga");
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  // Skybox - back
  glBindTexture(GL_TEXTURE_2D, txId[6]);
  loadTGA("textures/skybox/back.tga");
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  // Skybox - top
  glBindTexture(GL_TEXTURE_2D, txId[7]);
  loadTGA("textures/skybox/top.tga");
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

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

void normal(float x1, float y1, float z1, float x2, float y2, float z2,
            float x3, float y3, float z3) {
  float nx, ny, nz;
  nx = y1 * (z2 - z3) + y2 * (z3 - z1) + y3 * (z1 - z2);
  ny = z1 * (x2 - x3) + z2 * (x3 - x1) + z3 * (x1 - x2);
  nz = x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2);

  glNormal3f(nx, ny, nz);
}

static void generalTimer(int value) {
  if (spaceship_flying) {
    spaceship_altitude++;
  }
  if (ball_fired) {
    cannonBallPhysics();
  }
  for (int i = 0; i < NUM_ROBOTS; i++) {
    Robot *r = &(robots[i]);
    r->movement(r);
  }

  trampTimer();

  glutPostRedisplay();
  glutTimerFunc(50, generalTimer, 0);
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

  // The floor is made up of several tiny squares on a 500x500 grid. Each
  // square has a unit size.
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

void skybox() {
  floor();

  glPushMatrix();
  glEnable(GL_TEXTURE_2D);

  // Left wall
  glBindTexture(GL_TEXTURE_2D, txId[3]);
  glBegin(GL_QUADS);
  glTexCoord2d(0.0, 0.0);
  glVertex3d(-500, 0, 500);
  glTexCoord2d(1.0, 0.0);
  glVertex3d(-500, 0., -500);
  glTexCoord2d(1.0, 1.0);
  glVertex3d(-500, 500., -500);
  glTexCoord2d(0.0, 1.0);
  glVertex3d(-500, 500, 500);
  glEnd();

  // Front wall
  glBindTexture(GL_TEXTURE_2D, txId[4]);
  glBegin(GL_QUADS);
  glTexCoord2d(0.0, 0.0);
  glVertex3d(-500, 0, -500);
  glTexCoord2d(1.0, 0.0);
  glVertex3d(500, 0., -500);
  glTexCoord2d(1.0, 1.0);
  glVertex3d(500, 500, -500);
  glTexCoord2d(0.0, 1.0);
  glVertex3d(-500, 500, -500);
  glEnd();

  // Right wall
  glBindTexture(GL_TEXTURE_2D, txId[5]);
  glBegin(GL_QUADS);
  glTexCoord2d(0.0, 0.0);
  glVertex3d(500, 0, -500);
  glTexCoord2d(1.0, 0.0);
  glVertex3d(500, 0, 500);
  glTexCoord2d(1.0, 1.0);
  glVertex3d(500, 500, 500);
  glTexCoord2d(0.0, 1.0);
  glVertex3d(500, 500, -500);
  glEnd();

  // Back wall
  glBindTexture(GL_TEXTURE_2D, txId[6]);
  glBegin(GL_QUADS);
  glTexCoord2d(0.0, 0.0);
  glVertex3d(500, 0, 500);
  glTexCoord2d(1.0, 0.0);
  glVertex3d(-500, 0, 500);
  glTexCoord2d(1.0, 1.0);
  glVertex3d(-500, 500, 500);
  glTexCoord2d(0.0, 1.0);
  glVertex3d(500, 500, 500);
  glEnd();

  // Top
  glBindTexture(GL_TEXTURE_2D, txId[7]);
  glBegin(GL_QUADS);
  glTexCoord2d(0.0, 0.0);
  glVertex3d(-500, 500, -500);
  glTexCoord2d(1.0, 0.0);
  glVertex3d(500, 500, -500);
  glTexCoord2d(1.0, 1.0);
  glVertex3d(500, 500, 500);
  glTexCoord2d(0.0, 1.0);
  glVertex3d(-500, 500, 500);
  glEnd();

  glPopMatrix();
}

static void updateSpaceshipCam(void) {
  if (spaceship_cam[1] > 10.0 && spaceship_altitude >= spaceship_cam[1]) {
    spaceship_cam[1] = spaceship_altitude;
  }
  eye_x = &spaceship_cam[0];
  eye_y = &spaceship_cam[1];
  eye_z = &spaceship_cam[2];
  look_x = &spaceship_cam[3];
  look_y = &spaceship_cam[4];
  look_z = &spaceship_cam[5];
}

/**
 * @brief Handles keyboard input for special keys.
 *
 * @param key
 * @param x
 * @param y
 */
static void special(int key, int x, int y) {
  if (key == GLUT_KEY_HOME) {
    is_mobile_cam = !is_mobile_cam;
  }

  if (is_mobile_cam) {
    eye_x = &mobile_cam[0];
    eye_y = &mobile_cam[1];
    eye_z = &mobile_cam[2];
    look_x = &mobile_cam[3];
    look_y = &mobile_cam[4];
    look_z = &mobile_cam[5];
    switch (key) {
      case GLUT_KEY_UP:
        *eye_x += 0.5 * sin(camera_angle);
        *eye_z -= 0.5 * cos(camera_angle);
        break;
      case GLUT_KEY_DOWN:
        *eye_x -= 0.5 * sin(camera_angle);
        *eye_z += 0.5 * cos(camera_angle);
        break;
      case GLUT_KEY_LEFT:
        camera_angle -= radians_five;
        break;
      case GLUT_KEY_RIGHT:
        camera_angle += radians_five;
        break;
      case GLUT_KEY_PAGE_UP:
        (*eye_y)++;
        break;
      case GLUT_KEY_PAGE_DOWN:
        (*eye_y)--;
        break;
      default:
        break;
    }

    *look_x = *eye_x + 100 * sin(camera_angle);
    *look_z = *eye_z - 100 * cos(camera_angle);
  } else {
    updateSpaceshipCam();
  }

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
 * @brief Contains function calls for generating the scene.
 *
 */
static void display(void) {
  float spot_pos[] = {-10.0, 14.0, 0.0, 1.0};
  float spot_dir[] = {-10.0, -10.0, 0.0, 0.0};
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  if (spaceship_flying && !is_mobile_cam) {
    updateSpaceshipCam();
  }

  gluLookAt(*eye_x, *eye_y, *eye_z, *look_x, *look_y, *look_z, 0, 1, 0);

  glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

  trampoline();
  skybox();
  castle();
  cannon();
  spaceship();
  for (int i = 0; i < NUM_ROBOTS; i++) {
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
  spaceshipInit();

  glClearColor(0.0, 0.0, 0.0, 0.0); // Background colour
  glClearDepth(1.0f);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_NORMALIZE);

  glEnable(GL_LIGHTING);

  // General light
  glEnable(GL_LIGHT0);
  glLightfv(GL_LIGHT0, GL_AMBIENT, grey);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
  glLightfv(GL_LIGHT0, GL_SPECULAR, white);

  // Red light on spaceship
  glEnable(GL_LIGHT1);
  glLightfv(GL_LIGHT1, GL_AMBIENT, grey);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, red);
  glLightfv(GL_LIGHT1, GL_SPECULAR, red);
  glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 60.0);
  glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 100.0);

  // Green light on spaceship
  glEnable(GL_LIGHT2);
  glLightfv(GL_LIGHT2, GL_AMBIENT, grey);
  glLightfv(GL_LIGHT2, GL_DIFFUSE, green);
  glLightfv(GL_LIGHT2, GL_SPECULAR, green);
  glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 60.0);
  glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 100.0);

  // Lights for the robots
  for (int i = 0; i < NUM_ROBOTS; i++) {
    Robot r = robots[i];
    glEnable(r.light);
    glLightfv(r.light, GL_AMBIENT, grey);
    glLightfv(r.light, GL_DIFFUSE, yellow);
    glLightfv(r.light, GL_SPECULAR, yellow);
    glLightf(r.light, GL_SPOT_CUTOFF, 60.0);
    glLightf(r.light, GL_SPOT_EXPONENT, 100.0);
  }

  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 50);
  glEnable(GL_COLOR_MATERIAL);

  gluQuadricDrawStyle(q, GLU_FILL);
  gluQuadricNormals(q, GLU_SMOOTH);

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
  glutTimerFunc(50, generalTimer, 0);
  glutTimerFunc(500, spaceshipLightsTimer, 0);
  initialize();
  glutDisplayFunc(display);
  glutMainLoop();
  return 0;
}
