#include "gol.h"
#include "text.h"
// ==================== ARDUINO BITMASK INFO ====================
// dataA = 2           PD2   pin 1 on CN0/CN1  active HIGH
// dataB = 3           PD3   pin 2 on CN0/CN1  active HIGH
// dataC = 4           PD4   pin 3 on CN0/CN1  active HIGH
// G2AU38_ENABLE = 5   PD5   pin 4 on CN0/CN1  active LOW top
// G2AU37_ENABLE = 6   PD6   pin 5 on CN0/CN1  active LOW botom
// G1_ENABLE = 7       PD7   pin 6 on CN0/CN1  active HIGH

// G2B_ENABLE = 8      PB0   pin 7 on CN0/CN1  active LOW
// serialClock = 9     PB1   pin 8 on CN0/CN1 B1 for 9  rising edge
// serialData = 10     PB2   pin 9 on CN0/CN1 B2 for 10  active HIGH



uint32_t frameTimer;  //timer for frame
const uint16_t pixleLength = 85;  //how many pixles in serial string
uint16_t displayBuffer[pixleLength] = {0}; //buffer to store the active frame



void setup() {
  Serial.begin(115200);
  // DDRB and DDRD 1 is output 0 is input
  DDRD = B11111110; PORTD = B11100010;
  DDRB = B00000111; PORTB = B00000000;
  clearDisplay();  //reset the display
  clearBuffer();  //clear the buffer
}



void loop() {
  loadRitMdrc();
  simulateDisplay(2000);
}



void updateDisplay(uint32_t holdTime) {  //holds the display on for increments of 10ms
  frameTimer = millis();
  while (millis() - frameTimer < holdTime) {
    for (uint8_t Y = 0; Y < 16; Y++) {
      PORTD = PORTD | B01100000;  //G2AU38, G2AU37 ENABLE HIGH
      for (uint16_t X = 0; X < pixleLength; X++) {
        if (displayBuffer[X] >> Y & 1) PORTB = PORTB | B00000100;
        else PORTB = PORTB & B11111011;
        PORTB = PORTB | B00000010;  //pin 8 on CN0/CN1 B1 for 9
        PORTB = PORTB & B11111101;  //pin 8 on CN0/CN1 B1 for 9
      }
      PORTD = PORTD & B11100011;
      PORTD = PORTD | (Y & 7) << 2;  //set the data bits
      if (Y < 8) PORTD = PORTD & B11011111;  //G2AU38 ENABLE LOW
      else PORTD = PORTD & B10111111;  //G2AU37 ENABLE LOW
      delayMicroseconds(507);  //507 for 100Hz rate 920 for 60Hz
    }
  }
  PORTD = PORTD | B01100000;  //disables the display so last line isnt on
}



void clearDisplay() {
  PORTD = B11100010;
  PORTB = B00000000;
  for (uint16_t X = 0; X < pixleLength; X++) {
    PORTB = PORTB | B00000010;  //clock high
    PORTB = PORTB & B11111101;  //clock low
  }
}



void clearBuffer() {
  for (uint16_t i = 0; i < pixleLength; i++) {
    displayBuffer[i] = 0x0000;  //invert colors
  }
}



void invertBuffer() {
  for (uint16_t i = 0; i < pixleLength; i++) {
    displayBuffer[i] = ~displayBuffer[i];  //invert colors
  }
}



void loadRitMdrc() {
  clearBuffer();
  uint16_t RIT_MDRC[48] = {
    0x0088, 0x0104, 0x0104, 0x0104, 0x00f8, 0x0000,  // C
    0x00c4, 0x0128, 0x0130, 0x0120, 0x01fc, 0x0000,  // R
    0x00f8, 0x0104, 0x0104, 0x01fc, 0x0104, 0x0000,  // D
    0x01fc, 0x0080, 0x0040, 0x0080, 0x01fc, 0x0000,  // M
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,  //
    0x0100, 0x0100, 0x01fc, 0x0100, 0x0100, 0x0000,  // T
    0x0000, 0x0104, 0x01fc, 0x0104, 0x0000, 0x0000,  // I
    0x00c4, 0x0128, 0x0130, 0x0120, 0x01fc, 0x0000   // R
  };

  for (uint8_t i = 0; i < 48; i++) displayBuffer[i + 19] =  RIT_MDRC[i] << 3;
  flipBufferY(); flipBufferX();
}



void flipBufferY() {
  uint16_t workingVariable;
  uint16_t tempCounter;
  for (uint16_t i = 0; i < pixleLength; i++) {
    workingVariable = 0; tempCounter = 0;
    for (int8_t j = 15; j >= 0; j--) {
      workingVariable = workingVariable | ((displayBuffer[i] >> j & 1) << tempCounter);
      tempCounter++;
    }
    displayBuffer[i] =  workingVariable;
  }
}



void flipBufferX() {
  uint16_t workingVariable;
  for (uint16_t i = 0; i < pixleLength / 2.0 + 0.5; i++) {
    workingVariable = displayBuffer[i];
    displayBuffer[i] = displayBuffer[pixleLength - 1 - i];
    displayBuffer[pixleLength - 1 - i] =  workingVariable;
  }
}



void simulateDisplay(uint32_t holdTime) {
  frameTimer = millis();
  for (uint16_t index = 0; index < pixleLength + 2; index++) Serial.print("=");
  Serial.println();
  for (uint16_t i = 0; i < 16; i++) {
    Serial.print("|");
    for (uint16_t index = 0; index < pixleLength; index++) {
      if (displayBuffer[index] & (1 << i)) Serial.print("#");
      else Serial.print(" ");
    }
    Serial.println("|");
  }
  for (uint16_t index = 0; index < pixleLength + 2; index++) Serial.print("=");
  Serial.println();
  while (millis() - frameTimer < holdTime);
}

