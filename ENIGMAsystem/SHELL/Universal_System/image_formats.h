/** Copyright (C) 2008 Josh Ventura
*** Copyright (C) 2013-2014 Robert B. Colton
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

#ifndef ENIGMA_IMAGEFORMATS_H
#define ENIGMA_IMAGEFORMATS_H

#include <string>
#include <vector>

/// NOTE: These image formats expect the data to be un-aligned and always reads and writes with BGRA full color

namespace enigma
{

struct RawImage {
  RawImage() {}
  ~RawImage() { delete[] pxdata; }
  RawImage(const RawImage&) = delete;
  RawImage(RawImage &&other): pxdata(other.pxdata), w(other.w), h(other.h) {
    other.pxdata = nullptr;
  }
  unsigned char* pxdata = nullptr;
  unsigned w = 0, h = 0;
};

struct Color {
  unsigned r, g, b, a;
};

/// Color formats
enum {
  color_fmt_rgba,
  color_fmt_rgb,
  color_fmt_bgra,
  color_fmt_bgr
};

Color image_get_pixel_color(unsigned char* pxdata, unsigned w, unsigned h, unsigned x, unsigned y);
void image_swap_color(unsigned char* pxdata, unsigned w, unsigned h, Color oldColor, Color newColor);
/// Note splits horizontally
std::vector<RawImage> image_split(unsigned char* pxdata, unsigned w, unsigned h, unsigned imgcount);
RawImage image_pad(unsigned char* pxdata, unsigned origWidth, unsigned origHeight, unsigned newWidth, unsigned newHeight); 
unsigned long *bgra_to_argb(unsigned char *bgra_data, unsigned pngwidth, unsigned pngheight, bool prepend_size = false);

/// Gets the image format, eg. ".bmp", ".png", etc.
std::string image_get_format(std::string filename);
/// Reverses the scan-lines from top to bottom or vice verse, this is not actually to be used, you should load and save the data correctly to avoid duplicating it
unsigned char* image_flip(const unsigned char* data, unsigned width, unsigned height, unsigned bytesperpixel);

/// Generic all-purpose image loading call.
unsigned char* image_load(std::string filename, std::string format, unsigned int* width, unsigned int* height, unsigned int* fullwidth, unsigned int* fullheight, int* imgnumb, bool flipped);
/// Generic all-purpose image loading call that will regexp the filename for the format and call the appropriate function.
unsigned char* image_load(std::string filename, unsigned int* width, unsigned int* height, unsigned int* fullwidth, unsigned int* fullheight, int* imgnumb, bool flipped);
/// Generic all-purpose image saving call.
int image_save(std::string filename, const unsigned char* data, std::string format, unsigned width, unsigned height, unsigned fullwidth, unsigned fullheight, bool flipped);
/// Generic all-purpose image saving call that will regexp the filename for the format and call the appropriate function.
int image_save(std::string filename, const unsigned char* data, unsigned width, unsigned height, unsigned fullwidth, unsigned fullheight, bool flipped);

unsigned char* image_load_bmp(std::string filename, unsigned int* width, unsigned int* height, unsigned int* fullwidth, unsigned int* fullheight, bool flipped);
unsigned char* image_decode_bmp(const std::string &image_data, unsigned int* width, unsigned int* height, unsigned int* fullwidth, unsigned int* fullheight, bool flipped);
unsigned char* image_load_gif(std::string filename, unsigned int* width, unsigned int* height, unsigned int* fullwidth, unsigned int* fullheight, int* imgnumb, bool flipped);
int image_save_bmp(std::string filename, const unsigned char* data, unsigned width, unsigned height, unsigned fullwidth, unsigned fullheight, bool flipped);

} //namespace enigma

#endif //ENIGMA_IMAGEFORMATS_H
