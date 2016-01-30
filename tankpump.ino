/***********************************************************************************
 * Aquarium Refill
 *
 *  Wiring
 *  Pin 2 - Float switch for tank
 *  Pin 3 - Float switch for bucket
 *  Pin 4 - Pump
 *  Pin 7 - LED 
 *
 *  Control Behavior:
 *    If the tank float switch is not floating (empty tank) and the bucket float switch is floating (bucket not empty) then turn on the pump until the tank float switch floats
 *      or the bucket float switch stops floating
 *    If the bucket float switch stops floating, illuminate the LED
 *
 *
 ***********************************************************************************/
//define the input/output pins
#define TANK_FLOAT_PIN 2
#define BUCKET_FLOAT_PIN 3
#define PUMP_PIN 4
#define LED_PIN 7
#define SWITCH_PIN 5
#define SENSOR_PIN 6

const long veryFastBlink = 250;
const long fastBlink = 500;
const long medBlink = 1000;
const long slowBlink = 2000;
const long ledOff = -1;
const long ledOn = 0;

unsigned long prevMillis = 0;
int ledState = LOW;
int fillingState = 0;
unsigned long fillingStart = 0;

void setup()
{
  //setup input pins for float switch 
  //To use a bare switch with no external pullup resistor, set the pin mode to 
  //INPUT_PULLUP to use internal pull resistors. This will invert the standard high/low behavior.
  pinMode(TANK_FLOAT_PIN, INPUT_PULLUP);
  pinMode(BUCKET_FLOAT_PIN, INPUT_PULLUP);
  pinMode(SWITCH_PIN, INPUT_PULLUP);
  pinMode(SENSOR_PIN, INPUT_PULLUP);
  
  //setup output pins for relays/pumping station and LED board
  pinMode(PUMP_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
}

void loop()
{  
  if (switchIsOff()) {
     debug(8);
     ledTo(slowBlink);
     pumpOff();
  }
  else if (sensorIsTripped()) {
     debug(9);
     ledTo(veryFastBlink);
     pumpOff();
  }
  else if (bucketIsLow()) {
     debug(10);
     ledTo(fastBlink);
     pumpOff();
  }
  else if (tankIsLow() || inFillingState()) {    
     debug(11);
     ledTo(ledOn);
     pumpOn();
  }
  else {
     debug(12);
     ledTo(ledOff);
     pumpOff();
  }
}

int inFillingState() {
  if (millis() - fillingStart > 10000){
    fillingState = 0;
  }
  
  return fillingState;
}

void debug(int pin){
  digitalWrite(8, pin == 8 ? HIGH : LOW);
  digitalWrite(9, pin == 9 ? HIGH : LOW);
  digitalWrite(10, pin == 10 ? HIGH : LOW);
  digitalWrite(11, pin == 11 ? HIGH : LOW);
  digitalWrite(12, pin == 12 ? HIGH : LOW);
}

void ledTo(long interval){  
  if (interval == ledOn){
      digitalWrite(LED_PIN, HIGH);
  }
  else if (interval == ledOff){
      digitalWrite(LED_PIN, LOW);
  }
  else {
      unsigned long currMillis = millis();
      if (currMillis - prevMillis >= interval) {
          prevMillis = currMillis;  
          ledState = ledState == LOW ? HIGH : LOW;
          digitalWrite(LED_PIN, ledState);
      }  
  }
}

int switchIsOff(){ return digitalRead(SWITCH_PIN) == HIGH; }
int bucketIsLow(){ return digitalRead(BUCKET_FLOAT_PIN) == LOW; }
int tankIsLow(){ return digitalRead(TANK_FLOAT_PIN) == LOW; }
int sensorIsTripped(){ return digitalRead(SENSOR_PIN) == LOW; }

void pumpOn() { 
  digitalWrite(PUMP_PIN, HIGH);
  if (!fillingState){
    fillingState = 1;
    fillingStart = millis();
  }
}

void pumpOff() { 
  digitalWrite(PUMP_PIN, LOW); 
  fillingState = 0;
}


