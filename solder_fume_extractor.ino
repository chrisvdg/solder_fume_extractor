// Inputs
int potLED = A3;
int potFan = A2;
int sw = A1;

// Outputs
int outLED = 0;
int outFan = 1;

// LED logic parameters
int ledFloor = 15;
int ledHysteresis = 3;
unsigned long ledOffDelay = 500;

// Fan logic parameters
int fanFloor = 35;
int fanHysteresis = 2;
unsigned long hardStartDuration = 4000;

struct switchState {
  bool led;
  bool fan;
};

void setup() {
  pinMode(potLED, INPUT_PULLUP);
  pinMode(potFan, INPUT_PULLUP);
  pinMode(sw, INPUT_PULLUP);
  
  pinMode(outLED, OUTPUT);
  pinMode(outFan, OUTPUT);
}

void loop() {
  int swVal = 0;
  struct switchState swState;
  swVal = analogRead(sw);
  swState = getSwitchState(swVal);

  if (swState.led) {
    handleLEDOn();
  } else {
    handleLEDOff();
  }

  if (swState.fan) {
    handleFanOn();
  } else {
    handelFanOff();
  }
}

// Switch handlers

struct switchState getSwitchState(int readVal) {
  struct switchState st;
  if (readVal < 300) {
    // Switch position 2
    st.led = true;
    st.fan = true;
  } else if (readVal < 550) {
    // Switch position 1
    st.led = true;
    st.fan = false;
  } else {
    // switch position off
    st.led = false;
    st.fan = false;
  }

  return st;
}

// LED handlers

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
    digitalWrite(outLED, LOW);
    return;
  }

  // Prevent LED from going on at startup
  if (previousValPotLED == 0) {
    return;
  }
  setLED(previousValPotLED);
}

void setLED(int potVal) {
  int setVal = 0;
  setVal = map(potVal, 0, 1023, 255, ledFloor);
  analogWrite(outLED, setVal);
}


// Fan handlers

int previousValPotFan = 0;
unsigned long fanOnTime = 0;

void handleFanOn() {
  int valPotFan = 0;

  if (fanOnTime == 0) {
    fanOnTime = millis();
  }

  if (millis() < fanOnTime + hardStartDuration) {
    digitalWrite(outFan, HIGH);
  }
  
  valPotFan = analogRead(potFan);
  valPotFan = accountHysteresis(previousValPotFan, valPotFan, fanHysteresis);
  setFan(valPotFan);
  previousValPotFan = valPotFan;
}

void handelFanOff() {
  fanOnTime = 0;
  digitalWrite(outFan, LOW);
}

void setFan(int potVal) {
  int setVal = 0;
  setVal = map(potVal, 0, 1023, 255, fanFloor);
  analogWrite(outFan, setVal);
}

// Generic helpers

int accountHysteresis(int previousVal, int currentVal, int hysteresis) {
  if (currentVal < previousVal - hysteresis || currentVal > previousVal + hysteresis) {
    return currentVal;
  }
  return previousVal;
}
