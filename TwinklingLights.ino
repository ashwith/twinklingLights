#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// Upper and lower limits for minimum and maxmimum
// brightness
const int maxBrightRange[] = {900, 1150};
const int minBrightRange[] = {250, 300};

// Variables to hold each LED's current brightness level,
// change of brightness level per cycle and the range of
// brightness levels.
struct {
  int brightness = 0;
  int brightDelta = 16;
  int brightMax = 0;
  int brightMin = 0;
} led[16];

// Iterator
byte ledID;

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
 
void setup() {

// Uncomment lines with "Serial." to watch variables
// on the monitor.

  pwm.begin();

//  Serial.begin(9600);

  // Set to maximum PWM frequency.
  pwm.setPWMFreq(1600);
  Wire.setClock(400000);

  // Initialize each LED's status variables with random values.
  // Make sure that the seed is generated using an unconnected
  // analog pin.
  randomSeed(analogRead(0));
  for (ledID = 0; ledID < 16; ledID++){
    led[ledID].brightMax = random(maxBrightRange[0], maxBrightRange[1]);
    led[ledID].brightMin = random(minBrightRange[0], minBrightRange[1]);
    led[ledID].brightness = random(led[ledID].brightMin, led[ledID].brightMax); 
    led[ledID].brightDelta -= random(8);
    pwm.setPWM(ledID, 0, 0);

//    Serial.print(led[ledID].brightMax);
//    Serial.print('\t');
//    Serial.print(led[ledID].brightMin);
//    Serial.print('\t');
//    Serial.print(led[ledID].brightness);
//    Serial.print('\t');
//    Serial.println(led[ledID].brightDelta);
  }

  // Power on test to check if any LED isn't working
  // or if any PWM output is down.
  delay(2000);
  for (ledID = 0; ledID < 16; ledID++){
    for (int pwm_lvl = 0; pwm_lvl < 4096; pwm_lvl++)
      pwm.setPWM(ledID, 0, pwm_lvl);
  }
  delay(200);
  for (ledID = 0; ledID < 16; ledID++){
      pwm.setPWM(ledID, 0, 0);
      delay(100);
  }
  for (ledID = 0; ledID < 16; ledID++){
      pwm.setPWM(ledID, 0, 4095);
      delay(100);
  }
  delay(2000);
}
 
void loop() {

  // Generate the "triangular wave" for each LED.
  // The DC for each PWM cycle should increase and decrease
  // like a triangualr wave.
  for (ledID = 0; ledID < 16; ledID++) 
  {
    // Have we reached the highest brightness level for this LED? If yes, make the
    // change in brightness negative.
    if (led[ledID].brightDelta > 0 && (led[ledID].brightness + led[ledID].brightDelta) > led[ledID].brightMax)
      led[ledID].brightDelta = -led[ledID].brightDelta;
    
    // Have we reached the lowest brightness level for this LED? If yes, make the
    // change in brightness positive.
    else if (led[ledID].brightDelta < 0 && (led[ledID].brightness + led[ledID].brightDelta) < led[ledID].brightMin)
      led[ledID].brightDelta = -led[ledID].brightDelta;

    // Update the LED's brightness.
    led[ledID].brightness += led[ledID].brightDelta;
    pwm.setPWM(ledID, 0, led[ledID].brightness);
  }

  // Make the twinkling frequency ~1Hz
  delay(30);
}
