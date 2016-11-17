#include <stdint.h>
/* Stole the font code from https://raw.githubusercontent.com/evanrich/Bluetooth-LED-Sign/master/Controller%20Code/Arduino%201.0/font.h  */ 
#include "text.h"
#include "font.h"

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

/* write_string_with_font will write a null-terminated string starting at the 
 * input column with the input font.
 * If this function goes over the buffer, it will just return and say the next
 * open column is zero. 
 * inStr: The null-terminated input string
 * NOTE: This assumes all capitals!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * startColumn: Where the text should start
 * fontData: A pointer to where the text data is stored
 * useFontLengths: Set to zero to use the fontLengths variable for variable length font chars
 * 	Otherwise set to the width of the chars
 * fontLengths: A pointer to where the lengths of the letters are stored 
 * Returns the next-open column*/ 
int write_string_with_font(uint16_t *board, char inStr[], int startColumn, uint16_t *fontData, int dontUseFontLengths,int *fontLengths){
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
		if (dontUseFontLengths){
			/* I think I get the below font data correct.... 
			 * If you get junk to the screen, this is the first thing I would check.*/ 
			nextOpen = write_char(board, &(fontData[letterIndex]), dontUseFontLengths, nextOpen);
		} else {
			/* I think I get the below font data correct.... 
			 * If you get junk to the screen, this is the first thing I would check.*/ 
			nextOpen = write_char(board, &(fontData[letterIndex]), fontLengths[letterIndex], nextOpen);
		}

	} while(inStr[letterIndex] != 0);
	return nextOpen;
}
/* Just write the string to the board, dont need anything fancy.  */ 
void write_string(uint16_t *board, char inStr[]){
  // Bad cast, but C, so it will work. 
	write_string_with_font(board, inStr, 0, (uint16_t*)DEFAULT_FONT, CHAR_LEN, (uint16_t*)DEFAULT_FONT);
}
	


