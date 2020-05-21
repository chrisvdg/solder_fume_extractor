int potLED = A0;
int potFan = A1;

int outLED = 5;
int outFan = 6;

int sw = A2;

void setup() {
  pinMode(potLED, INPUT_PULLUP);
  pinMode(potFan, INPUT_PULLUP);
  pinMode(sw, INPUT_PULLUP);
  
  pinMode(outLED, OUTPUT);
  pinMode(outFan, OUTPUT);

//  Serial.begin(9600);  
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
//    Serial.println("Led is on");
    
    int valPotLED = 0;
    int setPotLED = 0;
    valPotLED = analogRead(potLED);
    setPotLED = map(valPotLED, 0, 1023, 255, 0);
    analogWrite(outLED, setPotLED);
  } else {
    analogWrite(outLED, 0);
  }

  if (fanOn) {
//    Serial.println("Fan is on");
   
    int valPotFan = 0;
    int setPotFan = 0;
    valPotFan = analogRead(potFan);
    setPotFan = map(valPotFan, 0, 1023, 255, 25);
    analogWrite(outFan, setPotFan);
  } else {
    analogWrite(outFan, 0);
  }

//  Serial.println(swVal);
//  int valPotLED = 0;
//  valPotLED = analogRead(potLED);
//  Serial.println(valPotLED);
//  int valPotFan = 0;
//  valPotFan = analogRead(potFan);
//  Serial.println(valPotFan);
//  Serial.println();
//  Serial.println();
//  delay(1000);
  
}
