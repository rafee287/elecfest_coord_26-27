#include <MD_MAX72xx.h>
#include <SPI.h>

// Define your hardware type. Most common is FC16_HW.
// Options: GENERIC_HW, FC16_HW, PAROLA_HW, ICSTATION_HW
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 1  // Number of 8x8 matrices connected
#define CS_PIN 5       // The pin connected to CS/Load
#define joyx 32         // joystick x pin
#define joyy 33         // joystick y pin

int shift = 0;

// Initialize the library
MD_MAX72XX matrix = MD_MAX72XX(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

byte start[8] = {
  B00000000,
  B00000000,
  B00011000,
  B00111100,
  B00111100,
  B00011000,
  B00000000,
  B00000000
};


byte north[8] = {
  B00000000,
  B00011000,
  B00111100,
  B01111110,
  B00011000,
  B00011000,
  B00011000,
  B00000000
};


byte south[8] = {
  B00000000,
  B00011000,
  B00011000,
  B00011000,
  B01111110,
  B00111100,
  B00011000,
  B00000000
};


byte west[8] = {
  B00000000,
  B00011000,
  B00011000,
  B00011000,
  B01111110,
  B00111100,
  B00011000,
  B00000000
};


byte east[8] = {
  B00000000,
  B00011000,
  B00111100,
  B01111110,
  B00011000,
  B00011000,
  B00011000,
  B00000000
};



void setup() {
  matrix.begin();              // Initialize the matrix
  matrix.control(MD_MAX72XX::INTENSITY, 2); // Set brightness (0-15)
  matrix.clear();              // Clear any existing pixels
  pinMode(joyx, INPUT);
  pinMode(joyy, INPUT);
  Serial.begin(115200);

}

void loop() {
  int x = analogRead(joyx);
  int y = analogRead(joyy);
  Serial.print("joystick value recived x: ");
  Serial.print(x);
  Serial.print(", y: ");
  Serial.print(y);
  Serial.print(" direction: ");

  if (y > 3796){
    for (int col = 0; col < 8; col++) {
      matrix.setColumn(0, (8+col-shift)%8, east[col]);
    }
    Serial.println("east");
  }
  else if (x > 3796){
    for (int row = 0; row < 8; row++) {
      matrix.setRow(0, (8+row-shift)%8, north[row]);
    }
    Serial.println("north");
  }
  else if (y < 300){
    for (int col = 0; col < 8; col++) {
      matrix.setColumn(0, (col+shift)%8, west[col]);
    }
    Serial.println("west");
  }
  else if (x < 300){
    for (int row = 0; row < 8; row++) {
      matrix.setRow(0, (row+shift)%8, south[row]);
    }
    Serial.println("south");
  }
  else{
    for (int row = 0; row < 8; row++) {
      matrix.setRow(0, row, start[row]);
    }
    Serial.println("none");
    shift = 0;
  }
  shift++;
  shift%= 8;
  delay(500);
}
