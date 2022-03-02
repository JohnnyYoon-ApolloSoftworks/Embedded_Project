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

void motor1Feed(int gram)
{
  while ((scale.gram() * 2) < gram)
  {
    if ((scale.gram() * 2) >= gram)
      break;

    stepMotor1.step(stepsPerRevolution * 4);
    delay(100);
  }
  buzzerAlarm(1);
}

void motor2Feed(int count)
{
  for (int i = 0; i < count; i++)
  {
    stepMotor2.step(stepsPerRevolution * 2);
    delay(100);
  }
  buzzerAlarm(2);
}

void motor3Feed(int count)
{
  for (int i = 0; i < count; i++)
  {
    stepMotor3.step(stepsPerRevolution * 4);
    delay(100);
  }
  buzzerAlarm(3);
}

void motor1Operating()
{
  DateTime now = rtc.now();

  if ((now.hour() == motorBreakfastHour && now.minute() == motorBreakfastMinute))
  {
    motor1ReserveStateBreakfast = true;
    motor1ResetTime = now.minute() + 2;

    if (motor1ResetTime >= 60)
      motor1ResetTime -= 60;
  }

  if (motor1ReserveStateBreakfast == true && motor1FeedState == false)
  {
    motor1Feed(motor1GramBreakfast);
    motor1FeedState = true;

    Serial.println("motor1 is running now");
    Serial.print("motor1 ResetTime is ");
    Serial.println(motor1ResetTime);
  }

  if ((now.hour() == motorLunchHour && now.minute() == motorLunchMinute))
  {
    motor1ReserveStateLunch = true;
    motor1ResetTime = now.minute() + 2;

    if (motor1ResetTime >= 60)
      motor1ResetTime -= 60;
  }

  if (motor1ReserveStateLunch == true && motor1FeedState == false)
  {
    motor1Feed(motor1GramLunch);
    motor1FeedState = true;

    Serial.println("motor1 is running now");
    Serial.print("motor1 ResetTime is ");
    Serial.println(motor1ResetTime);
  }

  if ((now.hour() == motorDinnerHour && now.minute() == motorDinnerMinute))
  {
    motor1ReserveStateDinner = true;
    motor1ResetTime = now.minute() + 2;

    if (motor1ResetTime >= 60)
      motor1ResetTime -= 60;
  }

  if (motor1ReserveStateDinner == true && motor1FeedState == false)
  {
    motor1Feed(motor1GramDinner);
    motor1FeedState = true;

    Serial.println("motor1 is running now");
    Serial.print("motor1 ResetTime is ");
    Serial.println(motor1ResetTime);
  }

  if (now.minute() == motor1ResetTime)
  {
    if (motor1FeedState)
      Serial.println("motor1 Reset!");

    motor1ReserveStateBreakfast = false;
    motor1ReserveStateLunch = false;
    motor1ReserveStateDinner = false;
    motor1FeedState = false;
  }
}

void motor2Operating()
{
  DateTime now = rtc.now();

  if ((now.hour() == motorBreakfastHour && now.minute() == motorBreakfastMinute))
  {
    motor2ReserveStateBreakfast = true;
    motor2ResetTime = now.minute() + 2;

    if (motor2ResetTime >= 60)
      motor2ResetTime -= 60;
  }

  if (motor2ReserveStateBreakfast == true && motor2FeedState == false)
  {
    motor2Feed(motor2CountBreakfast);
    motor2FeedState = true;

    Serial.println("motor2 is running now");
    Serial.print("motor2 ResetTime is ");
    Serial.println(motor2ResetTime);
  }

  if ((now.hour() == motorLunchHour && now.minute() == motorLunchMinute))
  {
    motor2ReserveStateLunch = true;
    motor2ResetTime = now.minute() + 2;

    if (motor2ResetTime >= 60)
      motor2ResetTime -= 60;
  }

  if (motor2ReserveStateLunch == true && motor2FeedState == false)
  {
    motor2Feed(motor2CountLunch);
    motor2FeedState = true;

    Serial.println("motor2 is running now");
    Serial.print("motor2 ResetTime is ");
    Serial.println(motor2ResetTime);
  }

  if ((now.hour() == motorDinnerHour && now.minute() == motorDinnerMinute))
  {
    motor2ReserveStateDinner = true;
    motor2ResetTime = now.minute() + 2;

    if (motor2ResetTime >= 60)
      motor2ResetTime -= 60;
  }

  if (motor2ReserveStateDinner == true && motor2FeedState == false)
  {
    motor2Feed(motor2CountDinner);
    motor2FeedState = true;

    Serial.println("motor2 is running now");
    Serial.print("motor2 ResetTime is ");
    Serial.println(motor2ResetTime);
  }

  if (now.minute() == motor2ResetTime)
  {
    if (motor2FeedState)
      Serial.println("motor2 Reset!");

    motor2ReserveStateBreakfast = false;
    motor2ReserveStateLunch = false;
    motor2ReserveStateDinner = false;
    motor2FeedState = false;
  }
}

