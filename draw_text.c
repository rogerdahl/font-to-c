#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "century_schoolbook_l_bold_30_font.h"

#define font_lookup century_schoolbook_l_bold_30_font_lookup
#define font_pixels century_schoolbook_l_bold_30_font_pixels

const int W = 800;
const int H = 150;
const int TRANSPARENT_COLOR = 0;

void drawText(uint8_t* buf, int pos_x, int pos_y, char* s) {
  char c = *s;
  while (c) {
    const struct font_char font_c = font_lookup[c];
    for (int y = 0; y < font_c.h; ++y) {
      for (int x = 0; x < font_c.w; ++x) {
        uint8_t v = font_pixels[font_c.offset + x + y * font_c.w];
        if (v != TRANSPARENT_COLOR) {
          buf[pos_x + x + font_c.left + (pos_y + y + font_c.top) * W] = v;
        }
      }
    }
    pos_x += font_c.advance;
    c = *(++s);
  }
}

int textWidth(char* s) {
  char c = *s;
  int w = 0;
  while (c) {
    const struct font_char font_c = font_lookup[c];
    w += font_c.advance;
    c = *(++s);
  }
  return w;
}

void drawCenteredText(uint8_t* buf, int pos_y, char* s) {
  int x = W / 2 - textWidth(s) / 2;
  drawText(buf, x, pos_y, s);
}

void drawRightAdjustedText(uint8_t* buf, int pos_y, char* s) {
  int x = W - textWidth(s);
  drawText(buf, x, pos_y, s);
}

void writePpm(char* filename, uint8_t* buf)
{
  FILE* fp = fopen(filename, "wb");
  fprintf(fp, "P6\n%d %d 255\n", W, H);
  for (int y = 0; y < H; ++y) {
    for (int x = 0; x < W; ++x) {
      uint8_t v = buf[x + (y * W)];
      fwrite(&v, 1, 1, fp);
      fwrite(&v, 1, 1, fp);
      fwrite(&v, 1, 1, fp);
    }
  }
}

int main(void) {
  uint8_t* buf = malloc(W * H * sizeof(uint8_t));
  drawText(buf, 0, 0 * TALLEST_CHAR_PIXELS, "The quick brown fox jumps over the lazy dog");
  drawCenteredText(buf, 1 * TALLEST_CHAR_PIXELS , "AWA\\\\\\///AAWWVVAAWWVWVWVW");
  drawRightAdjustedText(buf, 2 * TALLEST_CHAR_PIXELS, "Ava Awa Wka Via Vja");
  drawCenteredText(buf, 3 * TALLEST_CHAR_PIXELS, "1234567890 !@#$%^&*()-=_+~`';:?></.,");
  writePpm("draw_text_sample.ppm", buf);
  return 0;
}
