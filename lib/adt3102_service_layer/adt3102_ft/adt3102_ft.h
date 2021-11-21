#ifndef __ADT3102_FT_H
#define __ADT3102_FT_H

#include "adt3102_type_define.h"

void ftTest(void);
void fttestVdd12(void);
void fttestPlli(void);
void fttest50Moscillator(void);
void fttestPlln(void);
void fttestBaseband(void);
void fttestLPvdd12(int slpLdo12);
void fttestRc32K(void);

void fttestRfBias(void);
void fttestRfBiasVbe(void);
void fttestRfBiasVbg(void);
void fttestRfbias1Din24(void);

void fttestPlliPllnBias(void);
void fttestPllilocktotalcurrent(void);
void pllibiascurrentdac12(void);
void pllibiascurrentitest(void);

void fttestPlliPllnBiasVcoBuff(void);
void fttestPllndvdd33(void);
void fttestPllnpllnblocks(void);
void fttestPllnvcobuff(void);
void fttestPllnvcoldo(void);
void fttestPllnpllldo(void);

void fttestPllndac12(void);
void fttestPllnvcoitest(void);

void fttestRfcurrent(void);
void fttestRfcurrentbasic(void);
void fttestRfcurrent20G(void);
void fttestRfcurrentDoubler(void);
void fttestRfcurrent1to4(void);
void fttestRfcurrent2PA(void);
void fttestRfcurrent2LNA(void);
void fttestRfcurrent2TIA(void);
void fttestRfcurrent2PGA(void);
void fttestRfcurrent2ADC(void);

void fttestBBbiasCurrent(void);

void frequecyCounter(int refclkSel, uint32 saveAddr);
void fttestPlliLock(void);
void fttestPllnLock(void);
void fttestPllnLock76G(void);
void fttestPllnLock77G(void);
void fttestPllnLock78G(void);
void fttestPllnLock79G(void);
void fttestPllnLock80G(void);
void fttestPllnLock81G(void);

void fttestChannelPd(void);
void fttestChannelPd20G(void);
void fttestChannelPddoubler(void);
void fttestChannelPd20GRx(void);
void fttestChannelPd20GTx(void);
void fttestChannelPd20GPA1(void);
void fttestChannelPd20GPA0(void);

void fttestgpadcIN1(void);
void fttestgpadcIN1gpadc(void);
void fttestgpadcATBdcTestPA(void);
void fttestgpadcATBdcTest20G(void);
void fttestgpadcATBdcTestDoubler(void);
void fttestgpadcATBdcTestPGA(void);
void fttestgpadcATBdcTestADC(void);
void fttestgpadcATBdcTestBist(void);

void fttestVgaBist(void);
void fttestDut(void);
void fttestMaster(void);
void fttestAdc(void);
void fttestGpioOutputH(void);
void fttestGpioOutputL(void);
void fttestGpioInput(void);
void mbistMaster(void);


#endif

