#include "cannon.h"
#include "main.h"
#include <GL/freeglut.h>
#include <cmath>

bool ball_fired = false;

double ball_x_initial = 0.0;
double ball_y_initial = 4.8;
double ball_z_initial = 54.5;

double ball_x = ball_x_initial;
double ball_y = ball_y_initial;
double ball_z = ball_z_initial;

int cannon_num_vertices;
int cannon_num_triangles;

double *x_cannon_mesh;
double *y_cannon_mesh;
double *z_cannon_mesh;

int *cannon_t1;
int *cannon_t2;
int *cannon_t3;

static double cannon_angle = 15.0;
static double gradient = tan(cannon_angle * M_PI / 180);
static double time = ball_z_initial;
static double delta_z = 2.0;
static bool ball_moving = true;

static const double power = 200.0; // increase power -> decrease in constant
static const double constant = 1 / power;

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
  glRotated(cannon_angle, 0, 0, 1);
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

void cannonBallPhysics(void) {
  if (ball_moving) {
    time += delta_z;
    delta_z -= 0.02;

    if (delta_z <= 0.0) {
      ball_moving = false;
    }

    double z = time;
    double reused = gradient + 2 * constant * ball_z_initial;
    ball_y = -(constant * pow(z, 2.0)) + reused * z + ball_y_initial +
             constant * pow(ball_z_initial, 2.0) - reused * ball_z_initial;
    if (ball_y <= 0.0) {
      time = ball_z_initial;
      ball_y_initial = 0;
      ball_z_initial = ball_z;
    } else {
      ball_z = z;
    }
  }
}
