// Potentiometer inputs
int potLED = A0;
int potFan = A1;

// PWM Outputs
int outLED = 5;
int outFan = 6;

// Switch
int sw = A2;

// LED logic parameters
int ledHysteresis = 3;
unsigned long ledOffDelay = 1000;

// Fan logic parameters
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
  int setPotLED = 0;

  valPotLED = analogRead(potLED);
  valPotLED = accountHysteresis(previousValPotLED, valPotLED, ledHysteresis);
  setPotLED = map(valPotLED, 0, 1023, 255, 15);
  analogWrite(outLED, setPotLED);

  previousValPotLED = valPotLED;
  lastMillisLEDOn = millis();
}

void handleLEDOff() {
  analogWrite(outLED, 0);
}

int previousValPotFan = 0;

void handleFanOn() {
  int valPotFan = 0;
  int setPotFan = 0;
  
  valPotFan = analogRead(potFan);
  valPotFan = accountHysteresis(previousValPotFan, valPotFan, fanHysteresis);
  setPotFan = map(valPotFan, 0, 1023, 255, 25);
  analogWrite(outFan, setPotFan);
  previousValPotFan = valPotFan;
}

void handelFanOff() {
  analogWrite(outFan, 0);
}


int accountHysteresis(int previousVal, int currentVal, int hysteresis) {
  if (currentVal < previousVal - hysteresis || currentVal > previousVal + hysteresis) {
    return currentVal;
  }
  return previousVal;
}
