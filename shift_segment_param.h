
/*************************************************************
 * Derek Schacht
 * 2014/04/23
 *
 * *** Use Statement and License ***
 * Free for non commercial use! Anything else is not authorized without consent.
 *  Contact [dschacht ( - at - ) gmail ( - dot - ) com] for use questions.
 *************************************************************
 */

#ifndef SHIFT_SEGMENT_PARAM_H
#define SHIFT_SEGMENT_PARAM_H

/* These define statements allow the code to be easily changed if the wiring from
 * the shift registers to the displays were to ever change. This supports a maximum
 * of 8 digits, two displays.
 */

/* 15 14 13 12 | 11 10 9 8 | 7 6 5 4 | 3 2 1 0 */

#define SEGMENT_A  0x0004
#define SEGMENT_B  0x0020
#define SEGMENT_C  0x0001
#define SEGMENT_D  0x2000
#define SEGMENT_E  0x1000
#define SEGMENT_F  0x0100
#define SEGMENT_G  0x0010
#define SEGMENT_DP 0x0400

#define DIGITS 8

#define DIGIT_1 0x0080 // Left most bottom display (display 2)
#define DIGIT_2 0x0040
#define DIGIT_3 0x0008
#define DIGIT_4 0x0002
#define DIGIT_5 0x8000 // Left most top display (display 1)
#define DIGIT_6 0x4000
#define DIGIT_7 0x0800
#define DIGIT_8 0x0200


#endif
