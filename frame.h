#ifndef FRAME_H
#define FRAME_H

#include "vectors.h"
#include "matrices.h"

struct frame {
    vec3 pos;
    vec3 up;

    float pitch, yaw, roll;
};

static inline void
frame_init(struct frame *frame, float x, float y, float z)
{
    frame->pos.x = x;
    frame->pos.y = y;
    frame->pos.z = z;

    frame->up.x = 0.0f;
    frame->up.y = 1.0f;
    frame->up.z = 0.0f;

    frame->pitch = frame->yaw = frame->roll = 0.0f;
}

static inline void
frame_advance(struct frame *frame, float dpos)
{
    GLfloat rot[16];
    vec4 forward = { 0.0f, 0.0f, -dpos, 1.0f }, out;

    matrix4_yrot(rot, frame->yaw);
    matrix4_mult_vec4(&out, rot, &forward);
    vec3scale((vec3 *)&out, 1.0f/out.w);
    vec3iadd(&frame->pos, (const vec3*)&out);
}

static inline void
frame_strafe(struct frame *frame, float dpos)
{
  GLfloat rot[16];
  vec4 side = { -dpos, 0.0f, 0.0f, 1.0f }, out;

  matrix4_yrot(rot, frame->yaw);
  matrix4_mult_vec4(&out, rot, &side);
  vec3scale((vec3 *)&out, 1.0f/out.w);
  vec3iadd(&frame->pos, (const vec3*)&out);
}

static inline void
frame_elevate(struct frame *frame, float dy)
{
  GLfloat rot[16];
  vec4 dpos = { 0.0f, dy, 0.0f, 1.0f};

  vec3iadd(&frame->pos, (vec3 *)&dpos);
}

#endif //FRAME_H
