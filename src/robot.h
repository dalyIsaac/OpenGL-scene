typedef struct robot_props {
  double x;
  double y;
  double z;
  double direction_angle;
  double limb_angle;
  bool right_leg_moving_forward;
  double rotation_angle;
  double rotation_translation_x;
  double rotation_translation_y;
  double rotation_translation_z;
  int light;
  void (*movement)(robot_props *robot);
} Robot;

void draw_robot(Robot robot);

const int NUM_ROBOTS = 2;

extern Robot robots[NUM_ROBOTS];

enum RobotMovement { In, Out, Rotating };
