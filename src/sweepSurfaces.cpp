#include "main.h"
#include <GL/freeglut.h>
#include <cmath>

/**
 * @brief Draws a sweep surface, using the given array of the components for the
 * x, y, and z-axes.
 *
 * @param num_vertices The number of vertices in the x, y, and z-axis arrays.
 * @param x The x-axis components for the vertices of the sweep surface.
 * @param y The y-axis components for the vertices of the sweep surface.
 * @param z The z-axis components for the vertices of the sweep surface.
 */
void sweepSurface(int num_vertices, float x[], float y[], float z[]) {
  float wx[num_vertices], wy[num_vertices], wz[num_vertices];
  float theta = -0.1745;

  glBegin(GL_TRIANGLE_STRIP);
  for (int j = 0; j < 36; j++) {
    for (int i = 0; i < num_vertices; i++) {
      wx[i] = x[i] * cos(theta) + z[i] * sin(theta);
      wy[i] = y[i];
      wz[i] = -x[i] * sin(theta) + z[i] * cos(theta);
    }

    for (int i = 0; i < num_vertices; i++) {
      if (i > 0) {
        normal(wx[i - 1], wy[i - 1], wz[i - 1], x[i - 1], y[i - 1], z[i - 1],
               x[i], y[i], z[i]);
      }
      glTexCoord2f(j / 36.0, i / float(num_vertices));
      glVertex3f(x[i], y[i], z[i]);
      if (i > 0) {
        normal(wx[i - 1], wy[i - 1], wz[i - 1], x[i], y[i], z[i], wx[i], wy[i],
               wz[i]);
      }
      glTexCoord2f((j + 1) / 36.0, i / float(num_vertices));
      glVertex3f(wx[i], wy[i], wz[i]);
    }

    for (int i = 0; i < num_vertices; i++) {
      x[i] = wx[i];
      y[i] = wy[i];
      z[i] = wz[i];
    }
  }
  glEnd();
}
