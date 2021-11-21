#ifndef __HANGZHOU_TEST_H
#define __HANGZHOU_TEST_H
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

void hangzhouTest(void);

void rc32kOutput(void);
void hclkOutput(void);
void testPower(void);
void adcClkOutput(void);
void manuPllnCapArray(int32 cpValue);

void test20GSingleFequence(void);
void test20GFmcw(void);
void test20GFmcwSingle(void);
void testChannelAtb(void);
void plliOutput(void);

void bistOn(uint16 bistSorSel, uint16 bistDivSel);
void testBist(void);
void testMbist(void);

void testSiggenDsp(void);
void testBistDsp(void);
void testIfInputDsp(void);
void testIfInputDspFreeRun(void);
void testAdc16mHclk125M(void);
void testTemprature(void);

void test80GSingleFequence(void);
void testSingleFequenceUcw(void);
void test80GSingleFequenceMaster(void);
void test80GSingleFequenceDut(void);
void test80GSingleFequenceRx(void);
void test80GFmcw(void);
void testAdcSample(void);
void test20GInputBist(void);
void testAdcDC(void);

void testHighClock50or125M(void);
void testLowPowerConsumption(void);
void mcu_sleep_test_hangzhou(int32 ram_on);
void test20GSingleFequenceUart(void);

void testBistRF_TX1_RX0(void);
void testBistRF_TX0_RX1(void);
void testBistRXABB(void);
void testBistONOFFAMP_Full_Bk(void);
void testBistONOFFAMP(void);


#endif

