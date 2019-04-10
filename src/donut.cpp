#include "main.h"
#include "sweepSurfaces.h"
#include <GL/freeglut.h>

/**
 * @brief The number of vertices inside the donut's base sweep surface.
 *
 */
const int N = 2;

/**
 * @brief The components for the vertices in the x-axis for the donut's base
 * sweep surface.
 *
 */
static float x_points[N] = {5, 5};

/**
 * @brief The components for the vertices in the y-axis for the donut's base
 * sweep surface.
 *
 */
static float y_points[N] = {0, 1};

/**
 * @brief The components for the vertices in the z-axis for the donut's base
 * sweep surface.
 *
 */
static float z_points[N] = {0};

/**
 * @brief Draws the donut, and translates it to the location where `GL_LIGHT0`
 * is being emitted from.
 *
 */
void donut(void) {
  glPushMatrix();

  glColor3f(0.317f, 0.301f, 0.501f);
  glTranslatef(lx, ly, lz);
  glRotated(90.0, 0.0, 0.0, 1.0);
  for (double i = 0.0; i < 360.0; i += 1.0) {
    glPushMatrix();
    glRotated(i, 1.0, 0.0, 0.0);
    glTranslated(0.0, 10.0, 10.0);
    sweepSurface(N, x_points, y_points, z_points);
    glPopMatrix();
  }

  glPopMatrix();
}
