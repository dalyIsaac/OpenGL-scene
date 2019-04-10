#include "main.h"
#include "sweepSurfaces.h"
#include <GL/freeglut.h>

const int N = 2;
static float x_points[N] = {5, 5};
static float y_points[N] = {0, 1};
static float z_points[N] = {0};

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
