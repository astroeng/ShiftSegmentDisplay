#Quad Seven Segment Driver

## Description

This library can drive two quad seven segment common anode displays. It uses two 8 bit shift registers such as the 74595 to drive a pair of displays. The library is setup to allow the user fairly easy configuration for their specific application/configuration.

## Installation

Copy the project directory to the library folder in the Arduino working directory.

## Use Details

The library uses timer2 to control the refresh rate of the displays.

Each of the bits sent to the shift registers represents a connection to the displays. The library expects the user to define a symbol for each segment and digit on the display.

The following is an example of what needs to be defined in the user code. Each high bit represents the connection from the shift register to the display:

```#define DIGIT_1 0x0001```
```#define DIGIT_2 0x0002```
```#define DIGIT_3 0x0004```
```#define DIGIT_4 0x0008```
```#define DIGIT_5 0x0010```
```#define DIGIT_6 0x0020```
```#define DIGIT_7 0x0040```
```#define DIGIT_8 0x0080```

```#define SEGMENT_A  0x0100```
```#define SEGMENT_B  0x0200```
```#define SEGMENT_C  0x0400```
```#define SEGMENT_D  0x0800```
```#define SEGMENT_E  0x1000```
```#define SEGMENT_F  0x2000```
```#define SEGMENT_G  0x4000```
```#define SEGMENT_DP 0x8000```

The user may also define:

```#define DIGITS 8  /* Pick a number 8 or less */```
