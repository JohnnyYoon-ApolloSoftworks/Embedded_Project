#include <RTClib_Johnny.h>
#include <Stepper.h>
#include "HX711.h"
#include "settings.h"

Hx711 scale(A1, A0);
DS1302 rtc;

// IN4, IN2, IN3, IN1
Stepper stepMotor1(stepsPerRevolution, motor1In4Pin, motor1In2Pin, motor1In3Pin, motor1In1Pin);
Stepper stepMotor2(stepsPerRevolution, motor2In4Pin, motor2In2Pin, motor2In3Pin, motor2In1Pin);
Stepper stepMotor3(stepsPerRevolution, motor3In4Pin, motor3In2Pin, motor3In3Pin, motor3In1Pin);

void rtcCalibration()
{
  rtc.adjust(DateTime(__DATE__, __TIME__));
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

void buzzerAlarm(int num)
{
  for (int i = 0; i < num; i++)
  {
    tone(8, 262);
    delay(100);
    noTone(8);
    delay(100);
  }
  delay(500);
}

void motor1Feed()
{
  while ((scale.gram() * 2) < motor1Gram)
  {
    if ((scale.gram() * 2) >= motor1Gram)
      break;

    stepMotor1.step(stepsPerRevolution * 4);
    delay(100);
  }
  buzzerAlarm(1);
}

void motor2Feed()
{
  for (int i = 0; i < motor2Count; i++)
  {
    stepMotor2.step(stepsPerRevolution * 2);
    delay(100);
  }
  buzzerAlarm(2);
}

void motor3Feed()
{
  for (int i = 0; i < motor3Count; i++)
  {
    stepMotor3.step(stepsPerRevolution * 4);
    delay(100);
  }
  buzzerAlarm(3);
}

void motor1Operating()
{
  DateTime now = rtc.now();

  if ((now.hour() == motor1BreakfastHour && now.minute() == motor1BreakfastMinute) ||
      (now.hour() == motor1LunchHour && now.minute() == motor1LunchMinute) ||
      (now.hour() == motor1DinnerHour && now.minute() == motor1DinnerMinute))
  {
    motor1ReserveState = true;
    motor1ResetTime = now.minute() + 3;

    if (motor1ResetTime >= 60)
      motor1ResetTime -= 60;
  }

  if (motor1ReserveState == true && motor1FeedState == false)
  {
    motor1Feed();
    motor1FeedState = true;

    Serial.println("motor1 is running now");
    Serial.print("motor1 ResetTime is ");
    Serial.println(motor1ResetTime);
  }

  if (now.minute() == motor1ResetTime)
  {
    if (motor1FeedState)
      Serial.println("motor1 Reset!");

    motor1ReserveState = false;
    motor1FeedState = false;
  }
}

void motor2Operating()
{
  DateTime now = rtc.now();

  if ((now.hour() == motor2BreakfastHour && now.minute() == motor2BreakfastMinute) ||
      (now.hour() == motor2LunchHour && now.minute() == motor2LunchMinute) ||
      (now.hour() == motor2DinnerHour && now.minute() == motor2DinnerMinute))
  {
    motor2ReserveState = true;
    motor2ResetTime = now.minute() + 3;

    if (motor2ResetTime >= 60)
      motor2ResetTime -= 60;
  }

  if (motor2ReserveState == true && motor2FeedState == false)
  {
    motor2Feed();
    motor2FeedState = true;

    Serial.println("motor2 is running now");
    Serial.print("motor2 ResetTime is ");
    Serial.println(motor2ResetTime);
  }

  if (now.minute() == motor2ResetTime)
  {
    if (motor2FeedState)
      Serial.println("motor2 Reset!");

    motor2ReserveState = false;
    motor2FeedState = false;
  }
}

void motor3Operating()
{
  DateTime now = rtc.now();

  if ((now.hour() == motor3BreakfastHour && now.minute() == motor3BreakfastMinute) ||
      (now.hour() == motor3LunchHour && now.minute() == motor3LunchMinute) ||
      (now.hour() == motor3DinnerHour && now.minute() == motor3DinnerMinute))
  {
    motor3ReserveState = true;
    motor3ResetTime = now.minute() + 3;

    if (motor3ResetTime >= 60)
      motor3ResetTime -= 60;
  }

  if (motor3ReserveState == true && motor3FeedState == false)
  {
    motor3Feed();
    motor3FeedState = true;

    Serial.println("motor3 is running now");
    Serial.print("motor3 ResetTime is ");
    Serial.println(motor3ResetTime);
  }

  if (now.minute() == motor3ResetTime)
  {
    if (motor3FeedState)
      Serial.println("motor3 Reset!");

    motor3ReserveState = false;
    motor3FeedState = false;
  }
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
    motor1Gram = buffer.substring(firstSeparator + 1, secondSeparator).toInt();

    Serial.print("motor1Gram - ");
    Serial.println(motor1Gram);
    break;

  case 11:
    motor1BreakfastHour = buffer.substring(firstSeparator + 1, secondSeparator).toInt();
    motor1BreakfastMinute = buffer.substring(secondSeparator + 1, lengthOfBuffer).toInt();

    Serial.print("motor1Breakfast - ");
    Serial.print(motor1BreakfastHour);
    Serial.print(":");
    Serial.print(motor1BreakfastMinute);
    Serial.print(":");
    Serial.println("00");
    break;

  case 12:
    motor1LunchHour = buffer.substring(firstSeparator + 1, secondSeparator).toInt();
    motor1LunchMinute = buffer.substring(secondSeparator + 1, lengthOfBuffer).toInt();

    Serial.print("motor1Lunch - ");
    Serial.print(motor1LunchHour);
    Serial.print(":");
    Serial.print(motor1LunchMinute);
    Serial.print(":");
    Serial.println("00");
    break;

  case 13:
    motor1DinnerHour = buffer.substring(firstSeparator + 1, secondSeparator).toInt();
    motor1DinnerMinute = buffer.substring(secondSeparator + 1, lengthOfBuffer).toInt();

    Serial.print("motor1Dinner - ");
    Serial.print(motor1DinnerHour);
    Serial.print(":");
    Serial.print(motor1DinnerMinute);
    Serial.print(":");
    Serial.println("00");
    break;

  case 2:
    motor2Count = buffer.substring(firstSeparator + 1, secondSeparator).toInt();

    Serial.print("motor2Count - ");
    Serial.println(motor2Count);
    break;

  case 21:
    motor2BreakfastHour = buffer.substring(firstSeparator + 1, secondSeparator).toInt();
    motor2BreakfastMinute = buffer.substring(secondSeparator + 1, lengthOfBuffer).toInt();

    Serial.print("motor2Breakfast - ");
    Serial.print(motor2BreakfastHour);
    Serial.print(":");
    Serial.print(motor2BreakfastMinute);
    Serial.print(":");
    Serial.println("00");
    break;

  case 22:
    motor2LunchHour = buffer.substring(firstSeparator + 1, secondSeparator).toInt();
    motor2LunchMinute = buffer.substring(secondSeparator + 1, lengthOfBuffer).toInt();

    Serial.print("motor2Lunch - ");
    Serial.print(motor2LunchHour);
    Serial.print(":");
    Serial.print(motor2LunchMinute);
    Serial.print(":");
    Serial.println("00");
    break;

  case 23:
    motor2DinnerHour = buffer.substring(firstSeparator + 1, secondSeparator).toInt();
    motor2DinnerMinute = buffer.substring(secondSeparator + 1, lengthOfBuffer).toInt();

    Serial.print("motor2Dinner - ");
    Serial.print(motor2DinnerHour);
    Serial.print(":");
    Serial.print(motor2DinnerMinute);
    Serial.print(":");
    Serial.println("00");
    break;

  case 3:
    motor3Count = buffer.substring(firstSeparator + 1, secondSeparator).toInt();

    Serial.print("motor3Count - ");
    Serial.println(motor3Count);
    break;

  case 31:
    motor3BreakfastHour = buffer.substring(firstSeparator + 1, secondSeparator).toInt();
    motor3BreakfastMinute = buffer.substring(secondSeparator + 1, lengthOfBuffer).toInt();

    Serial.print("motor3Breakfast - ");
    Serial.print(motor3BreakfastHour);
    Serial.print(":");
    Serial.print(motor3BreakfastMinute);
    Serial.print(":");
    Serial.println("00");
    break;

  case 32:
    motor3LunchHour = buffer.substring(firstSeparator + 1, secondSeparator).toInt();
    motor3LunchMinute = buffer.substring(secondSeparator + 1, lengthOfBuffer).toInt();

    Serial.print("motor3Lunch - ");
    Serial.print(motor3LunchHour);
    Serial.print(":");
    Serial.print(motor3LunchMinute);
    Serial.print(":");
    Serial.println("00");
    break;

  case 33:
    motor3DinnerHour = buffer.substring(firstSeparator + 1, secondSeparator).toInt();
    motor3DinnerMinute = buffer.substring(secondSeparator + 1, lengthOfBuffer).toInt();

    Serial.print("motor3Dinner - ");
    Serial.print(motor3DinnerHour);
    Serial.print(":");
    Serial.print(motor3DinnerMinute);
    Serial.print(":");
    Serial.println("00");
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

  stepMotor1.setSpeed(300);
  stepMotor2.setSpeed(300);
  stepMotor3.setSpeed(300);

  rtcCalibration();
}

void loop()
{
  if (Serial1.available())
    bluetoothRecieve();

  motor1Operating();
  motor2Operating();
  motor3Operating();

  debugLog();
  delay(1000);
}