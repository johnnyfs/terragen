#include "matrices.h"

void
matrix4_identity(GLfloat m[16])
{
  m[0] = m[5] = m[10] = m[15] = 1.0f;
  m[1] = m[2] = m[3] = m[4] = 0.0f;
  m[6] = m[7] = m[8] = m[9] = 0.0f;
  m[11] = m[12] = m[13] = m[14] = 0.0f;
}

void
matrix4_mult(GLfloat out[16], const GLfloat a[16], const GLfloat b[16])
{
    out[0] = a[0] * b[0] + a[4] * b[1] + a[8]  * b[2] + a[12] * b[3];
    out[1] = a[1] * b[0] + a[5] * b[1] + a[9]  * b[2] + a[13] * b[3];
    out[2] = a[2] * b[0] + a[6] * b[1] + a[10] * b[2] + a[14] * b[3];
    out[3] = a[3] * b[0] + a[7] * b[1] + a[11] * b[2] + a[15] * b[3];

    out[4] = a[0] * b[4] + a[4] * b[5] + a[8]  * b[6] + a[12] * b[7];
    out[5] = a[1] * b[4] + a[5] * b[5] + a[9]  * b[6] + a[13] * b[7];
    out[6] = a[2] * b[4] + a[6] * b[5] + a[10] * b[6] + a[14] * b[7];
    out[7] = a[3] * b[4] + a[7] * b[5] + a[11] * b[6] + a[15] * b[7];

    out[8]  = a[0] * b[8] + a[4] * b[9] + a[8]  * b[10] + a[12] * b[11];
    out[9]  = a[1] * b[8] + a[5] * b[9] + a[9]  * b[10] + a[13] * b[11];
    out[10] = a[2] * b[8] + a[6] * b[9] + a[10] * b[10] + a[14] * b[11];
    out[11] = a[3] * b[8] + a[7] * b[9] + a[11] * b[10] + a[15] * b[11];

    out[12] = a[0] * b[12] + a[4] * b[13] + a[8]  * b[14] + a[12] * b[15];
    out[13] = a[1] * b[12] + a[5] * b[13] + a[9]  * b[14] + a[13] * b[15];
    out[14] = a[2] * b[12] + a[6] * b[13] + a[10] * b[14] + a[14] * b[15];
    out[15] = a[3] * b[12] + a[7] * b[13] + a[11] * b[14] + a[15] * b[15];
}

void
camera(const vec3 *eye, const vec3 *up, const vec3 *focus, GLfloat out[16])
{
    vec3 forward, right;
    GLfloat om[16], tm[16];

    vec3sub(&forward, eye, focus);
    vec3normalize(&forward);
    vec3cross(&right, &forward, up);

    om[0] = right.x; om[4] = right.y; om[8] = right.z; om[12] = 0.0f;
    om[1] = up->x; om[5] = up->y; om[9] = up->z; om[13] = 0.0f;
    om[2] = -forward.x; om[6] = -forward.y; om[10] = -forward.z; om[14] = 0.0f;
    om[3] = om[7] = om[11] = 0.0f; om[15] = 1.0f;

    tm[0] = tm[5] = tm[10] = tm[15] = 1.0f;
    tm[1] = tm[2] = tm[3] = tm[4] = 0.0f;
    tm[6] = tm[7] = tm[8] = tm[9] = tm[11] = 0.0f;
    tm[12] = -eye->x; tm[13] = -eye->y; tm[14] = -eye->z;

    matrix4_mult(out, om, tm);
}

void
matrix4_print(FILE *stream, const GLfloat m[16])
{
    fprintf(stream, "%.6f, %.6f, %.6f, %.6f\n", m[0], m[4], m[8],  m[12]);
    fprintf(stream, "%.6f, %.6f, %.6f, %.6f\n", m[1], m[5], m[9],  m[13]);
    fprintf(stream, "%.6f, %.6f, %.6f, %.6f\n", m[2], m[6], m[10], m[14]);
    fprintf(stream, "%.6f, %.6f, %.6f, %.6f\n", m[3], m[7], m[11], m[15]);
}

void
matrix4_mult_vec4(vec4 *out, const GLfloat m[16], const vec4 *v)
{
    out->x = m[0] * v->x + m[4] * v->y + m[8] * v->z + m[12] * v->w;
    out->y = m[1] * v->x + m[5] * v->y + m[9] * v->z + m[13] * v->w;
    out->z = m[2] * v->x + m[6] * v->y + m[10] * v->z + m[14] * v->w;
    out->w = m[3] * v->x + m[7] * v->y + m[11] * v->z + m[15] * v->w;
}

void matrix4_rot(const GLfloat m[16], float pitch, float yaw, float roll)
{

}

void matrix4_yrot(GLfloat m[16], float t)
{
  GLfloat ct = cos(t), st = sin(t);

  m[0] = ct;
  m[1] = 0.0f;
  m[2] = -st;
  m[3] = 0.0f;
  m[4] = 0.0f;
  m[5] = 1.0f;
  m[6] = 0.0f;
  m[7] = 0.0f;
  m[8] = st;
  m[9] = 0.0f;
  m[10] = ct;
  m[11] = 0.0f;
  m[12] = 0.0f;
  m[13] = 0.0f;
  m[14] = 0.0f;
  m[15] = 1.0f;
}

void matrix4_zrot(GLfloat m[16], float t)
{
  GLfloat ct = cos(t), st = sin(t);

  m[0] = ct;
  m[1] = st;
  m[2] = 0.0f;
  m[3] = 0.0f;

  m[4] = -st;
  m[5] = ct;
  m[6] = 0.0f;
  m[7] = 0.0f;

  m[8] = 0.0f;
  m[9] = 0.0f;
  m[10] = 1.0f;
  m[11] = 0.0f;

  m[12] = 0.0f;
  m[13] = 0.0f;
  m[14] = 0.0f;
  m[15] = 1.0f;
}
