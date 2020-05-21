// Potentiometer inputs
int potLED = A0;
int potFan = A1;

// PWM Outputs
int outLED = 5;
int outFan = 6;

// Switch
int sw = A2;

// LED logic parameters
int ledFloor = 15;
int ledHysteresis = 3;
unsigned long ledOffDelay = 500;

// Fan logic parameters
int fanFloor = 40;
int fanHysteresis = 2;
unsigned long hardStartDuration = 3000;

void setup() {
  pinMode(potLED, INPUT_PULLUP);
  pinMode(potFan, INPUT_PULLUP);
  pinMode(sw, INPUT_PULLUP);
  
  pinMode(outLED, OUTPUT);
  pinMode(outFan, OUTPUT);
}

void loop() {
  bool ledOn;
  bool fanOn;

  int swVal = 0;
  swVal = analogRead(sw);
  if (swVal < 300) {
    // Switch position 2
    ledOn = true;
    fanOn = true;
  } else if (swVal < 550) {
    // Switch position 1
    ledOn = true;
    fanOn = false;
  } else {
    // switch position off
    ledOn = false;
    fanOn = false;
  }


  if (ledOn) {
    handleLEDOn();
  } else {
    handleLEDOff();
  }

  if (fanOn) {
    handleFanOn();
  } else {
    handelFanOff();
  }
}

int previousValPotLED = 0;
unsigned long lastMillisLEDOn = 0;

void handleLEDOn() {
  int valPotLED = 0;

  valPotLED = analogRead(potLED);
  valPotLED = accountHysteresis(previousValPotLED, valPotLED, ledHysteresis);
  setLED(valPotLED);

  previousValPotLED = valPotLED;
  lastMillisLEDOn = millis();
}

void handleLEDOff() {
  if (lastMillisLEDOn + ledOffDelay <= millis()) {
    analogWrite(outLED, 0);
    return;
  }

  // Prevent LED from going on at startup
  if (previousValPotLED == 0) {
    return;
  }
  setLED(previousValPotLED);
}

int previousValPotFan = 0;

void handleFanOn() {
  int valPotFan = 0;
  
  valPotFan = analogRead(potFan);
  valPotFan = accountHysteresis(previousValPotFan, valPotFan, fanHysteresis);
  setFan(valPotFan);
  previousValPotFan = valPotFan;
}

void handelFanOff() {
  analogWrite(outFan, 0);
}


void setLED(int potVal) {
  int setVal = 0;
  setVal = map(potVal, 0, 1023, 255, ledFloor);
  analogWrite(outLED, setVal);
}

void setFan(int potVal) {
  int setVal = 0;
  setVal = map(potVal, 0, 1023, 255, fanFloor);
  analogWrite(outFan, setVal);
}


int accountHysteresis(int previousVal, int currentVal, int hysteresis) {
  if (currentVal < previousVal - hysteresis || currentVal > previousVal + hysteresis) {
    return currentVal;
  }
  return previousVal;
}
