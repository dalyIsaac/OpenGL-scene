#include "trampoline.h"
#include "sweepSurfaces.h"
#include <GL/freeglut.h>
#include <cmath>

static const int base_n = 2;
static float base_x[base_n] = {5, 5};
static float base_y[base_n] = {0, 5};
static float base_z[base_n] = {0};

static float fabric_x[fabric_n] = {0, 1, 2, 3, 4, 5};
float fabric_y[fabric_n] = {0};
static float fabric_z[fabric_n] = {0};

static const float denom = 8;

float tramp_time = 1.0;
bool tramp_rising = true;

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

void trampoline(void) {
  glPushMatrix();
  glTranslated(0.0, 0.0, -60.0);

  // Tramp base
  sweepSurface(base_n, base_x, base_y, base_z);

  fabric();

  glPopMatrix();
}