void motor3Operating()
{
  DateTime now = rtc.now();

  if ((now.hour() == motorBreakfastHour && now.minute() == motorBreakfastMinute))
  {
    motor3ReserveStateBreakfast = true;
    motor3ResetTime = now.minute() + 2;

    if (motor3ResetTime >= 60)
      motor3ResetTime -= 60;
  }

  if (motor3ReserveStateBreakfast == true && motor3FeedState == false)
  {
    motor3Feed(motor3CountBreakfast);
    motor3FeedState = true;

    Serial.println("motor3 is running now");
    Serial.print("motor3 ResetTime is ");
    Serial.println(motor3ResetTime);
  }

  if ((now.hour() == motorLunchHour && now.minute() == motorLunchMinute))
  {
    motor3ReserveStateLunch = true;
    motor3ResetTime = now.minute() + 2;

    if (motor3ResetTime >= 60)
      motor3ResetTime -= 60;
  }

  if (motor3ReserveStateLunch == true && motor3FeedState == false)
  {
    motor3Feed(motor3CountLunch);
    motor3FeedState = true;

    Serial.println("motor3 is running now");
    Serial.print("motor3 ResetTime is ");
    Serial.println(motor3ResetTime);
  }

  if ((now.hour() == motorDinnerHour && now.minute() == motorDinnerMinute))
  {
    motor3ReserveStateDinner = true;
    motor3ResetTime = now.minute() + 2;

    if (motor3ResetTime >= 60)
      motor3ResetTime -= 60;
  }

  if (motor3ReserveStateDinner == true && motor3FeedState == false)
  {
    motor3Feed(motor3CountDinner);
    motor3FeedState = true;

    Serial.println("motor3 is running now");
    Serial.print("motor3 ResetTime is ");
    Serial.println(motor3ResetTime);
  }

  if (now.minute() == motor3ResetTime)
  {
    if (motor3FeedState)
      Serial.println("motor3 Reset!");

    motor3ReserveStateBreakfast = false;
    motor3ReserveStateLunch = false;
    motor3ReserveStateDinner = false;
    motor3FeedState = false;
  }
}

void bluetoothRecieve()
{
  String buffer = Serial1.readStringUntil('#');

  int firstSeparator = buffer.indexOf("/", 0);
  int secondSeparator = buffer.indexOf("/", (firstSeparator + 1));
  int lengthOfBuffer = buffer.length();

  int modeBuffer = buffer.substring(0, firstSeparator).toInt();

  switch (modeBuffer)
  {
  case 1:
    motor1GramBreakfast = buffer.substring(firstSeparator + 1, secondSeparator).toInt();

    Serial.print("motor1GramBreakfast - ");
    Serial.println(motor1GramBreakfast);
    break;

  case 2:
    motor1GramLunch = buffer.substring(firstSeparator + 1, secondSeparator).toInt();

    Serial.print("motor1GramLunch - ");
    Serial.println(motor1GramLunch);
    break;

  case 3:
    motor1GramDinner = buffer.substring(firstSeparator + 1, secondSeparator).toInt();

    Serial.print("motor1GramDinner - ");
    Serial.println(motor1GramDinner);
    break;

  case 4:
    motor2CountBreakfast = buffer.substring(firstSeparator + 1, secondSeparator).toInt();

    Serial.print("motor2CountBreakfast - ");
    Serial.println(motor2CountBreakfast);
    break;

  case 5:
    motor2CountLunch = buffer.substring(firstSeparator + 1, secondSeparator).toInt();

    Serial.print("motor2CountLunch - ");
    Serial.println(motor2CountLunch);
    break;

  case 6:
    motor2CountDinner = buffer.substring(firstSeparator + 1, secondSeparator).toInt();

    Serial.print("motor2CountDinner - ");
    Serial.println(motor2CountDinner);
    break;

  case 7:
    motor3CountBreakfast = buffer.substring(firstSeparator + 1, secondSeparator).toInt();

    Serial.print("motor3CountBreakfast - ");
    Serial.println(motor3CountBreakfast);
    break;

  case 8:
    motor3CountLunch = buffer.substring(firstSeparator + 1, secondSeparator).toInt();

    Serial.print("motor3CountLunch - ");
    Serial.println(motor3CountLunch);
    break;

  case 9:
    motor3CountDinner = buffer.substring(firstSeparator + 1, secondSeparator).toInt();

    Serial.print("motor3CountDinner - ");
    Serial.println(motor3CountDinner);
    break;

  case 11:
    motorBreakfastHour = buffer.substring(firstSeparator + 1, secondSeparator).toInt();
    motorBreakfastMinute = buffer.substring(secondSeparator + 1, lengthOfBuffer).toInt();

    Serial.print("motorBreakfast - ");
    Serial.print(motorBreakfastHour);
    Serial.print(":");
    Serial.print(motorBreakfastMinute);
    Serial.print(":");
    Serial.println("00");
    break;

  case 12:
    motorLunchHour = buffer.substring(firstSeparator + 1, secondSeparator).toInt();
    motorLunchMinute = buffer.substring(secondSeparator + 1, lengthOfBuffer).toInt();

    Serial.print("motorLunch - ");
    Serial.print(motorLunchHour);
    Serial.print(":");
    Serial.print(motorLunchMinute);
    Serial.print(":");
    Serial.println("00");
    break;

  case 13:
    motorDinnerHour = buffer.substring(firstSeparator + 1, secondSeparator).toInt();
    motorDinnerMinute = buffer.substring(secondSeparator + 1, lengthOfBuffer).toInt();

    Serial.print("motorDinner - ");
    Serial.print(motorDinnerHour);
    Serial.print(":");
    Serial.print(motorDinnerMinute);
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