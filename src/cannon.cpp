#include "cannon.h"
#include "main.h"
#include <GL/freeglut.h>
#include <cmath>

/**
 * @brief Indicates if the ball has being fired.
 *
 */
bool ball_fired = false;

/**
 * @brief The radius of the ball.
 *
 */
static double ball_radius = 0.7;

/**
 * @brief The initial position of the ball in the x-axis.
 *
 */
double ball_x_initial = 0.0;

/**
 * @brief The initial position of the ball in the y-axis.
 *
 */
double ball_y_initial = 4.6;

/**
 * @brief The initial position of the ball in the z-axis.
 *
 */
double ball_z_initial = 74.5;

/**
 * @brief The current location of the ball in the x-axis.
 *
 */
double ball_x = ball_x_initial;

/**
 * @brief The current location of the ball in the y-axis.
 *
 */
double ball_y = ball_y_initial;

/**
 * @brief The current location of the ball in the z-axis.
 *
 */
double ball_z = ball_z_initial;

/**
 * @brief The number of vertices that the cannon mesh has.
 *
 */
int cannon_num_vertices;

/**
 * @brief The number of triangles that the cannon mesh has.
 *
 */
int cannon_num_triangles;

/**
 * @brief Pointer to the cannon mesh array for the x-axis.
 *
 */
double *x_cannon_mesh;

/**
 * @brief Pointer to the cannon mesh array for the y-axis.
 *
 */
double *y_cannon_mesh;

/**
 * @brief Pointer to the cannon mesh array for the z-axis.
 *
 */
double *z_cannon_mesh;

/**
 * @brief Triangles for the cannon mesh.
 *
 */
int *cannon_t1;

/**
 * @brief Triangles for the cannon mesh.
 *
 */
int *cannon_t2;

/**
 * @brief Triangles for the cannon mesh.
 *
 */
int *cannon_t3;

/**
 * @brief The angle of the cannon, in degrees.
 *
 */
static double cannon_angle = 15.0;

/**
 * @brief The gradient of the line formed by the cannon's current angle.
 *
 */
static double gradient = tan(cannon_angle * M_PI / 180);

/**
 * @brief The next change to be applied to `ball_z`.
 *
 */
static double delta_z = 4.0;

/**
 * @brief The amount that `delta_z` is going to be decremented by.
 *
 */
static double decr = 0.03;

/**
 * @brief Indicates if the cannonball is moving, after the cannon has fired.
 * Should be ignored before the cannon has being fired (i.e. if `ball_moving` is
 * true, but the cannon hasn't been fired, then it should be ignored).
 *
 */
static bool ball_moving = true;

/**
 * @brief Indicates that the ball is bouncing for the last time. After its next
 * impact with the ground, it will start rolling.
 *
 */
static bool ball_ending = false;

/**
 * @brief Indicates if the ball is currently rolling along the ground.
 *
 */
static bool ball_rolling = false;

/**
 * @brief The power value for the cannon.
 *
 */
static const double power = 400.0; // increase power -> decrease in constant

/**
 * @brief The constant which is used inside the cannonball's physics model. If
 * there's an increase in power, the constant decreases.
 *
 */
static const double constant = 1 / power;

/**
 * @brief Draws the cannon using data from the mesh file.
 *
 */
static void drawCannonFromMesh(void) {
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

/**
 * @brief Draws the cannonball, and its shadow.
 *
 */
static void cannonball(void) {
  // Cannonball
  glPushMatrix();
  glTranslated(ball_x, ball_y + 0.2, ball_z);
  glutSolidSphere(ball_radius, 36, 18);
  glPopMatrix();

  // Shadow cannonball
  glDisable(GL_LIGHTING);
  glPushMatrix();
  glMultMatrixf(shadow_mat);
  glTranslated(ball_x, ball_y, ball_z);
  glColor4f(0.2, 0.2, 0.2, 1.0);
  glutSolidSphere(ball_radius, 36, 18);
  glPopMatrix();
  glEnable(GL_LIGHTING);
}

/**
 * @brief Draws a cannon.
 *
 * @param isShadow Indicates that the cannon being drawn is a shadow.
 */
static void _cannon(bool isShadow) {
  glPushMatrix();
  if (isShadow) {
    glDisable(GL_LIGHTING);
    glMultMatrixf(shadow_mat);
  }

  // Global transitions
  glTranslated(0.0, 0.0, 70.0);
  glRotated(-90.0, 0.0, 1.0, 0.0);
  glScaled(0.1, 0.1, 0.1);

  // Cannon
  glPushMatrix();
  glTranslated(-20.0, 30.0, 0);
  glRotated(cannon_angle, 0, 0, 1);
  glTranslated(20.0, -30.0, 0);
  drawCannonFromMesh();
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

  if (isShadow) {
    glColor4f(0.2, 0.2, 0.2, 1.0);
  }

  glPopMatrix();
  glEnable(GL_LIGHTING);
}

/**
 * @brief Draws the castle, and it's constituent supports.
 *
 */
void cannon(void) {
  _cannon(true);
  _cannon(false);

  cannonball();
}

/**
 * @brief Returns the ball's next y-value.
 *
 * @return double
 */
static double getBallY(void) {
  double reused = gradient + 2 * constant * ball_z_initial;
  double new_y = -(constant * pow(ball_z, 2.0)) + reused * ball_z +
                 ball_y_initial + constant * pow(ball_z_initial, 2.0) -
                 reused * ball_z_initial;
  return new_y;
}

/**
 * @brief Defines the behavior of the cannonball.
 *
 */
void cannonballPhysics(void) {
  ball_z += delta_z;
  if (ball_rolling) {
    ball_y = ball_radius;
    delta_z -= decr;
    if (delta_z <= 0.0) {
      ball_rolling = false;
      ball_moving = false;
    }
  } else if (ball_moving) {
    if (ball_ending == false) {
      delta_z -= decr;
    }

    // Used to detect when delta_z is less than a minimum value. If delta_z is
    // below this value, then to prevent the ball from hanging in the air, the
    // decrease in delta_z is temporarily halted until the ball hits the ground.
    // Afterwards, it rolls to a stop.
    if (delta_z <= 1.25) {
      ball_ending = true;
    }

    ball_y = getBallY();
    if (ball_y <= 0.0) {
      if (ball_ending) {
        ball_rolling = true;
      } else {
        ball_z_initial = ball_z + 0.1;
        ball_z = ball_z_initial + delta_z;
        ball_y_initial = 0.0;
        ball_y = 0.5;
      }
    }
  }
}
