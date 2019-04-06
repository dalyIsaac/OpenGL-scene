typedef struct robot_props {
  double angle;
  double limb_angle;
  double x;
  double y;
  double z;
  void (*movement)(robot_props *robot);
} Robot;

void draw_robot(Robot robot);

const int ROBOTS_LENGTH = 1;

extern Robot robots[ROBOTS_LENGTH];
