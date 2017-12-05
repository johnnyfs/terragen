#ifndef SHAPES_H
#define SHAPES_H

static void
cube(GLfloat s)
{
  const GLfloat vertices[] = {
    /* Front face CW from top left */
    -s,  s, -s, // 0 ULF
     s,  s, -s, // 1 URF
     s, -s, -s, // 2 LRF
    -s, -s, -s, // 3 LLF

    /* Rear face CW from top left */
    -s,  s,  s, // 0 ULB
     s,  s,  s, // 1 URB
     s, -s,  s, // 2 LRB
    -s, -s,  s  // 3 LLB
  };
  const GLuint indeces[] = {
    /* Front */
    0, 3, 1,    // UL, LL, UR
    1, 3, 2,    // UR, LL, LR
    /* Right */
    1, 2, 5,    // UR, LR, RUR
    5, 2, 6,    // RUR, LR, RLR
    /* Rear */
    4, 5, 7,    // RUL, RUR, RLL
    5, 6, 7,    // RUR, RLR, RLL
    /* Left */
    4, 7, 0,    // RUL, RLL, UL
    0, 7, 3,    // UL, RLL, LL
    /* Top */ 
    4, 0, 5,    // RUL, UL, RUR
    5, 0, 1,    // RUR, UL, UR
    /* Bottom */
    2, 3, 7,    // LR, LL, RLL
    2, 7, 6     // LR, RLR, RLL
  };
  GLuint vao, buffers[2];
  
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glGenBuffers(2, buffers);
  glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof (vertices), vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof (indeces), indeces, GL_STATIC_DRAW);
  glEnableVertexAttribArray(pos);
  glVertexAttribPointer(pos, 3, GL_FLOAT, 0, 0, 0);

  return vao;
}

#endif
