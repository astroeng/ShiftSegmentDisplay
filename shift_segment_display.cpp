/*************************************************************
 * Derek Schacht
 * 2014/04/23
 *
 * *** Use Statement and License ***
 * Free for non commercial use! Anything else is not authorized without consent.
 *  Contact [dschacht ( - at - ) gmail ( - dot - ) com] for use questions.
 *************************************************************
 */

#include <Arduino.h>
#include "shift_segment_param.h"
#include "shift_segment_display.h"

/* With a timer clock multiplier of 1/1024 and a timeout of 160 this ISR will fire
 * about every 6144 microseconds. Which is a refresh rate of 40.69 hz at the display.
 * 130 gives about 31.00 hz Which is just a bit faster than the human eye's perception.
 * After looking at it for a minute, I'm convinced that I see flicker. Moving to 145
 * seems to have solved it and keeps the ISR from running excessivly.
 * 145 gives about 35.2 hz
 */
#define CLOCKSPEED 16000000
#define DIVIDER 1024

#ifndef TIMERCOUNTLIMIT
#define TIMERCOUNTLIMIT 240
#endif

#define REFRESHRATE CLOCKSPEED / DIVIDER / DIGITS / (256 - TIMERCOUNTLIMIT)

/* These variables are used by the ISR and need to be static globals for
 * that reason. Unless the ISR can be made to run within the scope of the
 * class. I'm not sure what the benifit would be since the class would 
 * still be restricted to a single instance given that the ISR is tied to
 * a hardware component on the AVR.
 */
static volatile unsigned int value;
static volatile unsigned char digit = 0;
static volatile unsigned int digitArray[DIGITS];
static volatile unsigned int displayValue[DIGITS];


/* Since the ISR needs the knowledge of these pins they need to be static
 * globals not held as part of the class. That is unless the ISR can be
 * made to run within the scope of the class.
 */
static int dataPinG       = 0;
static int dataClockPinG  = 0;
static int latchClockPinG = 0;

/* This ISR refreshes a sigle digit on the 7 segment display each time it
 * runs. The ISR needs to run eight times to update both displays.
 */
ISR(TIMER2_OVF_vect)
{
  /* Reset the timer value. */
  TCNT2 = TIMERCOUNTLIMIT;
  
  /* Make sure that this pin is low when this ISR starts. */
  
  digitalWrite(latchClockPinG, LOW);
  
  /* Compute the value needed for shifting. */
  value = digitArray[digit] | (~(displayValue[digit]));
  
  /* Shift out the first byte which is the number to display. Then shift
   * out the byte that controls which digit is illuminated. 
   */
  shiftOut(dataPinG, dataClockPinG, MSBFIRST, highByte(value));
  shiftOut(dataPinG, dataClockPinG, MSBFIRST, lowByte(value));

  /* Pulse the clock for the parallel load of the actual output latches. */
  digitalWrite(latchClockPinG, HIGH);
  digitalWrite(latchClockPinG, LOW);
  
  /* Move to the next digit, wrap at the max number of digits. */
  digit++;
  digit = digit % DIGITS;

};

/* This frinction is used to setup timer 2 which is the source of the
 * interrupt that is refreshing the 7 segment display.
 */
void ShiftSegmentDisplay::setupTimer()
{
  /* First disable the timer overflow interrupt while we're configuring */  
  TIMSK2 &= ~(1<<TOIE2);  
  /* Configure timer2 in normal mode (pure counting, no PWM etc.) */  
  TCCR2A &= ~((1<<WGM21) | (1<<WGM20));  
  TCCR2B &= ~(1<<WGM22);  
  /* Select clock source: internal I/O clock */  
  ASSR &= ~(1<<AS2);  
  /* Disable Compare Match A interrupt enable (only want overflow) */  
  TIMSK2 &= ~(1<<OCIE2A);

  /* Now configure the prescaler to CPU clock divided by 1024 */
  /* Page 158 in the chip manual. */
  TCCR2B |= ((1<<CS22) | (1<<CS21) | (1<<CS20)); // Set bits
  TCCR2B &= ~(0);                                // Clear bit

  /* Finally load and enable the timer */  
  TCNT2 = TIMERCOUNTLIMIT;  
  TIMSK2 |= (1<<TOIE2);
}

/* This function turns a number in int format into an array that can
 * be used to drive the 7 segment display.
 */
void ShiftSegmentDisplay::setDisplayValue(unsigned long value, int decimalPointPosition)
{
  /* When updating the array that contains the display value interrupts are disabled.
   * This helps prevent display glitches.
   */
  
  /* Critical Section Start */
  time1 = micros();
  noInterrupts();
  displayValue[3] = numberArray[value % 10];
  value = value / 10;
  displayValue[2] = numberArray[value % 10];
  value = value / 10;
  displayValue[1] = numberArray[value % 10];
  value = value / 10;
  displayValue[0] = numberArray[value % 10];
  displayValue[3-decimalPointPosition] |= decimalPoint;
  interrupts();
  time1 = micros() - time1;
  /* Critical Section End */
}

/* This function turns a number in int format into an array that can
 * be used to drive the 7 segment display.
 */
