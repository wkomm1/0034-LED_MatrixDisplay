#include <stdint.h>
#ifndef GOL_H
#define GOL_H


void update_life(uint16_t *board);
uint16_t update_life_column(uint16_t curCol, uint16_t prevCol, uint16_t nextCol);
#endif
