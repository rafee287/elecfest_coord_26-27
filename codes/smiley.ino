#include <MD_MAX72xx.h>
#include <SPI.h>

// Define your hardware type. Most common is FC16_HW.
// Options: GENERIC_HW, FC16_HW, PAROLA_HW, ICSTATION_HW
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 1  // Number of 8x8 matrices connected
#define CS_PIN 5       // The pin connected to CS/Load

// Initialize the library
MD_MAX72XX matrix = MD_MAX72XX(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

// 8x8 Smiley Face pattern (1 = LED On, 0 = LED Off)
byte smiley[8] = {
  B00111100,
  B01000010,
  B10100101,
  B10000001,
  B10100101,
  B10011001,
  B01000010,
  B00111100
};

void setup() {
  matrix.begin();              // Initialize the matrix
  matrix.control(MD_MAX72XX::INTENSITY, 2); // Set brightness (0-15)
  matrix.clear();              // Clear any existing pixels
  for (int row = 0; row < 8; row++) {
    matrix.setRow(0, row, smiley[row]);
  }
}

void loop() {

}
