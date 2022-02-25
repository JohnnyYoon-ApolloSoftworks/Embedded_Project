#include <Servo.h>
#include <RTClib_Johnny.h>
#include "HX711.h"
#include "settings.h"

Hx711 scale(A1, A0);
DS1302 rtc;
Servo servo1;
Servo servo2;
Servo servo3;


void rtcCalibration()
{
  rtc.adjust(DateTime(__DATE__, __TIME__));
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

void debugLog()
{
  DateTime now = rtc.now();
  Serial.print(now.tostr(buf));
  Serial.print(" | wheight : ");
  Serial.print(scale.gram() * 2, 1);
  Serial.println(" g");
}

void showWeight()
{
  Serial.print(scale.gram() * 2, 1);
  Serial.println(" g");
}

void showDate()
{
  DateTime now = rtc.now();
  Serial.println(now.tostr(buf));
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

void servo1Operating()
{
  DateTime now = rtc.now();

  if((now.hour() == servo1BreakfastHour && now.minute() == servo1BreakfastMinute) ||
     (now.hour() == servo1LunchHour && now.minute() == servo1LunchMinute) ||
     (now.hour() == servo1DinnerHour && now.minute() == servo1DinnerMinute))
  {
    servo1ReserveState = true;
    servo1ResetTime = now.minute() + 5;

    if(servo1ResetTime >= 60)
      servo1ResetTime -= 60;
  }

  if (servo1ReserveState == true && servo1RunState == false)
  {
    servo1Run();
    servo1RunState = true;
    
    Serial.println("servo1 is running now");
    Serial.print("servo1 ResetTime is ");
    Serial.println(servo1ResetTime);
  }

  if(now.minute() == servo1ResetTime)
  {
    if(servo1RunState)
      Serial.println("servo1 Reset!");

    servo1ReserveState = false;
    servo1RunState = false;
  }
}

void servo2Operating()
{
  DateTime now = rtc.now();

  if((now.hour() == servo2BreakfastHour && now.minute() == servo2BreakfastMinute) ||
     (now.hour() == servo2LunchHour && now.minute() == servo2LunchMinute) ||
     (now.hour() == servo2DinnerHour && now.minute() == servo2DinnerMinute))
  {
    servo2ReserveState = true;
    servo2ResetTime = now.minute() + 5;

       if(servo2ResetTime >= 60)
        servo2ResetTime -= 60;
  }

  if (servo2ReserveState == true && servo2RunState == false)
  {
    servo2Run();
    servo2RunState = true;

    Serial.println("servo2 is running now");
    Serial.print("servo2 ResetTime is ");
    Serial.println(servo2ResetTime);
  }

  if(now.minute() == servo2ResetTime)
  {
    if(servo2RunState)
      Serial.println("servo2 Reset!");

    servo2ReserveState = false;
    servo2RunState = false;
  }
}

void servo3Operating()
{
  DateTime now = rtc.now();

  if((now.hour() == servo3BreakfastHour && now.minute() == servo3BreakfastMinute) ||
     (now.hour() == servo3LunchHour && now.minute() == servo3LunchMinute) ||
     (now.hour() == servo3DinnerHour && now.minute() == servo3DinnerMinute))
  {
    servo3ReserveState = true;
    servo3ResetTime = now.minute() + 5;

       if(servo3ResetTime >= 60)
      servo3ResetTime -= 60;
  }

  if (servo3ReserveState == true && servo3RunState == false)
  {
    servo3Run();
    servo3RunState = true;

    Serial.println("servo3 is running now");
    Serial.print("servo3 ResetTime is ");
    Serial.println(servo3ResetTime);
  }

  if(now.minute() == servo3ResetTime)
  {
    if(servo3RunState)
      Serial.println("servo3 Reset!");

    servo3ReserveState = false;
    servo3RunState = false;
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
    
    Serial.print("servo1Gram - ");
    Serial.println(servo1Gram);
    break;

  case 11:
    servo1BreakfastHour = buffer.substring(firstSeparator + 1, secondSeparator).toInt();
    servo1BreakfastMinute = buffer.substring(secondSeparator + 1, lengthOfBuffer).toInt();
    
    Serial.print("servo1Breakfast - ");
    Serial.print(servo1BreakfastHour);
    Serial.print(" : ");
    Serial.println(servo1BreakfastMinute);
    break;

  case 12:
    servo1LunchHour = buffer.substring(firstSeparator + 1, secondSeparator).toInt();
    servo1LunchMinute = buffer.substring(secondSeparator + 1, lengthOfBuffer).toInt();

    Serial.print("servo1Lunch - ");
    Serial.print(servo1LunchHour);
    Serial.print(" : ");
    Serial.println(servo1LunchMinute);
    break;

  case 13:
    servo1DinnerHour = buffer.substring(firstSeparator + 1, secondSeparator).toInt();
    servo1DinnerMinute = buffer.substring(secondSeparator + 1, lengthOfBuffer).toInt();

    Serial.print("servo1Dinner - ");
    Serial.print(servo1DinnerHour);
    Serial.print(" : ");
    Serial.println(servo1DinnerMinute);
    break;

  case 2:
    servo2Count = buffer.substring(firstSeparator + 1, secondSeparator).toInt();

    Serial.print("servo2Count - ");
    Serial.println(servo2Count);
    break;

  case 21:
    servo2BreakfastHour = buffer.substring(firstSeparator + 1, secondSeparator).toInt();
    servo2BreakfastMinute = buffer.substring(secondSeparator + 1, lengthOfBuffer).toInt();

    Serial.print("servo2Breakfast - ");
    Serial.print(servo2BreakfastHour);
    Serial.print(" : ");
    Serial.println(servo2BreakfastMinute);
    break;

  case 22:
    servo2LunchHour = buffer.substring(firstSeparator + 1, secondSeparator).toInt();
    servo2LunchMinute = buffer.substring(secondSeparator + 1, lengthOfBuffer).toInt();

    Serial.print("servo2Lunch - ");
    Serial.print(servo2LunchHour);
    Serial.print(" : ");
    Serial.println(servo2LunchMinute);
    break;

  case 23:
    servo2DinnerHour = buffer.substring(firstSeparator + 1, secondSeparator).toInt();
    servo2DinnerMinute = buffer.substring(secondSeparator + 1, lengthOfBuffer).toInt();

    Serial.print("servo2Dinner - ");
    Serial.print(servo2DinnerHour);
    Serial.print(" : ");
    Serial.println(servo2DinnerMinute);
    break;

  case 3:
    servo3Count = buffer.substring(firstSeparator + 1, secondSeparator).toInt();

    Serial.print("servo3Count - ");
    Serial.println(servo3Count);
    break;

  case 31:
    servo3BreakfastHour = buffer.substring(firstSeparator + 1, secondSeparator).toInt();
    servo3BreakfastMinute = buffer.substring(secondSeparator + 1, lengthOfBuffer).toInt();

    Serial.print("servo3Breakfast - ");
    Serial.print(servo3BreakfastHour);
    Serial.print(" : ");
    Serial.println(servo3BreakfastMinute);
    break;

  case 32:
    servo3LunchHour = buffer.substring(firstSeparator + 1, secondSeparator).toInt();
    servo3LunchMinute = buffer.substring(secondSeparator + 1, lengthOfBuffer).toInt();

    Serial.print("servo3Lunch - ");
    Serial.print(servo3LunchHour);
    Serial.print(" : ");
    Serial.println(servo3LunchMinute);
    break;

  case 33:
    servo3DinnerHour = buffer.substring(firstSeparator + 1, secondSeparator).toInt();
    servo3DinnerMinute = buffer.substring(secondSeparator + 1, lengthOfBuffer).toInt();

    Serial.print("servo3Dinner - ");
    Serial.print(servo3DinnerHour);
    Serial.print(" : ");
    Serial.println(servo3DinnerMinute);
    break;

  default:
    Serial.print("Wrong Input : ");
    Serial.println(Serial1.read());
    break;
  }
}

void setup()
{
  Serial.begin(9600);
  Serial1.begin(9600);

  rtc.begin();

  pinMode(buzzerPin, OUTPUT);

  rtcCalibration();
}

void loop()
{
  if (Serial1.available())
    bluetoothRecieve();

  servo1Operating();
  servo2Operating();
  servo3Operating();

  debugLog();

  delay(5000);
}