void ShiftSegmentDisplay::setDisplayValue2(unsigned long value, int decimalPointPosition)
{
  /* When updating the array that contains the display value interrupts are disabled.
   * This helps prevent display glitches.
   */
  
  /* Critical Section Start */
  time2 = micros();
  noInterrupts();
  displayValue[7] = numberArray[value % 10];
  value = value / 10;
  displayValue[6] = numberArray[value % 10];
  value = value / 10;
  displayValue[5] = numberArray[value % 10];
  value = value / 10;
  displayValue[4] = numberArray[value % 10];
  displayValue[7-decimalPointPosition] |= decimalPoint;
  interrupts();
  time2 = micros() - time2;
  /* Critical Section End */
}

void ShiftSegmentDisplay::setDigits(unsigned int digit_1, unsigned int digit_2,
									unsigned int digit_3, unsigned int digit_4,
									unsigned int digit_5, unsigned int digit_6,
									unsigned int digit_7, unsigned int digit_8)
{
	segmentMask = digit_1 | digit_2 | digit_3 | digit_4 | digit_5 | digit_6 | digit_7 | digit_8;
	
	Serial.println(segmentMask,HEX);
	
	digitArray[7] = digit_8;
	digitArray[6] = digit_7;
	digitArray[5] = digit_6;
	digitArray[4] = digit_5;
	digitArray[3] = digit_4;
	digitArray[2] = digit_3;
	digitArray[1] = digit_2;
	digitArray[0] = digit_1;
	
}

void ShiftSegmentDisplay::setSegments(unsigned int seg_A, unsigned int seg_B,
									  unsigned int seg_C, unsigned int seg_D,
									  unsigned int seg_E, unsigned int seg_F,
									  unsigned int seg_G, unsigned int seg_DP)
{

	decimalPoint = seg_DP | segmentMask;
	
	numberArray[0] = seg_A | seg_B | seg_C | seg_D | seg_E | seg_F |         segmentMask;
	numberArray[1] =         seg_B | seg_C |                                 segmentMask;
	numberArray[2] = seg_A | seg_B |         seg_D | seg_E |         seg_G | segmentMask;
	numberArray[3] = seg_A | seg_B | seg_C | seg_D |                 seg_G | segmentMask;
	numberArray[4] =         seg_B | seg_C |                 seg_F | seg_G | segmentMask;
	numberArray[5] = seg_A |         seg_C | seg_D |         seg_F | seg_G | segmentMask;
	numberArray[6] = seg_A |         seg_C | seg_D | seg_E | seg_F | seg_G | segmentMask;
	numberArray[7] = seg_A | seg_B | seg_C |                                 segmentMask;
	numberArray[8] = seg_A | seg_B | seg_C | seg_D | seg_E | seg_F | seg_G | segmentMask;
	numberArray[9] = seg_A | seg_B | seg_C | seg_D |         seg_F | seg_G | segmentMask;

}

ShiftSegmentDisplay::ShiftSegmentDisplay()
{
  /* Dummy Constructor */
}

ShiftSegmentDisplay::ShiftSegmentDisplay(int dataClockPin, int dataPin, int latchPin)
{
  /* Save the pins so that the ISR has visibility to them. */
  dataClockPinG  = dataClockPin;
  dataPinG       = dataPin;
  latchClockPinG = latchPin;

  pinMode(dataClockPinG , OUTPUT);
  pinMode(latchClockPinG, OUTPUT);
  pinMode(dataPinG      , OUTPUT);  
  
  decimalPoint = SEGMENT_DP | SEGMENT_MASK;

  /* These values are active low so they are inverted when or'ed and written out to the shift registers */
  
  numberArray[0] = SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_E | SEGMENT_F |             SEGMENT_MASK;
  numberArray[1] =             SEGMENT_B | SEGMENT_C |                                                 SEGMENT_MASK;
  numberArray[2] = SEGMENT_A | SEGMENT_B |             SEGMENT_D | SEGMENT_E |             SEGMENT_G | SEGMENT_MASK;
  numberArray[3] = SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D |                         SEGMENT_G | SEGMENT_MASK;
  numberArray[4] =             SEGMENT_B | SEGMENT_C |                         SEGMENT_F | SEGMENT_G | SEGMENT_MASK;
  numberArray[5] = SEGMENT_A |             SEGMENT_C | SEGMENT_D |             SEGMENT_F | SEGMENT_G | SEGMENT_MASK;
  numberArray[6] = SEGMENT_A |             SEGMENT_C | SEGMENT_D | SEGMENT_E | SEGMENT_F | SEGMENT_G | SEGMENT_MASK;
  numberArray[7] = SEGMENT_A | SEGMENT_B | SEGMENT_C |                                                 SEGMENT_MASK;
  numberArray[8] = SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_E | SEGMENT_F | SEGMENT_G | SEGMENT_MASK;
  numberArray[9] = SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D |             SEGMENT_F | SEGMENT_G | SEGMENT_MASK;

  /* These values are active high. These are or'ed with the numberArray. */
  
  digitArray[7] = DIGIT_8;
  digitArray[6] = DIGIT_7;
  digitArray[5] = DIGIT_6;
  digitArray[4] = DIGIT_5;
  digitArray[3] = DIGIT_4;
  digitArray[2] = DIGIT_3;
  digitArray[1] = DIGIT_2;
  digitArray[0] = DIGIT_1;

  displayValue[0] = 0;
  displayValue[1] = 0;
  displayValue[2] = 0;
  displayValue[3] = 0;
  displayValue[4] = 0;
  displayValue[5] = 0;
  displayValue[6] = 0;
  displayValue[7] = 0;
  
  setupTimer();
}


int ShiftSegmentDisplay::getRefreshRate()
{
  return REFRESHRATE;
}

