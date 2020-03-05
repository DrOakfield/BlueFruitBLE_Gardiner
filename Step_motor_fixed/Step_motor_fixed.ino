#include <SPI.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"

#include "BluefruitConfig.h"




#define FACTORYRESET_ENABLE         0
#define MINIMUM_FIRMWARE_VERSION    "0.6.6"
#define MODE_LED_BEHAVIOUR          "MODE"

Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);


// A small helper
void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}


// Include the Arduino Stepper.h library:
#include <Stepper.h>
// Define number of steps per rotation:
const int stepsPerRevolution = 2048;
// Wiring:
// Pin 8 to IN1 on the ULN2003 driver
// Pin 9 to IN2 on the ULN2003 driver
// Pin 10 to IN3 on the ULN2003 driver
// Pin 11 to IN4 on the ULN2003 driver
// Create stepper object called 'myStepper', note the pin order:
Stepper myStepper = Stepper(stepsPerRevolution, 8, 10, 9, 11);


//default last position at startup is 0%
int lastPos = 0;





int rollTo100(int lastPos){
    
    switch (lastPos) {
      case 0:
        myStepper.step(stepsPerRevolution * 4);
        break;

      case 25:
        myStepper.step(stepsPerRevolution * 3);
        break;

      case 50:
        myStepper.step(stepsPerRevolution * 2);
        break;
        
      case 75:
        myStepper.step(stepsPerRevolution * 1);
        break;

      case 100:
        break;

      break;
    }

    lastPos = 100;
   
    return lastPos;
}

int rollTo75(int lastPos){
    
    switch (lastPos) {
      case 0:
        myStepper.step(stepsPerRevolution * 3);
        break;

      case 25:
        myStepper.step(stepsPerRevolution * 2);
        break;

      case 50:
        myStepper.step(stepsPerRevolution);
        break;
        
      case 75:
        break;

      case 100:
        myStepper.step(-stepsPerRevolution);
        break;

      break;
    }

    lastPos = 75;
   
    return lastPos;
}

int rollTo50(int lastPos) {
  
  switch (lastPos) {
      case 0:
        myStepper.step(stepsPerRevolution * 2);
        break;

      case 25:
        myStepper.step(stepsPerRevolution);
        break;

      case 50:
        break;
        
      case 75:
        myStepper.step(-stepsPerRevolution);
        break;

      case 100:
        myStepper.step(-stepsPerRevolution * 2);
        break;

      break;
    }

    lastPos = 50;
   
    return lastPos;  
   
}
  

int rollTo25(int lastPos){
  
  switch (lastPos) {
      case 0:
        myStepper.step(stepsPerRevolution);
        break;

      case 25:
        break;

      case 50:
        myStepper.step(-stepsPerRevolution);
        break;
        
      case 75:
        myStepper.step(-stepsPerRevolution * 2);
        break;

      case 100:
        myStepper.step(-stepsPerRevolution * 3);
        break;

      break;
    }

    lastPos = 25;
   
    return lastPos;  
  
}

int rollTo0(int lastPos){
  
  
  switch (lastPos) {
      case 0:
        break;

      case 25:
        myStepper.step(-stepsPerRevolution);
        break;

      case 50:
        myStepper.step(-stepsPerRevolution * 2);
        break;
        
      case 75:
        myStepper.step(-stepsPerRevolution * 3);
        break;

      case 100:
        myStepper.step(-stepsPerRevolution * 4);
        break;

      break;
    }

    lastPos = 0;
   
    return lastPos;  
}


void setup() {
  myStepper.setSpeed(12);
  
  // Begin Serial communication at a baud rate of 9600:
  Serial.begin(9600);

  if ( !ble.begin(VERBOSE_MODE) )
  {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }
  Serial.println( F("OK!") );
    
    
  if ( FACTORYRESET_ENABLE )
  {
    /* Perform a factory reset to make sure everything is in a known state */
    Serial.println(F("Performing a factory reset: "));
    if ( ! ble.factoryReset() ) {
      error(F("Couldn't factory reset"));
    }
  }


  Serial.println(F("******************************"));
  //Give module a new name
  ble.println("AT+GAPDEVNAME=CURTAINCONTROLLER"); // named CURTAINCONTROLLER

  // Check response status
  ble.waitForOK();

  /* Wait for connection */
  while (! ble.isConnected()) {
    delay(500);
  }
  // Set module to DATA mode
  Serial.println( F("Switching to DATA mode!") );
  ble.setMode(BLUEFRUIT_MODE_DATA);

  Serial.println(F("******************************"));
  
  
  //vi modtager input fra app 100
  /*lastPos = rollTo75(lastPos);
  delay(2000);
  lastPos = rollTo25(lastPos);
  delay(2000);
  lastPos = rollTo100(lastPos);
  delay(2000);
  lastPos = rollTo50(lastPos);
  delay(2000);
  rollTo0(lastPos);
  */
}
void loop() {
  if (ble.available()) {
    Serial.print("* "); Serial.print(ble.available()); Serial.println(F(" bytes available from BTLE"));
  }
  // Echo received data
  while ( ble.available() )
  {
    int c = ble.read();
    Serial.print((char)c);
  }
  delay(1000);

}
