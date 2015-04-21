
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


/* This class is intended to be used with two 74595 shift registers connected in series.
 * The 16 outputs from those shift registers should be connected to two four digit seven
 * segment displays. The shift register farthest from the arduino on the serial chain is
 * the HIGH byte and the closest is the LOW byte. Each of the digits on the displays is 
 * scanned onto the display creating the illusion of a static display. This must be done
 * fairly quickly about 40hz or faster to prevent the user from seeing flicker.
 */

/* Known to work with:
 *   https://www.sparkfun.com/products/733
 *   https://www.sparkfun.com/products/9483
 *   Don't forget resistors.
 */

/* There are a couple of ways to use this class. You may either modify the values in 
 * shift_segment_param.h to meet your hardware configuration, or define the values for
 * each connection and pass them into the setDigits and setSegments functions. ALWAYS
 * call setDigits first! This is due to the need to compute a mask value that is
 * applied to the segment values when populating the segment array. The definitions in
 * shift_segment_param are a great example. Each high bit represents the position that
 * a particular element is connected.
 */

/* If you wish to change how a number is drawn you will need to update the table in the
 * setSegments function. This class does support HEX values but the displayed characters
 * are pretty archaic.
 */

/* Dependencies:
 * The display scanning routine uses timer2 to perform an interrupt after an interval of
 * time. Timer2 will not be available for any other uses.
 * There are no assumed pins for this utility as they are all configured via the
 * constructor. The configuration should be done in the setup() function.
 */

/* I have tested this on arduinos that use 328p AVR. I do not have any others around so
 * testing on other variants has not been done by myself. It should be a pretty easy
 * port to change the timer code if Timer2 is needed for some other reason.
 */

class ShiftSegmentDisplay
{
  public:
    ShiftSegmentDisplay();
    ShiftSegmentDisplay(int dataClockPin, int dataPin, int latchPin);
	
    void setDigits(unsigned int digit_1, unsigned int digit_2, unsigned int digit_3, unsigned int digit_4,
                   unsigned int digit_5, unsigned int digit_6, unsigned int digit_7, unsigned int digit_8);
  
    void setSegments(unsigned int seg_A, unsigned int seg_B, unsigned int seg_C, unsigned int seg_D,
                     unsigned int seg_E, unsigned int seg_F, unsigned int seg_G, unsigned int seg_DP);
	
    void setDisplayValue(unsigned long value, int decimalPointPosition=0, int base=10);
    void setDisplayValue2(unsigned long value, int decimalPointPosition=0, int base=10);
    
    int getRefreshRate();

  private:
    void setupTimer();
    unsigned int decimalPoint;
    unsigned int segmentMask;
    unsigned int numberArray[16];
	

};

#endif
