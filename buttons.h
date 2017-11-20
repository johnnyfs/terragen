#ifndef BUTTONS_H
#define BUTTONS_H

#include <SDL2/SDL.h>

enum button {
    BUTTON_FORWARD,
    BUTTON_BACK,
    BUTTON_RIGHT,
    BUTTON_LEFT,
    BUTTON_STRAFE,
    BUTTON_MAX
};

static const SDL_Keycode button_map[] = {
  SDLK_k,
  SDLK_j,
  SDLK_l,
  SDLK_h,
  SDLK_LSHIFT
};

static inline bool
button_is_pressed(bool buttons[BUTTON_MAX], enum button b)
{
  return buttons[b];
}

static inline void
buttons_press(bool buttons[BUTTON_MAX], SDL_Keycode sym)
{
  for (int i = 0; i < BUTTON_MAX; i ++) {
    if (button_map[i] == sym) {
      buttons[i] = true;
      return;
    }
  }
}

static inline void
buttons_release(bool buttons[BUTTON_MAX], SDL_Keycode sym)
{
  for (int i = 0; i < BUTTON_MAX; i ++) {
    if (button_map[i] == sym) {
      buttons[i] = false;
      return;
    }
  }
}

#endif //BUTTONS_H
