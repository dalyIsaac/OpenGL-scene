#include "spaceship.h"
#include "main.h"
#include "sweepSurfaces.h"
#include <GL/freeglut.h>
#include <cmath>

/**
 * @brief The current altitude of the spaceship.
 *
 */
float spaceship_altitude = 0.0;

/**
 * @brief The height of the spaceship's fuselage.
 *
 */
double fuselage_height = 20.0;

/**
 * @brief The radius of the spaceship's fuselage.
 *
 */
float fuselage_radius = 2.0;

/**
 * @brief True if the spaceship is flying.
 *
 */
bool spaceship_flying = false;

/**
 * @brief Toggled by the timer, to indicate whether the spaceship's navigational
 * lights are on.
 *
 */
static bool lights_on = true;

/**
 * @brief The number of vertices inside the base curve for the spaceship's nose
 * cone.
 *
 */
const int N = 2 + 1;

/**
 * @brief x-axis components of the nose cone.
 *
 */
float x_points[N] = {0};

/**
 * @brief y-axis components of the nose cone.
 *
 */
float y_points[N] = {0};

/**
 * @brief z-axis components of the nose cone.
 *
 */
float z_points[N] = {0};

/**
 * @brief Initializes the values for the nose cone.
 *
 */
static void noseConeInit(void) {
  for (int i = 0; i < N; i++) {
    double x = i;
    x_points[i] = x;
    y_points[i] = -(x * x) + fuselage_radius * 2.0;
  }
}

/**
 * @brief Initializes the spaceship.
 *
 */
void spaceshipInit(void) { noseConeInit(); }

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
static void fin(GLdouble translate_x = 0.0, GLdouble translate_y = 0.0,
                GLdouble translate_z = 0.0, GLdouble angle = 0.0,
                GLdouble rotate_x = 0.0, GLdouble rotate_y = 0.0,
                GLdouble rotate_z = 0.0) {
  glPushMatrix();
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
  float red_pos[] = {-2.0f, 14.0f + spaceship_altitude, 0.0f, 1.0f};
  float red_spot_dir[] = {-10.0f, -10.0f, 0.0f, 0.0f};
  float green_pos[] = {2.0f, 14.0f + spaceship_altitude, 0.0f, 1.0f};
  float green_spot_dir[] = {10.0f, -10.0f, 0.0f, 0.0f};

  glPushMatrix();
  glTranslatef(0.0, spaceship_altitude, 0.0);

  // Fuselage
  glPushMatrix();
  glTranslated(0.0, 4.0, 0.0);
  glRotated(-90.0, 1.0, 0.0, 0.0);
  glutSolidCylinder(fuselage_radius, fuselage_height, 100.0, 100.0);
  glPopMatrix();

  // Nose cone
  glPushMatrix();
  glTranslated(0.0, 4.0 + fuselage_height, 0.0);
  sweepSurface(N, x_points, y_points, z_points);
  glPopMatrix();

  glPushMatrix();
  fin(fuselage_radius);
  fin(fuselage_radius, 0.0, 0.0, 120.0, 0.0, 1.0, 0.0);
  fin(fuselage_radius, 0.0, 0.0, 240.0, 0.0, 1.0, 0.0);
  glPopMatrix();

  if (lights_on) {
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
    // Red light
    glLightfv(GL_LIGHT1, GL_POSITION, red_pos);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, red_spot_dir);

    // Green light
    glLightfv(GL_LIGHT2, GL_POSITION, green_pos);
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, green_spot_dir);
  } else {
    glDisable(GL_LIGHT1);
    glDisable(GL_LIGHT2);
  }

  // Red light disk
  glPushMatrix();
  glTranslated(red_pos[0], red_pos[1], red_pos[2]);
  glRotated(90.0, 0.0, 1.0, 0.0);
  if (lights_on) {
    glColor3f(red[0], red[1], red[2]);
  }
  gluDisk(q, 0.0, 0.5, 20, 2);
  glPopMatrix();

  // Green light disk
  glPushMatrix();
  glTranslated(green_pos[0], green_pos[1], green_pos[2]);
  glRotated(90.0, 0.0, 1.0, 0.0);
  if (lights_on) {
    glColor3f(green[0], green[1], green[2]);
  }
  gluDisk(q, 0.0, 0.5, 20, 2);
  glPopMatrix();

  glPopMatrix();
}

/**
 * @brief Timer for the navigational lights of the spaceship. Runs every 500
 * milliseconds.
 *
 * @param value
 */
void spaceshipLightsTimer(int value) {
  lights_on = !lights_on;
  glutPostRedisplay();
  glutTimerFunc(500, spaceshipLightsTimer, 0);
}
