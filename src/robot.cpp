#include "robot.h"
#include <GL/freeglut.h>

/**
 * @brief Draws a single robot.
 *
 */
void draw_robot(Robot robot) {
  glPushMatrix();
  glTranslated(robot.x, robot.y, robot.z);
  glRotated(robot.angle, 0.0, 1.0, 0.0);

  // Head
  glColor3d(1.0, 0.78, 0.06);
  glPushMatrix();
  glTranslated(0, 7.7, 0);
  glutSolidCube(1.4);
  glPopMatrix();

  // Torso
  glColor3d(1.0, 0.0, 0.);
  glPushMatrix();
  glTranslated(0, 5.5, 0);
  glScaled(3, 3, 1.4);
  glutSolidCube(1);
  glPopMatrix();

  // Right leg
  glColor3d(0.0, 0.0, 1.);
  glPushMatrix();
  glTranslated(-0.8, 4.0, 0);
  glRotated(-robot.limb_angle, 1, 0, 0);
  glTranslated(0.8, -4.0, 0);
  glTranslated(-0.8, 2.2, 0);
  glScaled(1, 4.4, 1);
  glutSolidCube(1);
  glPopMatrix();

  // Left leg
  glColor3d(0.0, 0.0, 1.);
  glPushMatrix();
  glTranslated(0.8, 4.0, 0.0);
  glRotated(robot.limb_angle, 1, 0, 0);
  glTranslated(-0.8, -4, 0);
  glTranslated(0.8, 2.2, 0);
  glScaled(1, 4.4, 1);
  glutSolidCube(1);
  glPopMatrix();

  // Right arm
  glColor3d(0.0, 0.0, 1.);
  glPushMatrix();
  glTranslated(-2, 6.5, 0);
  glRotated(robot.limb_angle, 1, 0, 0);
  glTranslated(2, -6.5, 0);
  glTranslated(-2, 5, 0);
  glScaled(1, 4, 1);
  glutSolidCube(1);
  glPopMatrix();

  // Left arm
  glColor3d(0.0, 0.0, 1.);
  glPushMatrix();
  glTranslated(2, 6.5, 0);
  glRotated(-robot.limb_angle, 1, 0, 0);
  glTranslated(-2, -6.5, 0);
  glTranslated(2, 5, 0);
  glScaled(1, 4, 1);
  glutSolidCube(1);
  glPopMatrix();

  glPopMatrix();
}

void robot_0_movement(Robot *robot) {
  robot->limb_angle += 15;
  if (robot->limb_angle >= 45.0) {
    robot->limb_angle = -45.0;
  }
  if (robot->x == 30.0 && robot->z <= 10.0 && robot->z > -50) {
    // Move up
    robot->z--;
    robot->angle = 180.0;
  } else if (robot->x <= 30.0 && robot->x > -30.0 && robot->z == -50.0) {
    // Move to the left
    robot->x--;
    robot->angle = 270.0;
  } else if (robot->x == -30.0 && robot->z >= -50.0 && robot->z < 10.0) {
    // Move down
    robot->z++;
    robot->angle = 0.0;
  } else if (robot->x >= -30.0 && robot->x < 30.0 && robot->z == 10.0) {
    // Move to the right
    robot->x++;
    robot->angle = 90.0;
  }
}

Robot robots[ROBOTS_LENGTH] = {{0.0, 0.0, 0.0, 0.0, 10.0, robot_0_movement}};
