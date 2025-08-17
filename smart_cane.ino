//------------------------------ include the Library --------------------------
#include <RTClib.h>       // real time clock
#include <WT588D.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <Wire.h>         // I2C

//------------------------------ define voice ---------------------------------
#define am 37
#define pm 113
#define oh 105
#define oclock 104
#define hourOffset 0
#define tensMinutesOffset 0x12

//------------------------------ Setup RTC ------------------------------------
RTC_DS1307 rtc;
String thisTime = "";
boolean amFlag = true; // false if pm
int minuteValue;
int hourValue;

//------- set the correct pin connections for the WT588D chip -----------------
#define WT588D_RST 3   // Module pin "REST"
#define WT588D_CS 6    // Module pin "P02"
#define WT588D_SCL 7   // Module pin "P03"
#define WT588D_SDA 5   // Module pin "P01"
#define WT588D_BUSY 4  // Module pin "LED/BUSY"

WT588D myWT588D(WT588D_RST, WT588D_CS, WT588D_SCL, WT588D_SDA, WT588D_BUSY);
#define talkPin 2      // pin used to request time
#define ledPin 13      // onboard LED

//-------- set the correct pin connections ------------------------------------
#define trigPin 13     // pin used to ultrasonic
#define echoPin 12     // pin used to ultrasonic
#define Buzzer 11      // pin used to buzzer
#define vibrate 10     // pin used to vibrate

//---------------------------- void setup -------------------------------------
void setup() {
  Serial.begin(38400);

  pinMode(talkPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); // turn off onboard LED

  pinMode(trigPin, OUTPUT);  // ultrasonic
  pinMode(echoPin, INPUT);
  pinMode(Buzzer, OUTPUT);
  pinMode(vibrate, OUTPUT);

  //---------------------- initialize the chip and port mapping ---------------
  myWT588D.begin();

  //--------------------------- Setup RTC -------------------------------------
  Wire.begin();
  rtc.begin();

  // automatically sets the RTC to the date & time on PC this sketch was compiled
  if (!rtc.begin())
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  {
    Serial.println("RTC is NOT running!");
  }

  //----------------------------- play boot up sound --------------------------
  speakPhrase(116); // ready
  speakPhrase(60);  // digital clock
}

//*************************************************************************************
// main loop
//*************************************************************************************
void loop() {
  ultrasonic();
  search_cane();

  //------------------------------- read the clock ----------------------------
  DateTime now = rtc.now();
  minuteValue = now.minute();
  hourValue = now.hour();

  //--------------------------- convert to 12 hour mode -----------------------
  if (now.hour() >= 12) {
    amFlag = false;
    hourValue = now.hour() - 12;
  } else amFlag = true;

  if (now.hour() == 0) hourValue = 12; // midnight

  // see if the Talk Time button was pressed
  if (digitalRead(talkPin) == 0) { // talk key pressed
    Serial.println(String(now.hour(), DEC) + ":" + String(now.minute(), DEC));
    delay(200);
    speakTime();
  }
} // end main loop

// ------------------------------ void speakTime -------------------------------
void speakTime() {
  DateTime now = rtc.now();
  Serial.print("The time is: ");
  Serial.println(String(now.hour(), DEC) + ":" + String(now.minute(), DEC));

  speakPhrase(0x88); // the time is

  //----------------------------- speak hour ----------------------------------
  if (minuteValue == 0) speakPhrase(oclock);
  else speakPhrase(hourValue + hourOffset);

  //------------------------------ speak minutes ------------------------------
  if (minuteValue > 0 && minuteValue < 21) {
    if (minuteValue < 10) {
      speakPhrase(oh);
    }
    speakPhrase(minuteValue);
  }

  if (minuteValue > 20) {
    int temp = minuteValue / 10;
    int temp2 = minuteValue;
    speakPhrase(temp + tensMinutesOffset);

    // only speak minutes 20, 30, 40 or 50
    if ((10 * temp) != temp2) {
      speakPhrase(minuteValue - (10 * temp));
      // only speak unit minutes 1-9
    }
  }

  //------------------------ add am or pm -------------------------------------
  if (amFlag) speakPhrase(am);
  else speakPhrase(pm);
} // end speakTime

// -------------------------- void busy ----------------------------------------
void busy(int pause) {
  delay(100);
  while (myWT588D.isBusy()) {
  }
  delay(pause);
}

//---------------------------- void speakPhrase -------------------------------
void speakPhrase(int phrase) {
  myWT588D.playSound(phrase);
  busy(0);
}

//---------------------------- void search_cane -------------------------------
void search_cane() {
  if (Serial.available()) {
    char val = Serial.read();
    if (val == 'A') {
      digitalWrite(Buzzer, HIGH);
      delay(300);
      digitalWrite(Buzzer, LOW);
    } else if (val == 'a') {
      digitalWrite(Buzzer, LOW);
    }
  }
}

//---------------------------- void ultrasonic --------------------------------
void ultrasonic() {
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  if (distance > 300) // Checking the distance, you can change the value
    distance = 300;

  if (distance < 50) {
    Serial.print(distance);
    Serial.println(" Object Alert");

    digitalWrite(Buzzer, HIGH);
    digitalWrite(vibrate, HIGH);

    for (int i = distance; i > 0; i--)
      delay(20);

    digitalWrite(Buzzer, LOW);
    digitalWrite(vibrate, LOW);

    for (int i = distance; i > 0; i--)
      delay(20);
  }
}
