#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <GL/gl.h>

#include "buttons.h"
#include "frame.h"
#include "matrices.h"
#include "shaders.h"
#include "util.h"

#define FAIL_IF(c, ...) if((c)) { fprintf(stderr, __VA_ARGS__); exit(EXIT_FAILURE); }

int
main(int argc, char *argv[])
{
    const int width = 1280, height = 960;

    /* Initialize SDL. */
    FAIL_IF(SDL_Init(SDL_INIT_VIDEO) < 0,
	    "Could not initialize SDL: %s\n", SDL_GetError());
    atexit(SDL_Quit);

    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    /* Create a window. */
    SDL_Window *window;
    const Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
    const Uint32 cxy = SDL_WINDOWPOS_CENTERED;
    
    window = SDL_CreateWindow("terragen", cxy, cxy, width, height, flags);

    /* Initialize opengl. */
    SDL_GLContext context;
    
    context = SDL_GL_CreateContext(window);
    FAIL_IF(context == NULL,
	    "Could not create sdl gl context: %s\n", SDL_GetError());

    glewInit();
    FAIL_IF(!glewIsSupported("GL_VERSION_4_0"),
	    "gl version 4.0 not supported\n");

    /* Build shader program. */
    char *log;
    GLuint fragment, vertex, prg;

    fragment = shader(GL_FRAGMENT_SHADER, "fragment.glsl", &log);
    FAIL_IF(fragment == 0,
	    "Could not compile fragment shader at fragment.glsl: %s\n", log);
    vertex = shader(GL_VERTEX_SHADER, "vertex.glsl", &log);
    FAIL_IF(vertex == 0,
	    "Could not compile fragment shader at vertex.glsl: %s\n", log);
    prg = glCreateProgram();
    glAttachShader(prg, vertex);
    glAttachShader(prg, fragment);
    glLinkProgram(prg);

    prg = validated(prg, GL_LINK_STATUS, (gl_get_f)glGetProgramiv,
		    (gl_set_f)glGetProgramInfoLog, &log);
    FAIL_IF(prg == 0, "Failed to link shader program: %s\n", log);

    glUseProgram(prg);

    /* Set up vertex arrays. */
    ATTRIB(prg, pos);
    const GLfloat s = 0.5f;
    const GLfloat vertices[] = {
      /* Front face CW from top left */
      -s, s, -s,   // 0 UL
      s, s, -s,    // 1 UR
      s, -s, -s,   // 2 LR
      -s, -s, -s,  // 3 LL
      /* Rear face CW from top left */
      -s, s, s,    // 4 UL
      s, s, s,     // 5 UR
      s, -s, s,    // 6 LR
      -s, -s, s    // 7 LL
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

    /* Set up uniforms. */
    UNIFORM(prg, proj);
    UNIFORM(prg, view);
    UNIFORM(prg, color);
    UNIFORM(prg, model);
    GLfloat aspect = (GLfloat)width / (GLfloat) height;
    GLfloat fov = 60.f * (GLfloat)M_PI * 2.0f / 360.f;
    GLfloat near = 0.01f, far = 15.0f;
    GLfloat f = 1.0f / tanf(fov / 2.0f);
    const GLfloat proj_matrix[16] = {
      f / aspect, 0.0f, 0.0f, 0.0f,
      0.0f, f, 0.0f, 0.0f,
      0.0f, 0.0f, -(near + far) / (near - far), 1.0f,
      0.0f, 0.0f, (2.0f * near * far) / (near - far), 0.0f
    };
    glUniformMatrix4fv(proj, 1, GL_FALSE, proj_matrix);

    GLfloat view_matrix[16];
    vec3 eye = { -1.5f, -1.0f, 2.5f }, up = { 0.0f, 1.0f, 0.0f };
    vec3 focus = { 0.0f, 0.0f, 0.0f };
    camera(&eye, &up, &focus, view_matrix);

    glUniformMatrix4fv(view, 1, GL_FALSE, view_matrix);

    /* Draw the screen. */
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClearColor(0.15f, 0.2f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    const GLfloat face_colors[18] = {
      68.0/256.0, 145.0/256.0, 94.0/256.0,
      0.0f, 0.0f, 0.0f,
      240.0/256.0, 145.0/256.0, 94.0/256.0,
      68.0/256.0, 145.0/256.0, 94.0/256.0,
      240.0/256.0, 145.0/256.0, 230.0/256.0,
      1.0, 0.0, 1.0
    };

    /* Enter the main loop. */
    GLfloat model_matrix[16];
    bool buttons[BUTTON_MAX];
    const double fps = 24;
    const double frame_len = 1.0 / fps;
    float speed = 0.01f;
    float rspeed = 0.01f;
    struct timeval tick, next;
    double dt;
    struct frame frame;

    memset(buttons, 0, sizeof (buttons));
    frame_init(&frame, -0.75f, -1.0f, 2.5f);
    gettimeofday(&tick, NULL);

    for (;;) {
      /* Redraw the screen. */
      glUniformMatrix4fv(view, 1, GL_FALSE, view_matrix);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      for (int row = 0; row < 7; row ++) {
	int n_blocks = 7 - row;
	
	for (int block = 0; block < n_blocks; block ++) {
	  GLfloat w = s * 2.0f;
	  GLfloat row_x = - n_blocks * sqrt(2.0f) / 2.0f;
	  GLfloat yrot[16], trans[16];

	  matrix4_trans(trans, row_x + (block * sqrt(2.0f)), row * w, -row * sqrt(2.0f) / 2.0f);
	  matrix4_yrot(yrot, M_PI / 4.0f);
	  matrix4_mult(model_matrix, trans, yrot);
	  glUniformMatrix4fv(model, 1, GL_FALSE, model_matrix);
	  for (int i = 0; i < 6; i++) {
	    glUniform3fv(color, 1, &face_colors[i * 3]);
	    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,
			 (const GLvoid *) (i * 6 * sizeof(GLfloat)));
	  }
	}
      }
      
      glFinish();
      SDL_GL_SwapWindow(window);
      
      /* Handle events. */
      SDL_Event event;
      while (SDL_PollEvent(&event)) {
	switch (event.type) {
	case SDL_QUIT:
	  goto done;
	  
	case SDL_KEYDOWN:
	  buttons_press(buttons, event.key.keysym.sym);
	  break;
	  
	case SDL_KEYUP:
	  buttons_release(buttons, event.key.keysym.sym);
	  break;
	  
	default:
	  break;
	}
      }
      
      /* Respond to button state */
      if (button_is_pressed(buttons, BUTTON_STRAFE)) {
	if (button_is_pressed(buttons, BUTTON_FORWARD)) {
	  frame_elevate(&frame, speed);
	} else if (button_is_pressed(buttons, BUTTON_BACK)) {
	  frame_elevate(&frame, -speed);
	}
      } else {
	if (button_is_pressed(buttons, BUTTON_FORWARD)) {
	  frame_advance(&frame, speed);
	} else if (button_is_pressed(buttons, BUTTON_BACK)) {
	  frame_advance(&frame, -speed);
	}
      }

      if (button_is_pressed(buttons, BUTTON_STRAFE)) {
	if (button_is_pressed(buttons, BUTTON_LEFT)) {
	  frame_strafe(&frame, -speed);
	} else if (button_is_pressed(buttons, BUTTON_RIGHT)) {
	  frame_strafe(&frame, speed);
	}
      } else {
	if (button_is_pressed(buttons, BUTTON_LEFT)) {
	  frame.yaw -= rspeed;
	} else if (button_is_pressed(buttons, BUTTON_RIGHT)) {
	  frame.yaw += rspeed;
	}
      }

      /* Recalculate the camera. */
      const struct vec4 look = { .x = 0.0f, .y = 0.0f, .z = -1.0, .w = 1.0 };
      struct vec4 look_rot;
      GLfloat yrot[16];
      
      matrix4_yrot(yrot, frame.yaw);
      matrix4_mult_vec4(&look_rot, yrot, &look);
      vec3add(&focus, &frame.pos, (vec3 *)&look_rot);
      camera(&frame.pos, &frame.up, &focus, view_matrix);

      /* Keep time. */
      gettimeofday(&next, NULL);
      dt = timeval_s(timeval_sub(next, tick));

      if (dt < frame_len) {
	wait(frame_len - dt);
      }
      
      tick = next;
    }
    
    /* Clean up. */
 done:
    glDeleteProgram(prg);
    glDetachShader(prg, vertex);
    glDetachShader(prg, fragment);
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);

    exit(EXIT_SUCCESS);
}
