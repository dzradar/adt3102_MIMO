#ifndef __UTILITIES_H
#define __UTILITIES_H

#include "adt3102_type_define.h"

#define BIST_AMP 1
#define BIST_ABB 0
#define BIST_DIV2_SEL       0x01//00001
#define BIST_DIV4_SEL       0x04//00100
#define BIST_DIV8_SEL       0x02//00010
#define BIST_DIV16_SEL      0x09//01001
#define BIST_DIV32_SEL      0x06//00110
#define BIST_DIV64_SEL      0x0A//01010
#define BIST_DIV128_SEL     0x15//10101
#define BIST_DIV256_SEL     0x19//11001
#define BIST_DIV512_SEL     0x16//10110
#define BIST_DIV1024_SEL    0x1A//11010
void fmcwChirpSingleFrequency(void);
void bistOn(uint16 bistSorSel, uint16 bistDivSel);
#endif


