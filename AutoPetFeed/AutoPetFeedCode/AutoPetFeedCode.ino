#include <Servo.h>
#include <RTClib_Johnny.h>
#include "HX711_ASW.h"
#include "settings.h"

Hx711 scale(A1, A0);
DS1302 rtc;
Servo servo1;
Servo servo2;
Servo servo3;

void showWeight()
{
  Serial.print(scale.gram() * 2, 1);
  Serial.println(" g");
}

void rtcCalibration()
{
  rtc.adjust(DateTime(__DATE__, __TIME__));
}

void showDate()
{
  DateTime now = rtc.now();
  Serial.println(now.tostr(buf));
}

void servoCalibration()
{
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);
  servo3.attach(servo3Pin);
  servo1.write(servoStop);
  servo2.write(servoStop);
  servo3.write(servoStop);
}

void servo1Run()
{
  servo1.attach(servo1Pin);

  // if ((scale.gram() * 2) < gram)
  // {
  //   servo1.write(servoRun);
  //   delay(angle225Delay);
  //   servo1.write(servoStop);
  //   delay(stopDelay);
  // }

  while ((scale.gram() * 2) < servo1Gram)
  {
    if ((scale.gram() * 2) >= servo1Gram)
      break;
    servo1.write(servoRun);
    delay(angle225Delay);
    servo1.write(servoStop);
    delay(stopDelay);
  }

  servo1.detach();
}

void servo2Run()
{
  servo2.attach(servo2Pin);

  for (int i = 0; i < servo2Count; i++)
  {
    servo2.write(servoRun);
    delay(angle225Delay);
    servo2.write(servoStop);
    delay(stopDelay);
  }

  servo2.detach();
}

void servo3Run()
{
  servo3.attach(servo3Pin);

  for (int i = 0; i < servo3Count; i++)
  {
    servo3.write(servoRun);
    delay(angle450Delay);
    servo3.write(servoStop);
    delay(stopDelay);
  }

  servo3.detach();
}

void bluetoothRecieve()
{
  String buffer = Serial1.readStringUntil('\n');

  int firstSeparator = buffer.indexOf("/", 0);
  int secondSeparator = buffer.indexOf("/", (firstSeparator + 1));
  int lengthOfBuffer = buffer.length();

  int modeBuffer = buffer.substring(0, firstSeparator).toInt();

  switch (modeBuffer)
  {
  case 1:
    servo1Gram = buffer.substring(firstSeparator + 1, secondSeparator).toInt();
    break;

  case 11:
    servo1BreakfastHour = buffer.substring(firstSeparator + 1, secondSeparator).toInt();
    servo1BreakfastMinute = buffer.substring(secondSeparator + 1, lengthOfBuffer).toInt();
    break;

  case 12:
    servo1LunchHour = buffer.substring(firstSeparator + 1, secondSeparator).toInt();
    servo1LunchMinute = buffer.substring(secondSeparator + 1, lengthOfBuffer).toInt();
    break;

  case 13:
    servo1DinnerHour = buffer.substring(firstSeparator + 1, secondSeparator).toInt();
    servo1DinnerMinute = buffer.substring(secondSeparator + 1, lengthOfBuffer).toInt();
    break;

  case 2:
    servo2Count = buffer.substring(firstSeparator + 1, secondSeparator).toInt();
    break;

  case 21:
    servo2BreakfastHour = buffer.substring(firstSeparator + 1, secondSeparator).toInt();
    servo2BreakfastMinute = buffer.substring(secondSeparator + 1, lengthOfBuffer).toInt();
    break;

  case 22:
    servo2LunchHour = buffer.substring(firstSeparator + 1, secondSeparator).toInt();
    servo2LunchMinute = buffer.substring(secondSeparator + 1, lengthOfBuffer).toInt();
    break;

  case 23:
    servo2DinnerHour = buffer.substring(firstSeparator + 1, secondSeparator).toInt();
    servo2DinnerMinute = buffer.substring(secondSeparator + 1, lengthOfBuffer).toInt();
    break;

  case 3:
    servo3Count = buffer.substring(firstSeparator + 1, secondSeparator).toInt();
    break;

  case 31:
    servo3BreakfastHour = buffer.substring(firstSeparator + 1, secondSeparator).toInt();
    servo3BreakfastMinute = buffer.substring(secondSeparator + 1, lengthOfBuffer).toInt();
    break;

  case 32:
    servo3LunchHour = buffer.substring(firstSeparator + 1, secondSeparator).toInt();
    servo3LunchMinute = buffer.substring(secondSeparator + 1, lengthOfBuffer).toInt();
    break;

  case 33:
    servo3DinnerHour = buffer.substring(firstSeparator + 1, secondSeparator).toInt();
    servo3DinnerMinute = buffer.substring(secondSeparator + 1, lengthOfBuffer).toInt();
    break;

  default:
    //nothing
    break;
  }
}

void buzzerAlarm(int num = 1)
{
  for (int i = 0; i < num; i++)
  {
    digitalWrite(buzzerPin, HIGH);
    delay(100);
    digitalWrite(buzzerPin, LOW);
    delay(100);
  }
  delay(500);
}

void debug()
{
  showWeight();
  showDate();
}

void setup()
{
  Serial.begin(9600);
  Serial1.begin(9600);

  rtc.begin();

  pinMode(buzzerPin, OUTPUT);
}

void loop()
{
  if (Serial1.available())
    bluetoothRecieve();

  debug();
  delay(100);
}