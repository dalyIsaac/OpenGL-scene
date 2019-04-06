#include "robot.h"
#include <GL/freeglut.h>

/**
 * @brief Draws a single robot.
 *
 */
void draw_robot(Robot robot) {
  glPushMatrix();
  glTranslated(0.0, 0.0, robot.rotation_translation);
  glRotated(robot.rotation_angle, 0.0, 1.0, 0.0);
  glTranslated(robot.x, robot.y, robot.z);
  glRotated(robot.direction_angle, 0.0, 1.0, 0.0);

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

void robot_leg_movement(Robot *robot, double delta) {
  if (robot->limb_angle >= 45.0) {
    robot->right_leg_moving_forward = false;
  } else if (robot->limb_angle <= -45.0) {
    robot->right_leg_moving_forward = true;
  }
  if (robot->right_leg_moving_forward) {
    robot->limb_angle += delta;
  } else {
    robot->limb_angle -= delta;
  }
}

void robot_0_movement(Robot *robot) {
  robot_leg_movement(robot, 15.0);
  if (robot->x == 30.0 && robot->z <= 10.0 && robot->z > -50) {
    // Move up
    robot->z--;
    robot->direction_angle = 180.0;
  } else if (robot->x <= 30.0 && robot->x > -30.0 && robot->z == -50.0) {
    // Move to the left
    robot->x--;
    robot->direction_angle = 270.0;
  } else if (robot->x == -30.0 && robot->z >= -50.0 && robot->z < 10.0) {
    // Move down
    robot->z++;
    robot->direction_angle = 0.0;
  } else if (robot->x >= -30.0 && robot->x < 30.0 && robot->z == 10.0) {
    // Move to the right
    robot->x++;
    robot->direction_angle = 90.0;
  }
}

void robot_1_movement(Robot *robot) {
  static bool circular_path = true;
  robot_leg_movement(robot, 4.0);
  if (circular_path) {
    robot->rotation_angle -= 1;
    if (robot->rotation_angle <= 0.0) {
      robot->rotation_angle = 360.0;
    }
  } else {
    // TODO
  }
}

Robot robots[ROBOTS_LENGTH] = {{
                                 x : 0.0,
                                 y : 0.0,
                                 z : 10.0,
                                 direction_angle : 0.0,
                                 limb_angle : 0.0,
                                 right_leg_moving_forward : true,
                                 rotation_angle : 0.0,
                                 rotation_translation : 0.0,
                                 movement : robot_0_movement
                               },
                               {
                                 x : 00.0,
                                 y : 0.0,
                                 z : 10.0,
                                 direction_angle : 90.0,
                                 limb_angle : 0.0,
                                 right_leg_moving_forward : true,
                                 rotation_angle : 0.0,
                                 rotation_translation : -20.0,
                                 movement : robot_1_movement
                               }};
