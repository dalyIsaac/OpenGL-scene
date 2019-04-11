#include "robot.h"
#include "castle.h"
#include "trampoline.h"
#include <GL/freeglut.h>
#include <cmath>

/**
 * @brief The current denominator used for scaling the y-axis position of robot
 * 2, who is bouncing on the trampoline.
 *
 */
static const float tramp_denom = 8;

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
  glRotated(robot.right_leg_angle, 1, 0, 0);
  glTranslated(0, -1.8, 0);
  glScaled(1, 4.4, 1);
  glutSolidCube(1);
  glPopMatrix();

  // Left leg
  glColor3d(0.0, 0.0, 1.);
  glPushMatrix();
  glTranslated(0.8, 4.0, 0.0);
  glRotated(robot.left_leg_angle, 1, 0, 0);
  glTranslated(0, -1.8, 0);
  glScaled(1, 4.4, 1);
  glutSolidCube(1);
  glPopMatrix();

  // Right arm
  glColor3d(0.0, 0.0, 1.);
  glPushMatrix();
  glTranslated(-2, 6.5, 0);
  glRotated(robot.right_arm_angle, 1, 0, 0);
  glTranslated(0, -1.5, 0);
  glScaled(1, 4, 1);
  glutSolidCube(1);
  glPopMatrix();

  // Left arm
  glColor3d(0.0, 0.0, 1.);
  glPushMatrix();
  glTranslated(2, 6.5, 0);
  glRotated(robot.left_arm_angle, 1, 0, 0);
  glTranslated(0, -1.5, 0);
  glScaled(1, 4, 1);
  glutSolidCube(1);
  glPopMatrix();

  glLightfv(robot.light, GL_POSITION, light_pos);
  glLightfv(robot.light, GL_SPOT_DIRECTION, spot_dir);
  glPopMatrix();
}

/**
 * @brief Modifies the angles of the limbs of a robot, so that they move in a
 * symmetrical and human-like fashion.
 *
 * @param robot Pointer to the robot, whose limbs' angles will be modified.
 * @param delta The change in the angle of the robot's limbs' angles.
 */
static void symmetrical_robot_limb_movement(Robot *robot, double delta) {
  if (robot->right_leg_angle >= 45.0) {
    robot->right_leg_moving_forward = false;
  } else if (robot->right_leg_angle <= -45.0) {
    robot->right_leg_moving_forward = true;
  }
  if (robot->right_leg_moving_forward) {
    robot->left_leg_angle -= delta;
    robot->left_arm_angle += delta;
    robot->right_leg_angle += delta;
    robot->right_arm_angle -= delta;
  } else {
    robot->left_leg_angle += delta;
    robot->left_arm_angle -= delta;
    robot->right_leg_angle -= delta;
    robot->right_arm_angle += delta;
  }
}

/**
 * @brief Moves robot 0 on a patrol around the exterior of the castle.
 *
 * @param robot
 */
static void robot_0_movement(Robot *robot) {
  symmetrical_robot_limb_movement(robot, 15.0);
  if (robot->x == 50.0 && robot->z <= 50.0 && robot->z > -50.0) {
    // Move up
    robot->z--;
    robot->direction_angle = 180.0;
  } else if (robot->x <= 50.0 && robot->x > -50.0 && robot->z == -50.0) {
    // Move to the left
    robot->x--;
    robot->direction_angle = 270.0;
  } else if (robot->x == -50.0 && robot->z >= -50.0 && robot->z < 50.0) {
    // Move down
    robot->z++;
    robot->direction_angle = 0.0;
  } else if (robot->x >= -50.0 && robot->x < 50.0 && robot->z == 50.0) {
    // Move to the right
    robot->x++;
    robot->direction_angle = 90.0;
  }
}

/**
 * @brief Moves robot 1 on a patrol around the spaceship, with brief forays
 * outside of the castle to take a look.
 *
 * @param robot
 */
static void robot_1_movement(Robot *robot) {
  static RobotMovement movement = RobotMovement::In;

  symmetrical_robot_limb_movement(robot, 4.0);
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

      if (robot->z >= 45.0) {
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

/**
 * @brief Dictates the movements of robot 2 so that he bounces on a trampoline.
 *
 * @param robot
 */
static void robot_2_movement(Robot *robot) {
  float x = tramp_time - 7.5;
  float y = -((x - 7.5) / tramp_denom) * ((x + 7.5) / tramp_denom);

  float proposed_relative_y = robot->y + (tramp_rising ? y : -y) - tramp_y;
  float current_fabric_y = (*fabric_y_bottom) * 5.0f;
  if (proposed_relative_y < current_fabric_y) {
    robot->y = current_fabric_y + tramp_y;
  } else {
    robot->y = proposed_relative_y + tramp_y;
  }

  if (tramp_rising) {
    robot->left_leg_angle += 3;
    robot->right_leg_angle += 3;

    robot->left_arm_angle -= 12;
    robot->right_arm_angle -= 12;
  } else {
    robot->left_leg_angle -= 3;
    robot->right_leg_angle -= 3;

    robot->left_arm_angle += 12;
    robot->right_arm_angle += 12;
  }
}

/**
 * @brief Array of ll the robots, and their associated attributes.
 *
 */
Robot robots[NUM_ROBOTS] = {{
                              x : 0.0,
                              y : 0.0,
                              z : 50.0,
                              direction_angle : 0.0,
                              left_leg_angle : 0.0,
                              right_leg_angle : 0.0,
                              left_arm_angle : 0.0,
                              right_arm_angle : 0.0,
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
                              z : 45.0,
                              direction_angle : 180.0,
                              left_leg_angle : 0.0,
                              right_leg_angle : 0.0,
                              left_arm_angle : 0.0,
                              right_arm_angle : 0.0,
                              right_leg_moving_forward : true,
                              rotation_angle : 0.0,
                              rotation_translation_x : 0.0,
                              rotation_translation_y : 0.0,
                              rotation_translation_z : 0.0,
                              light : GL_LIGHT4,
                              movement : robot_1_movement
                            },
                            {
                              x : tramp_x,
                              y : tramp_y,
                              z : tramp_z,
                              direction_angle : 180.0,
                              left_leg_angle : -30.0,
                              right_leg_angle : -30.0,
                              left_arm_angle : 0.0,
                              right_arm_angle : 0.0,
                              right_leg_moving_forward : true,
                              rotation_angle : 0.0,
                              rotation_translation_x : 0.0,
                              rotation_translation_y : 0.0,
                              rotation_translation_z : 0.0,
                              light : GL_LIGHT5,
                              movement : robot_2_movement
                            }};
