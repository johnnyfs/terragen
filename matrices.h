#ifndef MATRICES_H
#define MATRICES_H

#include <stdio.h>

#include <GL/glew.h>
#include <GL/gl.h>

#include "vectors.h"

struct frame;

void camera(const vec3 *eye, const vec3 *up, const vec3 *focus, GLfloat m[16]);
void matrix4_mult(GLfloat out[16], const GLfloat a[16], const GLfloat b[16]);
void matrix4_mult_vec4(vec4 *out, const GLfloat m[16], const vec4 *v);
void matrix4_print(FILE *stream, const GLfloat m[16]);
void matrix4_rot(const GLfloat m[16], float pitch, float yaw, float roll);
void matrix4_yrot(GLfloat m[16], float t);
void matrix4_zrot(GLfloat m[16], float t);

/* Creates a camera matrix from the given frame. */
void matrix4_frame(GLfloat out[16], const struct frame *frame);
static inline void matrix4_trans(GLfloat out[16], GLfloat x, GLfloat y, GLfloat z)
{
    out[0] = out[5] = out[10] = out[15] = 1.0f;
    out[1] = out[2] = out[3] = out[4] = 0.0f;
    out[6] = out[7] = out[8] = out[9] = out[11] = 0.0f;
    out[12] = x;
    out[13] = y;
    out[14] = z;
}

#endif //!MATRICES_H
