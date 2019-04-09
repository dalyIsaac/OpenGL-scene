#include "spaceship.h"
#include "main.h"
#include <GL/freeglut.h>
#include <cmath>

float spaceship_altitude = 0.0;
double spaceship_height = 20.0;
bool spaceship_flying = false;
float spaceship_radius = 2.0;

static bool lights_on = true;

const int N = 2 + 1; // Total number of vertices on the base curve
float angle = 25;
float vx[N] = {0};
float vy[N] = {0};
float vz[N] = {0};

static void noseConeInit(void) {
  for (int i = 0; i < N; i++) {
    double x = i;
    vx[i] = x;
    vy[i] = -(x * x) + spaceship_radius * 2.0;
  }
}

void spaceshipInit(void) { noseConeInit(); }

static void cone(void) {
  float wx[N], wy[N], wz[N];
  float theta = -0.1745;

  glBegin(GL_TRIANGLE_STRIP);
  for (int j = 0; j < 36; j++) {
    for (int i = 0; i < N; i++) {
      wx[i] = vx[i] * cos(theta) + vz[i] * sin(theta);
      wy[i] = vy[i];
      wz[i] = -vx[i] * sin(theta) + vz[i] * cos(theta);
    }

    for (int i = 0; i < N; i++) {
      if (i > 0) {
        normal(wx[i - 1], wy[i - 1], wz[i - 1], vx[i - 1], vy[i - 1], vz[i - 1],
               vx[i], vy[i], vz[i]);
      }
      glTexCoord2f(j / 36.0, i / float(N));
      glVertex3f(vx[i], vy[i], vz[i]);
      if (i > 0) {
        normal(wx[i - 1], wy[i - 1], wz[i - 1], vx[i], vy[i], vz[i], wx[i],
               wy[i], wz[i]);
      }
      glTexCoord2f((j + 1) / 36.0, i / float(N));
      glVertex3f(wx[i], wy[i], wz[i]);
    }

    for (int i = 0; i < N; i++) {
      vx[i] = wx[i];
      vy[i] = wy[i];
      vz[i] = wz[i];
    }
  }
  glEnd();
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
  glutSolidCylinder(spaceship_radius, spaceship_height, 100.0, 100.0);
  glPopMatrix();

  // Nose cone
  glPushMatrix();
  glTranslated(0.0, 4.0 + spaceship_height, 0.0);
  cone();
  glPopMatrix();

  glPushMatrix();
  fin(spaceship_radius);
  fin(spaceship_radius, 0.0, 0.0, 120.0, 0.0, 1.0, 0.0);
  fin(spaceship_radius, 0.0, 0.0, 240.0, 0.0, 1.0, 0.0);
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

void spaceshipLightsTimer(int value) {
  lights_on = !lights_on;
  glutPostRedisplay();
  glutTimerFunc(500, spaceshipLightsTimer, 0);
}
