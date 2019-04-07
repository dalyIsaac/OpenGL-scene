#include <GL/freeglut.h>

extern GLuint txId[3];
extern GLUquadric *q;

extern double spaceship_altitude;
extern bool spaceship_flying;
extern double spaceship_height;

extern bool ball_fired;
extern double ball_x;
extern double ball_y;
extern double ball_z;

extern int cannon_num_vertices;
extern int cannon_num_triangles;

extern double *x_cannon_mesh;
extern double *y_cannon_mesh;
extern double *z_cannon_mesh;

extern int *cannon_t1;
extern int *cannon_t2;
extern int *cannon_t3;

void normal(int tindx);
