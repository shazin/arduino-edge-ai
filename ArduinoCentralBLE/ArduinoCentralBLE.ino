#include "Arduino_LED_Matrix.h"

#include <ArduinoBLE.h>

BLEService happyService("19B10000-E8F2-537E-4F6C-D104768A1214"); // Bluetooth® Low Energy LED Service

// Bluetooth® Low Energy LED Switch Characteristic - custom 128-bit UUID, read and writable by central
BLEByteCharacteristic switchCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);

ArduinoLEDMatrix matrix;

void setup() {
  Serial.begin(115200);
  matrix.begin();

  while (!Serial);

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy module failed!");

    while (1);
  }

  // set advertised local name and service UUID:
  BLE.setLocalName("LED");
  BLE.setAdvertisedService(happyService);

  // add the characteristic to the service
  happyService.addCharacteristic(switchCharacteristic);

  // add service
  BLE.addService(happyService);

  // set the initial value for the characeristic:
  switchCharacteristic.writeValue(0);

  // start advertising
  BLE.advertise();

  Serial.println("BLE Happy Peripheral");

  leftEye();
  rightEye();
  happyMouth();
}

uint8_t frame[8][12] = {
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

void leftEye(){
  //Left eye
  frame[1][3] = 1;
  frame[1][4] = 1;
  frame[2][3] = 1;
  frame[2][4] = 1;
}

void wink(){
  //Wink with the left eye
  frame[1][3] = 0;
  frame[1][4] = 0;
  frame[2][3] = 1;
  frame[2][4] = 1;
}

void rightEye(){
  //Right eye
  frame[1][8] = 1;
  frame[1][9] = 1;
  frame[2][8] = 1;
  frame[2][9] = 1;
}

void happyMouth(){
  //Mouth
  frame[6][3] = 0;
  frame[6][9] = 0;
  frame[4][3] = 1;
  frame[4][9] = 1;
  frame[5][3] = 1;
  frame[5][4] = 1;
  frame[5][5] = 1;
  frame[5][6] = 1;
  frame[5][7] = 1;
  frame[5][8] = 1;
  frame[5][9] = 1;
}

void sadMouth(){
  //Mouth
  frame[4][3] = 0;
  frame[4][9] = 0;
  frame[6][3] = 1;
  frame[6][9] = 1;
  frame[5][3] = 1;
  frame[5][4] = 1;
  frame[5][5] = 1;
  frame[5][6] = 1;
  frame[5][7] = 1;
  frame[5][8] = 1;
  frame[5][9] = 1;
}

void loop(){


  BLEDevice central = BLE.central();

  // if a central is connected to peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());

    // while the central is still connected to peripheral:
    while (central.connected()) {
      // if the remote device wrote to the characteristic,
      // use the value to control the LED:
      if (switchCharacteristic.written()) {
        if (switchCharacteristic.value()) {   // any value other than 0
          Serial.println("Happy on");
          happyMouth();       // will turn the LED on
          matrix.renderBitmap(frame, 8, 12);
        } else {                              // a 0 value
          Serial.println(F("Happy off"));
          sadMouth();        // will turn the LED off
          matrix.renderBitmap(frame, 8, 12);
        }
      }

      //delay(1000);
      //wink();

      matrix.renderBitmap(frame, 8, 12);
      //delay(1000);
    }
  }

    // when the central disconnects, print it out:
  Serial.print(F("Disconnected from central: "));
  Serial.println(central.address());

  matrix.renderBitmap(frame, 8, 12);

  delay(1000);
  //wink();
  
  matrix.renderBitmap(frame, 8, 12);
  delay(1000);
}