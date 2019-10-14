/** Copyright (C) 2019 Samuel Venable
***
*** This file is a part of the ENIGMA Development Environment.
***
*** ENIGMA is free software: you can redistribute it and/or modify it under the
*** terms of the GNU General Public License as published by the Free Software
*** Foundation, version 3 of the license or any later version.
***
*** This application and its source code is distributed AS-IS, WITHOUT ANY
*** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*** details.
***
*** You should have received a copy of the GNU General Public License along
*** with this code. If not, see <http://www.gnu.org/licenses/>
**/

#include "Window.h"
#include "Platforms/SDL/Window.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <X11/Xlib.h>

static inline string remove_trailing_zeros(int numb) {
  string strnumb = std::to_string(numb);

  while (!strnumb.empty() && strnumb.find('.') != string::npos && (strnumb.back() == '.' || strnumb.back() == '0'))
    strnumb.pop_back();

  return strnumb;
}

namespace enigma {

// called from initGameWindow()
// capture sdl window disp/hwnd
void window_id_init() {
  SDL_SysWMinfo wmInfo;
  SDL_VERSION(&wmInfo.version);
  SDL_GetWindowWMInfo(enigma::windowHandle, &wmInfo);
  enigma::x11::disp = wmInfo.info.x11.display;
  enigma::x11::win = wmInfo.info.x11.window;
}

}

namespace enigma_user {

// returns sdl window pointer for extensions
// we cast to/from a void * for generic-ness
void *window_handle() {
  return (void *)enigma::x11::win;
}

// returns an identifier for the sdl window
// this string can be used in shell scripts
string window_identifier() {
  return remove_trailing_zeros((int)enigma::x11::win);
}

// returns an identifier for certain window
// this string can be used in shell scripts
string window_get_identifier(void *hwnd) {
  return remove_trailing_zeros((int)(Window)hwnd);
}

}