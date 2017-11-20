#ifndef VECTORS_H
#define VECTORS_H

#include <math.h>
#include <stdio.h>

#include <GL/glew.h>
#include <GL/gl.h>

typedef struct vec3 {
    GLfloat x, y, z;
} vec3;

typedef struct vec4 {
    GLfloat x, y, z, w;
} vec4;

static inline void
vec3iadd(vec3 * restrict self, const vec3 * restrict v)
{
    self->x += v->x;
    self->y += v->y;
    self->z += v->z;
}

static inline void
vec3add(vec3 * restrict out, const vec3 * restrict a, const vec3 * restrict b)
{
  out->x = a->x + b->x;
  out->y = a->y + b->y;
  out->z = a->z + b->z;
}

static inline void
vec3sub(vec3 * restrict dst, const vec3 * restrict s1, const vec3 * restrict s2)
{
    dst->x = s1->x - s2->x;
    dst->y = s1->y - s2->y;
    dst->z = s1->z - s2->z;
}

static inline GLfloat
vec3mag(vec3 * v)
{
    return sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
}

static inline void
vec3scale(vec3 *v, GLfloat s)
{
    v->x *= s;
    v->y *= s;
    v->z *= s;
}

static inline void
vec4scale(vec4 *v, GLfloat s)
{
    v->x *= s;
    v->y *= s;
    v->z *= s;
    v->w *= s;
}

static inline void
vec3normalize(vec3 *v)
{
    GLfloat m = vec3mag(v);
    return vec3scale(v, 1.0f / m);
}

static inline void
vec3cross(vec3 * restrict out, const vec3 * restrict v1, const vec3 * restrict v2)
{
    out->x = v1->y * v2->z - v2->y * v1->z;
    out->y = v1->z * v2->x - v2->z * v1->x;
    out->z = v1->x * v2->y - v2->x * v1->y;
}

static inline void
vec3print(FILE *stream, const vec3 *v)
{
    fprintf(stream, "(%.6f, %.6f, %.6f)\n", v->x, v->y, v->z);
}

static inline void
vec4print(FILE *stream, const vec4 *v)
{
    fprintf(stream, "(%.6f, %.6f, %.6f, %.6f)\n", v->x, v->y, v->z, v->w);
}

#endif //!VECTORS_H
