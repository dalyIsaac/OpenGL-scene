#include "spaceship.h"
#include "main.h"
#include <GL/freeglut.h>

float spaceship_altitude = 0.0;
double spaceship_height = 20.0;
bool spaceship_flying = false;

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
  float red_light_pos[] = {2.0f, 4.0f + spaceship_altitude, 0.0f, 1.0f};
  float red_light_spot_dir[] = {-20.0f, -10.0f, 0.0f, 0.0f};
  float green_light_pos[] = {2.0f, 4.0f + spaceship_altitude, 0.0f, 1.0f};
  float green_light_spot_dir[] = {20.0f, -10.0f, 0.0f, 0.0f};

  glPushMatrix();
  glTranslatef(0.0, spaceship_altitude, 0.0);

  // Red light
  glLightfv(GL_LIGHT1, GL_POSITION, red_light_pos);
  glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, red_light_spot_dir);

  // Green light
  glLightfv(GL_LIGHT2, GL_POSITION, green_light_pos);
  glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, green_light_spot_dir);

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
