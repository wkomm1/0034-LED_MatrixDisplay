#include <stdint.h>
#include "gol.h"
/*The array of pixels is a 85 element long of uint_16s.
 * So how will this all work?
 * Ill tell you how this will freaking work.
 * So we are going to get the current column, the left and the right column. 
 * Using that, we are going to go through pixel by pixel, assembling a list of
 * all the pixels near the current one. 
 * We will then populate another array with the current column.
 * Or better yet, we will store the first and last column of the previous state
 * and just update it in place. */ 


/* Now for the rules of the game of life...
 * Any live cell with fewer than two live neighbours dies, as if caused by under-population.
 * Any live cell with two or three live neighbours lives on to the next generation.
 * Any live cell with more than three live neighbours dies, as if by over-population.
 * Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
 * 
 * NOTE! All things happen simultaniously. Which means that:
 * We need to store the first, last column.
 * As we go from left to right, we need to save a copy of the column that we are
 * dealing with as it appeared before.
 * Now. Lets write some code...*/ 


#define NUM_OF_COLS 85
#define LAST_COL NUM_OF_COLS - 1
void update_life(uint16_t *board){
	uint16_t firstCol, lastCol, curCol, nextCol;
	uint16_t virginPrevCol;  /* The unaltered previous column */ 
	int colIndex;
	firstCol = board[0];
	lastCol = board[LAST_COL];

	/* Take care of the first row */ 
	curCol = firstCol;
	nextCol = board[1];
	virginPrevCol = firstCol; /* Save this for later */ 
	board[0] = update_life_column(curCol, lastCol, nextCol);
	for (colIndex = 1;colIndex < NUM_OF_COLS;colIndex++){
		/* We need to save the current column as the  */ 
		if (colIndex == LAST_COL){
			curCol = lastCol;
			nextCol = firstCol;
		} else {
			curCol = board[colIndex];
			nextCol = board[colIndex+1];
		}
		board[colIndex] = update_life_column(curCol, virginPrevCol, nextCol);
		virginPrevCol = curCol;  /* Save the column for next go */ 
	}
}

#define BIT0 (1<<0)
#define BIT1 (1<<1)
#define BIT2 (1<<2)
#define BIT3 (1<<3)
#define BIT4 (1<<4)
#define BIT5 (1<<5)
#define BIT6 (1<<6)
#define BIT7 (1<<7)
#define BIT8 (1<<8)
#define BIT9 (1<<9)
#define BIT10 (1<<10)
#define BIT11 (1<<11)
#define BIT12 (1<<12)
#define BIT13 (1<<13)
#define BIT14 (1<<14)
#define BIT15 (1<<15)
uint16_t update_life_column(uint16_t curCol, uint16_t prevCol, uint16_t nextCol){
	/* This function will update the a new column based on the
	 * next and previous columns. */ 
	uint16_t outCol;
	uint16_t nextRowBit, prevRowBit, curRowBit;
	int rowIndex;
	int count;
	/* We will handle the first row and last row independently.  */ 
	outCol = 0;
	/* Now for the rest of the bits... */ 
	for (rowIndex=0;rowIndex<16;rowIndex++){
		if (rowIndex == 0){
			curRowBit = BIT0;
			prevRowBit = BIT15;
			nextRowBit = BIT1;
		} else if (rowIndex == 15){
			curRowBit = BIT15;
			prevRowBit = BIT14;
			nextRowBit = BIT0;
		} else {
			curRowBit = 1 << rowIndex;
			prevRowBit = 1 << (rowIndex-1);
			nextRowBit = 1 << (rowIndex+1);
		}
		/* Now get a count of how many neighbors are alive... */ 
		if (curCol & prevRowBit){
			count++;
		}
		if (curCol & nextRowBit){
			count++;
		}
		if (prevCol & curRowBit){
			count++;
		}
		if (prevCol & nextRowBit){
			count++;
		}
		if (prevCol & prevRowBit){
			count++;
		}
		if (nextCol & curRowBit){
			count++;
		}
		if (nextCol & prevRowBit){
			count++;
		}
		if (nextCol & nextRowBit){
			count++;
		}
		/* Now to check the rules which are...
		 * Any live cell with fewer than two live neighbours dies, as if caused by under-population.
		 * Any live cell with two or three live neighbours lives on to the next generation.
		 * Any live cell with more than three live neighbours dies, as if by over-population.
		 * Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
		 */
		if (curCol & curRowBit){
			/* Then we are alive */ 
			if (count == 2 | count == 3){
				nextCol |= curRowBit;
				/* Then we live, otherwise, we are dead. */ 
			}
		} else {
			if (count == 3){
				nextCol |= curRowBit;
			}
		}
	}
	return outCol;
}
