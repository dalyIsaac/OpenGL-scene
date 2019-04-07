#include "cannon.h"
#include "main.h"
#include <GL/freeglut.h>

bool ball_fired = false;
double ball_x = 0.0;
double ball_y = 4.8;
double ball_z = 54.5;

int cannon_num_vertices;
int cannon_num_triangles;

double *x_cannon_mesh;
double *y_cannon_mesh;
double *z_cannon_mesh;

int *cannon_t1;
int *cannon_t2;
int *cannon_t3;

/**
 * @brief Draws the cannon.
 *
 */
static void drawCannon(void) {
  glColor3d(0.4, 0.5, 0.4);

  glBegin(GL_TRIANGLES);
  for (int tindx = 0; tindx < cannon_num_triangles; tindx++) {
    normal(tindx);
    glVertex3d(x_cannon_mesh[cannon_t1[tindx]], y_cannon_mesh[cannon_t1[tindx]],
               z_cannon_mesh[cannon_t1[tindx]]);
    glVertex3d(x_cannon_mesh[cannon_t2[tindx]], y_cannon_mesh[cannon_t2[tindx]],
               z_cannon_mesh[cannon_t2[tindx]]);
    glVertex3d(x_cannon_mesh[cannon_t3[tindx]], y_cannon_mesh[cannon_t3[tindx]],
               z_cannon_mesh[cannon_t3[tindx]]);
  }
  glEnd();
}

static void cannonBall(void) {
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
