#include "castle.h"
#include "main.h"
#include <GL/freeglut.h>

double const wall_height = 30.0;
static const double wall_thickness = 6.0;
static const double half_height = wall_height / 2;
static const double half_thick = wall_thickness / 2;

/**
 * @brief Draws a castle tower, and moves it to the given location.
 *
 * @param x Translation distance in the x-direction.
 * @param y Translation distance in the y-direction.
 * @param z Translation distance in the z-direction.
 */
static void castleTower(GLdouble x = 0.0, GLdouble y = 0.0, GLdouble z = 0.0) {
  double tower_height = 35.0;
  glEnable(GL_TEXTURE_2D);

  glPushMatrix();
  glTranslated(x, y, z);

  // Tower base
  glBindTexture(GL_TEXTURE_2D, txId[1]);
  glPushMatrix();
  glRotated(-90.0, 1.0, 0.0, 0.0);
  gluCylinder(q, 5.0, 5.0, tower_height, 100.0, 100.0);
  glPopMatrix();

  // Tower root
  glBindTexture(GL_TEXTURE_2D, txId[2]);
  glPushMatrix();
  glTranslated(0.0, tower_height, 0.0);
  glRotated(-90.0, 1.0, 0.0, 0.0);
  gluCylinder(q, 7.0, 0.0, 10.0, 100.0, 100.0);
  glPopMatrix();

  glPopMatrix();

  glDisable(GL_TEXTURE_2D);
}

/**
 * @brief Draws the passageway for the castle wall
 *
 * @param half_thick Half of the thickness of the wall.
 * @param half_height Half of the height of the wall.
 */
static void passageway(double half_thick, double half_height) {
  double tex_height = 5.0 / 6.0;
  glBegin(GL_QUAD_STRIP);
  glNormal3d(1.0, 0.0, 0.0);

  // Front left bottom corner
  glTexCoord2d(0.0, 0.0);
  glVertex3d(-5, 0.0, half_thick);

  // Back left bottom corner
  glTexCoord2d(0.0, tex_height);
  glVertex3d(-5, 0.0, -half_thick);

  // Front left top corner
  glTexCoord2d(2.0, 0.0);
  glVertex3d(-5, half_height, half_thick);

  // Back left top corner
  glTexCoord2d(2.0, tex_height);
  glVertex3d(-5, half_height, -half_thick);

  glNormal3d(0.0, -1.0, 0.0);

  // Front right top corner
  glTexCoord2d(4.0, 0.0);
  glVertex3d(5, half_height, half_thick);

  // Back right top corner
  glTexCoord2d(4.0, tex_height);
  glVertex3d(5, half_height, -half_thick);

  glNormal3d(-1.0, 0.0, 0.0);

  // Front right bottom corner
  glTexCoord2d(6.0, 0.0);
  glVertex3d(5, 0.0, half_thick);

  // Back right bottom corner
  glTexCoord2d(6.0, tex_height);
  glVertex3d(5, 0.0, -half_thick);
  glEnd();
}

/**
 * @brief Draws the top of castle walls.
 *
 * @param wall_height The height of the wall.
 * @param half_thick Half of the thickness of the wall.
 */
static void wallTop(double wall_height, double half_thick) {
  double tex_height = 5.0 / 6.0;
  glBegin(GL_QUAD_STRIP);
  glNormal3d(0.0, 1.0, 0.0);

  // Left front corner
  glTexCoord2d(0.0, 0.0);
  glVertex3d(-40.0, wall_height, half_thick);

  // Left back corner
  glTexCoord2d(0.0, tex_height);
  glVertex3d(-40.0, wall_height, -half_thick);

  // Right front corner
  glTexCoord2d(8.0, 0.0);
  glVertex3d(40.0, wall_height, half_thick);

  // Right back corner
  glTexCoord2d(8.0, tex_height);
  glVertex3d(40.0, wall_height, -half_thick);
  glEnd();
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
static void castleWall(GLdouble translate_x = 0.0, GLdouble translate_y = 0.0,
                       GLdouble translate_z = 0.0, GLdouble angle = 0.0,
                       GLdouble rotate_x = 0.0, GLdouble rotate_y = 0.0,
                       GLdouble rotate_z = 0.0) {

  glPushMatrix();
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, txId[0]);

  glTranslated(translate_x, translate_y, translate_z);
  glRotated(angle, rotate_x, rotate_y, rotate_z);

  // Walls
  for (double i = 1.0; i >= -1.0; i -= 2.0) {
    glBegin(GL_QUAD_STRIP);
    glNormal3d(0.0, 0.0, i);

    // Bottom left corner
    glTexCoord2d(0.0, 0.0);
    glVertex3d(-40.0, 0.0, i * half_thick);

    // Bottom left-inner corner
    glTexCoord2d(3.5, 0.0);
    glVertex3d(-5.0, 0.0, i * half_thick);

    // Top left corner
    glTexCoord2d(0.0, 4.0);
    glVertex3d(-40.0, wall_height, i * half_thick);

    // Left upper-inner corner
    glTexCoord2d(3.5, 2.0);
    glVertex3d(-5.0, half_height, i * half_thick);

    // Right upper corner
    glTexCoord2d(8.0, 4.0);
    glVertex3d(40.0, wall_height, i * half_thick);

    // Right upper-inner corner
    glTexCoord2d(4.5, 2.0);
    glVertex3d(5.0, half_height, i * half_thick);

    // Right bottom corner
    glTexCoord2d(8.0, 0.0);
    glVertex3d(40.0, 0.0, i * half_thick);

    // Bottom right-inner corner
    glTexCoord2d(4.5, 0.0);
    glVertex3d(5.0, 0.0, i * half_thick);
    glEnd();
  }

  passageway(half_thick, half_height);

  wallTop(wall_height, half_thick);

  glDisable(GL_TEXTURE_2D);
  glPopMatrix();
}

/**
 * @brief Draws the castle.
 *
 */
void castle(void) {
  // Right front tower
  castleTower(40.0, 0.0, 40.0);
  // Front
  castleWall(0.0, 0.0, 40.0);
  // Left front tower
  castleTower(-40.0, 0.0, 40.0);

  // Right back tower
  castleTower(40.0, 0.0, -40.0);
  // Back
  castleWall(0.0, 0.0, -40.0);
  // Left back tower
  castleTower(-40.0, 0.0, -40.0);

  // Right side wall
  castleWall(40.0, 0.0, 0.0, 90.0, 0.0, 1.0, 0.0);

  // Left side wall
  castleWall(-40.0, 0.0, 0.0, 90.0, 0.0, 1.0, 0.0);
}
