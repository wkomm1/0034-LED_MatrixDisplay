#include <stdint.h>

#ifndef TEXT_H
#define TEXT_H

#define FONT_6x13
/* This will include the 6x13 font. */ 
#define DEFAULT_FONT font_6x13
#define CHAR_LEN 6
#define FONT_START_CHAR (' ')
#define FONT_END_CHAR ('~')
void write_string(uint16_t *board, char inStr[]);
int write_string_with_font(uint16_t *board, char inStr[], int startColumn, uint16_t *fontData, int dontUseFontLengths,int *fontLengths);

#endif
