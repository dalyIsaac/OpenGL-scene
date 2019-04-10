#include "robot.h"
#include <GL/freeglut.h>
#include <cmath>

/**
 * @brief Draws a single robot.
 *
 */
void draw_robot(Robot robot) {
  float light_pos[] = {0.0f, 7.7f, 0.0f, 1.0f};
  float spot_dir[] = {0.0f, -10.0f, 10.0f, 0.0f};

  glPushMatrix();
  glTranslated(robot.rotation_translation_x, robot.rotation_translation_y,
               robot.rotation_translation_z);
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
  glTranslated(0, -1.8, 0);
  glScaled(1, 4.4, 1);
  glutSolidCube(1);
  glPopMatrix();

  // Left leg
  glColor3d(0.0, 0.0, 1.);
  glPushMatrix();
  glTranslated(0.8, 4.0, 0.0);
  glRotated(robot.limb_angle, 1, 0, 0);
  glTranslated(0, -1.8, 0);
  glScaled(1, 4.4, 1);
  glutSolidCube(1);
  glPopMatrix();

  // Right arm
  glColor3d(0.0, 0.0, 1.);
  glPushMatrix();
  glTranslated(-2, 6.5, 0);
  glRotated(robot.limb_angle, 1, 0, 0);
  glTranslated(0, -1.5, 0);
  glScaled(1, 4, 1);
  glutSolidCube(1);
  glPopMatrix();

  // Left arm
  glColor3d(0.0, 0.0, 1.);
  glPushMatrix();
  glTranslated(2, 6.5, 0);
  glRotated(-robot.limb_angle, 1, 0, 0);
  glTranslated(0, -1.5, 0);
  glScaled(1, 4, 1);
  glutSolidCube(1);
  glPopMatrix();

  glLightfv(robot.light, GL_POSITION, light_pos);
  glLightfv(robot.light, GL_SPOT_DIRECTION, spot_dir);
  glPopMatrix();
}

static void robot_limb_movement(Robot *robot, double delta) {
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

static void robot_0_movement(Robot *robot) {
  robot_limb_movement(robot, 15.0);
  if (robot->x == 30.0 && robot->z <= 30.0 && robot->z > -30.0) {
    // Move up
    robot->z--;
    robot->direction_angle = 180.0;
  } else if (robot->x <= 30.0 && robot->x > -30.0 && robot->z == -30.0) {
    // Move to the left
    robot->x--;
    robot->direction_angle = 270.0;
  } else if (robot->x == -30.0 && robot->z >= -30.0 && robot->z < 30.0) {
    // Move down
    robot->z++;
    robot->direction_angle = 0.0;
  } else if (robot->x >= -30.0 && robot->x < 30.0 && robot->z == 30.0) {
    // Move to the right
    robot->x++;
    robot->direction_angle = 90.0;
  }
}

static void robot_1_movement(Robot *robot) {
  static RobotMovement movement = RobotMovement::In;

  robot_limb_movement(robot, 4.0);
  switch (movement) {
    case RobotMovement::In:
      robot->z -= 0.25;

      if (robot->z <= 15.0 && robot->z > 10.0) {
        robot->direction_angle = fmod(robot->direction_angle + 4.5, 360.0);
      }

      if (robot->x == 0.0 && robot->z == 10.0) {
        movement = RobotMovement::Rotating;
      }
      break;
    case RobotMovement::Rotating:
      robot->rotation_angle--;

      if (fmod(robot->rotation_angle, 90.0) <= -60.0) {
        robot->direction_angle = fmod(robot->direction_angle + 3.0, 360.0);
      }

      if (robot->direction_angle == 0.0) {
        movement = RobotMovement::Out;
      }
      break;
    case RobotMovement::Out:
      robot->z += 0.25;

      if (robot->z >= 25.0) {
        movement = RobotMovement::ChangingDirection;
      }
      break;
    case RobotMovement::ChangingDirection:
      robot->direction_angle = fmod(robot->direction_angle + 3.0, 360.0);

      if (robot->direction_angle == 180.0) {
        movement = RobotMovement::In;
      }
      break;
  }
}

void robot_2_movement(Robot *robot) {}

Robot robots[NUM_ROBOTS] = {{
                              x : 0.0,
                              y : 0.0,
                              z : 30.0,
                              direction_angle : 0.0,
                              limb_angle : 0.0,
                              right_leg_moving_forward : true,
                              rotation_angle : 0.0,
                              rotation_translation_x : 0.0,
                              rotation_translation_y : 0.0,
                              rotation_translation_z : 0.0,
                              light : GL_LIGHT3,
                              movement : robot_0_movement
                            },
                            {
                              x : 0.0,
                              y : 0.0,
                              z : 25.0,
                              direction_angle : 180.0,
                              limb_angle : 0.0,
                              right_leg_moving_forward : true,
                              rotation_angle : 0.0,
                              rotation_translation_x : 0.0,
                              rotation_translation_y : 0.0,
                              rotation_translation_z : 0.0,
                              light : GL_LIGHT4,
                              movement : robot_1_movement
                            },
                            {
                              x : 0.0,
                              y : 0.0,
                              z : -60.0,
                              direction_angle : 180.0,
                              limb_angle : 0.0,
                              right_leg_moving_forward : true,
                              rotation_angle : 0.0,
                              rotation_translation_x : 0.0,
                              rotation_translation_y : 0.0,
                              rotation_translation_z : 0.0,
                              light : GL_LIGHT5,
                              movement : robot_2_movement
                            }};
