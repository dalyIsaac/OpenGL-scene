#include <GL/freeglut.h>

extern GLuint txId[3];
extern GLUquadric *q;

extern float red[4];
extern float green[4];

extern float lx;
extern float ly;
extern float lz;
extern float light_pos[4];
extern float shadowMat[16];

void normal(int tindx);
