#include <Wire.h>

int gyroRaw[3];                         // raw sensor data, each axis, pretty useless really but here it is.
double gyroDPS[3];                      // gyro degrees per second, each axis

float heading[3]={0.0f};                // heading[x], heading[y], heading [z]

int gyroZeroRate[3];                    // Calibration data.  Needed because the sensor does center at zero, but rather always reports a small amount of rotation on each axis.
int gyroThreshold[3];                   // Raw rate change data less than the statistically derived threshold is discarded.

float dpsPerDigit=.00875f;              // for conversion to degrees per second

void setup() {
  Serial.begin(9600);
  Wire.begin();
  setupGyro();
  calibrateGyro();
}

void loop() {
  updateGyroValues();
  updateHeadings();
  //testCalibration();

  printDPS();
  Serial.print("   -->   ");
  printHeadings();
  Serial.println();
}




