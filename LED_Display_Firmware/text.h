#include <stdint.h>

#ifndef TEXT_H
#define TEXT_H

#define FONT_6x13
/* This will include the 6x13 font. */ 
#define DEFAULT_FONT font_6x13
#define DEFAULT_CHAR_LEN 6

#define FONT_4x6
#define SMALL_FONT font_4x6
#define SMALL_CHAR_LEN 4  /* The width of the font */ 
#define FONT_START_CHAR (' ')
#define FONT_END_CHAR ('~')
void write_string_big(uint16_t *board, char inStr[]);
void write_string_small_top(uint16_t *board, char inStr[]);
void write_string_small_bottom(uint16_t *board, char inStr[]);
int write_string_with_font_16(uint16_t *board, char inStr[], int startColumn, uint16_t *fontData, int dontUseFontWidths,int *fontWidths, signed int shiftAmount);
int write_string_with_font_8(uint16_t *board, char inStr[], int startColumn, uint8_t *fontData, int dontUseFontWidths,int *fontWidths, signed int shiftAmount);

#endif
