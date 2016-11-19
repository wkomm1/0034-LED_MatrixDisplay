#include <stdint.h>
/* Stole the font code from https://raw.githubusercontent.com/evanrich/Bluetooth-LED-Sign/master/Controller%20Code/Arduino%201.0/font.h  */ 
#include "text.h"
#include "fontRev.h"

/* We need to display some text...
 * Lets start with a function that will write a character to the board starting
 * at the specified column.
 * If this function goes over the buffer, it will just return and say the next
 * open column is zero. 
 * This function will take in:
 * a pointer to the board
 * a pointer the the fixed length array of uint16s that correspond to the letter
 * the length of the letter data
 * an int specifing which column to start in, zero indexed.
 * This function returns the next-open column.*/ 

int write_char(uint16_t *board, uint16_t *letter, int letterLength, int startColumn){
	int col;
	for (col=startColumn; col < (startColumn + letterLength + 1) ; col++){
		board[col] = letter[col-startColumn];
	}
	/* Has the possibility to be off by one. Don't think so, but don't 
	 * quote me.*/ 
	return startColumn+letterLength+1;
}

int write_char_with_shift_16(uint16_t *board, uint16_t *letter, int letterLength, int startColumn, signed int shiftAmount){
	int col;
	for (col=startColumn; col < (startColumn + letterLength + 1) ; col++){
		board[col] = (letter[col-startColumn] << shiftAmount);
	}
	/* Has the possibility to be off by one. Don't think so, but don't 
	 * quote me.*/ 
	return startColumn+letterLength+1;
}

int write_char_with_shift_8(uint16_t *board, uint8_t *letter, int letterLength, int startColumn, signed int shiftAmount){
	int col;
	for (col=startColumn; col < (startColumn + letterLength + 1) ; col++){
		board[col] = (letter[col-startColumn] << shiftAmount);
	}
	/* Has the possibility to be off by one. Don't think so, but don't 
	 * quote me.*/ 
	return startColumn+letterLength+1;
}

/* write_string_with_font will write a null-terminated string starting at the 
 * This will be the 8 bit function
 * input column with the input font.
 * If this function goes over the buffer, it will just return and say the next
 * open column is zero. 
 * inStr: The null-terminated input string
 * startColumn: Where the text should start
 * fontData: A pointer to where the text data is stored
 * useFontWidths: Set to zero to use the fontWidths variable for variable length font chars
 * 	Otherwise set to the width of the chars
 * fontWidths: A pointer to where the Widths of the letters are stored 
 * shiftAmount: How many bits to shift up
 * Returns the next-open column*/ 
int write_string_with_font_8(uint16_t *board, char inStr[], int startColumn, uint8_t *fontData, int dontUseFontWidths,int *fontWidths, int shiftAmount){
	int index;
	int letterIndex;  /* The de-asciified letter index, so letter - 'A' */ 
	int nextOpen;
	nextOpen = startColumn;
	index = 0;
	do {
		if (inStr[index] > FONT_END_CHAR || inStr[index] < FONT_START_CHAR){
			break;
			/* Make sure we didnt get bad data. */ 
		}
		letterIndex = inStr[index] - FONT_END_CHAR;
		if (dontUseFontWidths){
			/* I think I get the below font data correct.... 
			 * If you get junk to the screen, this is the first thing I would check.*/ 
			nextOpen = write_char_with_shift_8(board, &(fontData[letterIndex]), dontUseFontWidths, nextOpen, shiftAmount);
		} else {
			/* I think I get the below font data correct.... 
			 * If you get junk to the screen, this is the first thing I would check.*/ 
			nextOpen = write_char_with_shift_8(board, &(fontData[letterIndex]), fontWidths[letterIndex], nextOpen, shiftAmount);
		}

	} while(inStr[letterIndex] != 0);
	return nextOpen;
}
/* write_string_with_font will write a null-terminated string starting at the 
 * This will be the 16 bit function
 * input column with the input font.
 * If this function goes over the buffer, it will just return and say the next
 * open column is zero. 
 * inStr: The null-terminated input string
 * startColumn: Where the text should start
 * fontData: A pointer to where the text data is stored
 * useFontWidths: Set to zero to use the fontWidths variable for variable length font chars
 * 	Otherwise set to the width of the chars
 * fontWidths: A pointer to where the Widths of the letters are stored 
 * shiftAmount: How many bits to shift up
 * Returns the next-open column*/ 
int write_string_with_font_16(uint16_t *board, char inStr[], int startColumn, uint16_t *fontData, int dontUseFontWidths,int *fontWidths, int shiftAmount){
	int index;
	int letterIndex;  /* The de-asciified letter index, so letter - 'A' */ 
	int nextOpen;
	nextOpen = startColumn;
	index = 0;
	do {
		if (inStr[index] > FONT_END_CHAR || inStr[index] < FONT_START_CHAR){
			break;
			/* Make sure we didnt get bad data. */ 
		}
		letterIndex = inStr[index] - FONT_END_CHAR;
		if (dontUseFontWidths){
			/* I think I get the below font data correct.... 
			 * If you get junk to the screen, this is the first thing I would check.*/ 
			nextOpen = write_char_with_shift_16(board, &(fontData[letterIndex]), dontUseFontWidths, nextOpen, shiftAmount);
		} else {
			/* I think I get the below font data correct.... 
			 * If you get junk to the screen, this is the first thing I would check.*/ 
			nextOpen = write_char_with_shift_16(board, &(fontData[letterIndex]), fontWidths[letterIndex], nextOpen, shiftAmount);
		}

	} while(inStr[letterIndex] != 0);
	return nextOpen;
}
/* Just write the string to the board, dont need anything fancy.  */ 
void write_string_big(uint16_t *board, char inStr[]){
  // Bad cast, but C, so it will work. 
	write_string_with_font_16(board, inStr, 0, (uint16_t*)DEFAULT_FONT, DEFAULT_CHAR_LEN, (int*)DEFAULT_FONT, 0);
}
	

/* Just write the string to the top row */
void write_string_small_top(uint16_t *board, char inStr[]){
  // Bad cast, but C, so it will work. 
	write_string_with_font_8(board, inStr, 0, (uint8_t*)SMALL_FONT, SMALL_CHAR_LEN, (int*)SMALL_FONT, 8);
}
	
/* Just write the string to the bottom row */
void write_string_small_bottom(uint16_t *board, char inStr[]){
  // Bad cast, but C, so it will work. 
	write_string_with_font_8(board, inStr, 0, (uint8_t*)SMALL_FONT, SMALL_CHAR_LEN, (int*)SMALL_FONT, 0);
}
	


