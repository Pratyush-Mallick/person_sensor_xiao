#include <Arduino.h>
#include <U8x8lib.h>
#include <Wire.h>
#include <Servo.h>
#include "I2CDriver.h"
 
U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* clock=*/ PIN_WIRE_SCL, /* data=*/ PIN_WIRE_SDA, /* reset=*/ U8X8_PIN_NONE);   // OLEDs without Reset of the Display

I2CDriver i2c = I2CDriver();
static inference_results_t results;
static uint8_t centroid_x;
static uint8_t centroid_y;

Servo servo_x; // A7 <--- 140 left X 0 right -->  Horizontal Movement mapped to X cetroid.
Servo servo_y;  // A0 <--- 60 down Y 0 up  -->  Vertical Movement mapped to Y centroid.

// Maximum servo movement restricted mechanical design.
#define MIN_SERVO      0
#define MAX_SERVO_X    140  
#define MAX_SERVO_Y    60

// Since the camera has a wider field of view, 
// which the servo motors can't match.
// Hence we need to capt the position coordinates.
#define MIN_CENTROID   50
#define MAX_CENTROID   200
#define FAN_SWITCH     D10

uint8_t motor_x_position;
uint8_t motor_y_position;

void setup(void) {

  MbedI2C(D4, D5);
  u8x8.begin();
  u8x8.setFlipMode(1);

  // Configuring the Person Sensor.
  i2c.setMode(i2c.MODE_CONTINUOUS);
  i2c.setIdModelEnabled(true);

  servo_x.attach(D7);
  servo_y.attach(A0); 
  pinMode(FAN_SWITCH, OUTPUT);

  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.setCursor(0, 0);
  u8x8.print("Confidence: ");
}
 
void loop(void) {
  results = i2c.read();
  u8x8.setCursor(11, 0);
  u8x8.print(results.boxes->confidence);
  u8x8.setCursor(0, 2);
  u8x8.print("Person Detected:");
  u8x8.setCursor(0, 4);

  if (results.boxes->confidence > 90) {
    // Turn on the FAN.
    digitalWrite(FAN_SWITCH, HIGH);

    centroid_x = (results.boxes->data[2] + results.boxes->data[0])/2;
    centroid_y = (results.boxes->data[3] + results.boxes->data[1])/2;

    // Limiting the centroid range to avoid overstressing the motor
    // due to mechanical design.
    centroid_x = centroid_x > MAX_CENTROID ? MAX_CENTROID : centroid_x;
    centroid_y = centroid_y > MAX_CENTROID ? MAX_CENTROID : centroid_y;

    // Mapping the positonal coordinates to servo positions.
    motor_x_position = map(centroid_x, MIN_CENTROID, MAX_CENTROID, MIN_SERVO, MAX_SERVO_X);
    motor_y_position = map(centroid_y, MIN_CENTROID, MAX_CENTROID, MIN_SERVO, MAX_SERVO_Y);
    servo_x.write(motor_x_position);
    servo_y.write(motor_y_position);

    u8x8.print("Yes"); 
  } else {
    u8x8.print("No!");
    digitalWrite(FAN_SWITCH, LOW);  
    servo_x.write(MAX_SERVO_X/2);
    servo_y.write(MAX_SERVO_Y/2);
  }
}