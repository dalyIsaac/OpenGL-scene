#include <GL/freeglut.h>

extern GLuint txId[8];
extern GLUquadric *q;

extern float red[4];
extern float green[4];

extern float lx;
extern float ly;
extern float lz;
extern float light_pos[4];
extern float shadowMat[16];

void normal(int tindx);
void normal(float x1, float y1, float z1, float x2, float y2, float z2,
            float x3, float y3, float z3);
