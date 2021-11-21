// copyright @ 2021 -
// by Dr. Kang Liu
// China Jiliang University
// kang.liu@cjlu.edu.cn

#include "math.h"
#include "arm_math.h"
#include "stdio.h"
#include "string.h"
#include "peak_single_mimo.h"



void background_removal(int32 srcAddr, int32 bgAddr, uint16 nFftRange,uint16 rangeMax, uint16 nFftVel,uint16 nLog2FftVel, float iqcoef, uint8 update_flag)
{
	volatile int32* pData;
    volatile int32* pDCdata;
	int32 iqRead, iqBg;
	int16 iRead,qRead,iBg,qBg;
    int16 iCal, qCal;
    
    // estimate background for several minutes
    // if doppler is zero, the frame is used for background estimation!
    if (update_flag == 1)
    {
        for(unsigned int count1 =0; count1< rangeMax; count1++)
        {
            //dcSumI = 0;
            //dcSumQ = 0;
            pData = (volatile int32*)(srcAddr + (count1<<2));
            pDCdata = (volatile int32*)(bgAddr + (count1<<2));

            // read backgrond removal I,Q int data
            iqBg = *pDCdata;
            iBg  = (*((int16 *)pDCdata )) ; //iqBg & (0xffff);
            qBg  = (*((int16 *)pDCdata+1 )) ; //iqBg >> 16; 
            
            for(uint16 count2 = 0; count2 < nFftVel; count2++)
            {
                // read src I, Q int data
                iqRead = *pData;
                iRead  = iqRead & (0xffff);
                qRead  = iqRead >> 16; 
                
                //dcSumI = dcSumI + iRead;
                //dcSumQ = dcSumQ + qRead;
                
                    
                //dcI = (int16)(dcSumI>>nLog2FftVel);
                //dcQ = (int16)(dcSumQ>>nLog2FftVel);   
                    
                    
                // weighting the background
                iBg   = (int16)((iBg * (1-iqcoef)) + (iRead * iqcoef));
                qBg   = (int16)((qBg * (1-iqcoef)) + (qRead * iqcoef));
                *pDCdata = (qBg<<16) + iBg;
                
                // update 
                pData = pData + nFftRange;
            }                
            
        }	
    }
    
	//background removal
    
    for(unsigned int count1 =0; count1< rangeMax; count1++)
    {
		pData = (volatile int32*)(srcAddr + (count1<<2));
        pDCdata = (volatile int32*)(bgAddr + (count1<<2));
        // read backgrond removal I,Q int data
        iqBg = *pDCdata;
		iBg  = iqBg & (0xffff);
		qBg  = iqBg >> 16; 
        for(uint16 count2 = 0; count2 < nFftVel; count2++)
        {
            // read src I, Q int data
			iqRead = *pData;
			iRead  = iqRead & (0xffff);
			qRead  = iqRead >> 16; 
            
            // remove background
			iCal   = iRead - iBg;
			qCal   = qRead - qBg;
			*pData = (qCal<<16) + iCal;
		    pData = pData + nFftRange;
        }
    }
}
