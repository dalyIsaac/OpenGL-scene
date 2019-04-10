#include "trampoline.h"
#include "castle.h"
#include "sweepSurfaces.h"
#include <GL/freeglut.h>
#include <cmath>

/**
 * @brief The x-axis component of the location of the trampoline (i.e. the
 * position where the trampoline is translated to).
 *
 */
const float tramp_x = 30.0;

/**
 * @brief The y-axis component of the location of the trampoline (i.e. the
 * position where the trampoline is translated to).
 *
 */
const float tramp_y = 0.0;

/**
 * @brief The z-axis component of the location of the trampoline (i.e. the
 * position where the trampoline is translated to).
 *
 */
const float tramp_z = 30.0;

/**
 * @brief Used in determining the denominator for the trampoline's bounce.
 *
 */
float tramp_time = 1.0;

/**
 * @brief True if the trampoline is rising.
 *
 */
bool tramp_rising = true;

/**
 * @brief The number of vertices inside the base curve for the trampoline.
 *
 */
static const int base_n = 2;

/**
 * @brief The x-axis components of the vertices for the base curve for the
 * trampoline.
 *
 */
static float base_x[base_n] = {5, 5};

/**
 * @brief The y-axis components of the vertices for the base curve for the
 * trampoline.
 *
 */
static float base_y[base_n] = {0, 5};

/**
 * @brief The z-axis components of the vertices for the base curve for the
 * trampoline.
 *
 */
static float base_z[base_n] = {0};

/**
 * @brief The number of vertices for the fabric of the trampoline.
 *
 */
const int fabric_n = 6;

/**
 * @brief The x-axis components of the vertices for the fabric curve for the
 * trampoline.
 *
 */
static float fabric_x[fabric_n] = {0, 1, 2, 3, 4, 5};

/**
 * @brief The y-axis components of the vertices for the fabric curve for the
 * trampoline.
 *
 */
static float fabric_y[fabric_n] = {0};

/**
 * @brief The z-axis components of the vertices for the fabric curve for the
 * trampoline.
 *
 */
static float fabric_z[fabric_n] = {0};

/**
 * @brief Pointer to the current minimum value for the fabric on the y-axis.
 *
 */
const float *fabric_y_bottom = &fabric_y[0];

/**
 * @brief Draws the fabric of the trampoline.
 *
 */
void fabric(void) {
  float base = pow(tramp_time + 5.0, 2.0);
  float h = base - 25; // time ^ 2 - 5 ^ 2
  for (int x = 0; x < fabric_n; x++) {
    fabric_y[x] = (x * x + h) / base;
  }

  glPushMatrix();
  glColor3f(0.968, 0.231, 0.913);
  glScaled(1.0, 5.0, 1.0);
  sweepSurface(fabric_n, fabric_x, fabric_y, fabric_z);
  glPopMatrix();
}

/**
 * @brief Changes the current time for the curve which dictates the delta for
 * the trampoline's fabric's curve.
 *
 */
void trampTimer(void) {
  if (tramp_time >= 15.0) {
    tramp_rising = false;
    tramp_time = 15.0;
  } else if (tramp_time <= 0.0) {
    tramp_rising = true;
    tramp_time = 0.0;
  }

  if (tramp_rising) {
    tramp_time++;
  } else {
    tramp_time--;
  }
}

/**
 * @brief Draws the trampoline.
 *
 */
void trampoline(void) {
  glPushMatrix();
  glTranslated(tramp_x, tramp_y, tramp_z);

  // Tramp base
  sweepSurface(base_n, base_x, base_y, base_z);

  fabric();

  glPopMatrix();
}
