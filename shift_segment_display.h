
/*************************************************************
 * Derek Schacht
 * 2014/04/23
 *
 * *** Use Statement and License ***
 * Free for non commercial use! Anything else is not authorized without consent.
 *  Contact [dschacht ( - at - ) gmail ( - dot - ) com] for use questions.
 *************************************************************
 */

#ifndef SHIFT_SEGMENT_DISPLAY_H
#define SHIFT_SEGMENT_DISPLAY_H


class ShiftSegmentDisplay
{
  public:
    ShiftSegmentDisplay();
    ShiftSegmentDisplay(int dataClockPin, int dataPin, int latchPin);
	
	void setDigits(unsigned int digit_1,unsigned int digit_2,unsigned int digit_3,unsigned int digit_4,
				   unsigned int digit_5,unsigned int digit_6,unsigned int digit_7,unsigned int digit_8);
  
	void setSegments(unsigned int seg_A,unsigned int seg_B,unsigned int seg_C,unsigned int seg_D,
				     unsigned int seg_E,unsigned int seg_F,unsigned int seg_G,unsigned int seg_DP);
	
    void setDisplayValue(unsigned long value, int decimalPointPosition);
    void setDisplayValue2(unsigned long value, int decimalPointPosition);
    
    int getRefreshRate();
	unsigned long time1;
	unsigned long time2;
  private:
	void setupTimer();
    unsigned int decimalPoint;
	unsigned int segmentMask;
    unsigned int numberArray[10];
	

};

#endif
