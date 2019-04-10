#include <GL/freeglut.h>

extern GLuint txId[8];
extern GLUquadric *q;

extern const float red[4];
extern const float green[4];
extern const float yellow[4];

extern const float lx;
extern const float ly;
extern const float lz;
extern const float light_pos[4];
extern float shadow_mat[16];

void normal(int tindx);
void normal(float x1, float y1, float z1, float x2, float y2, float z2,
            float x3, float y3, float z3);
