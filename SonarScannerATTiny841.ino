//
// Sonar distance sensor scanner for 8 HC-SR04's
//
// This sketch is made for the ATTiny841 and enables it to read the distance of 8 HC-SR04's
// The MAX_DISTANCE and PIN_INTERVAL values are related. An HC-SR04 can read an echo of an object
// up to 5m way. To avoid any crosstalk, one then needs to wait 29ms as a minimum.
// If you are only interested in shorter distances, then you can shorten the time between pings, but
// only if you also lower the maximum distance measured by the HC-SR04 using the NewPing library by Tim Eckel.
// The ATTiny841 then puts the distance measured (in CM) in each HC-SR04 in an array of bytes.
// This array can be read via I2C, and is the only thing that can be read from the ATTiny841 in this sketch.
//
// Code by Gert Sanders, April 2018.
//
// Libraries used:
// NewPing v1.9
//
//
// The Spencer Konde core files for ATTinies
// Upload this with board "ATTiny441/841 (No bootloader)"
// I use a USBasp programmer, so as a programmer I selected "USBasp (ATTinyCore)". Works like a charm, even if it complains about a timing error.
//
// PCB for this sketch available on OSHPark:
// https://oshpark.com/shared_projects/OCg8YC7X
//
// In the I2C Master you can use folowing code:
//
/*

#include <Wire.h>
#define SONAR_NUM 8
#define SONARI2CADDRESS 8
#define SONAR_LOOP_MILLIS 100     // read slave every 100ms

byte sonar[SONAR_NUM] = { 0, 0, 0, 0, 0, 0, 0, 0};
unsigned long sonarlooptimer = 0;

void setup()
{
  Wire.begin(); // join i2c bus (address optional for master)
  //  Serial.begin(115200);
  sonarlooptimer = millis();
}

void loop()
{
  checkSonar();
}

void checkSonar()
{ // this function takes arounf 1000-1030 microseconds to complete
  if ((millis() - sonarlooptimer) > SONAR_LOOP_MILLIS)
  {
    Wire.requestFrom(SONARI2CADDRESS, 8); // device 8, 8 bytes
    for (uint8_t i = 0; i < SONAR_NUM; i++)
    { // Loop through each sensor value and receive byte
      if (Wire.available())
        sonar[i] = Wire.read(); // read 1 byte
    }
    sonarlooptimer = millis();
  }
}

*/



#include <Wire.h>
#include <NewPing.h>

#define SCANNERI2CID 8    // The I2C address of this scanner.
#define SONAR_NUM 8       // Number of sensors.
#define MAX_DISTANCE 150  // Maximum distance (in cm) to ping.

#define SONAR1 10    // 0
#define SONAR2 7     // 1
#define SONAR3 1     // 2
#define SONAR4 9     // 3
#define SONAR5 3     // 4
#define SONAR6 0     // 5
#define SONAR7 8     // 6
#define SONAR8 2     // 7

NewPing sonar[SONAR_NUM] =
{ // Sensor object array.
  NewPing(SONAR1, SONAR1, MAX_DISTANCE), // Each sensor's trigger pin, echo pin, and max distance to ping.
  NewPing(SONAR2, SONAR2, MAX_DISTANCE),
  NewPing(SONAR3, SONAR3, MAX_DISTANCE),
  NewPing(SONAR4, SONAR4, MAX_DISTANCE),
  NewPing(SONAR5, SONAR5, MAX_DISTANCE),
  NewPing(SONAR6, SONAR6, MAX_DISTANCE),
  NewPing(SONAR7, SONAR7, MAX_DISTANCE),
  NewPing(SONAR8, SONAR8, MAX_DISTANCE)
};

volatile uint8_t sonarDistance[SONAR_NUM];

// the setup function runs once when you press reset or power the board
void setup()
{
  Wire.begin(SCANNERI2CID);          // join i2c bus with address #8
  Wire.onRequest(requestEvent);      // register event
  delay(100);                        // take a breather before enless loop
}


// the loop function runs over and over again forever
void loop()
{
  for (uint8_t i = 0; i < SONAR_NUM; i++)
  { // Loop through each sensor and store results.
    sonarDistance[i] = sonar[i].ping_cm();
    if ((sonarDistance[i] == 0) or (sonarDistance[i] > MAX_DISTANCE))
      sonarDistance[i] = MAX_DISTANCE;
  }
}


void requestEvent()
{
  for (uint8_t i = 0; i < SONAR_NUM; i++)
  { // Loop through each sensor value and send byte
    Wire.write(sonarDistance[i]); // respond with message of 1 byte
  }
}

