#include <RTClib_Johnny.h>
#include <Stepper.h>
#include "HX711.h"
#include "settings.h"


Hx711 scale(A1, A0); // HX711 객체 생성 (전달 인자로 핀 번호를 입력)
DS1302 rtc; // RTC 객체 생성

// 스텝모터 객체 생성
// IN4, IN2, IN3, IN1
Stepper stepMotor1(stepsPerRevolution, motor1In4Pin, motor1In2Pin, motor1In3Pin, motor1In1Pin);
Stepper stepMotor2(stepsPerRevolution, motor2In4Pin, motor2In2Pin, motor2In3Pin, motor2In1Pin);
Stepper stepMotor3(stepsPerRevolution, motor3In4Pin, motor3In2Pin, motor3In3Pin, motor3In1Pin);

// RTC 초기화 및 시간 재설정을 위한 함수
void rtcCalibration()
{
  rtc.adjust(DateTime(__DATE__, __TIME__));
}

// 디버깅을 위해 시간 및 무게 데이터 로그를 출력하는 함수
void debugLog()
{
  DateTime now = rtc.now();
  Serial.print(now.tostr(buf));
  Serial.print(" | wheight : ");
  Serial.print(scale.gram() * 2, 1);
  Serial.println(" g");
}

// 디버깅을 위해 무게 데이터를 출력하는 함수
void showWeight()
{
  Serial.print(scale.gram() * 2, 1);
  Serial.println(" g");
}

// 디버깅을 위해 시간 데이터를 출력하는 함수
  void showDate()
{
  DateTime now = rtc.now();
  Serial.println(now.tostr(buf));
}

// 부저 동작 함수
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

// 첫번째 스텝모터 동작 함수
void motor1Feed(int gram)
{
  while ((scale.gram() * 2) < gram)
  {
    if ((scale.gram() * 2) >= gram) // 설정된 무게보다 측정된 무게가 크거나 같으면 while문 탈출
      break;

    stepMotor1.step(stepsPerRevolution * 4);  // 45도 동작을 해야하기 때문에 11.25도 * 4를 함
    delay(100);
  }
  buzzerAlarm(1); // 동작 후 부저 한번 울림
}

// 두번째 스텝모터 동작 함수
void motor2Feed(int count)
{
  for (int i = 0; i < count; i++)
  {
    stepMotor2.step(stepsPerRevolution * 2);  // 22.5도 동작을 해야하기 때문에 11.25도 * 2를 함
    delay(100);
  }
  buzzerAlarm(2); // 동작 후 부저 두번 울림
}

// 세번째 스텝모터 동작 함수
void motor3Feed(int count)
{
  for (int i = 0; i < count; i++)
  {
    stepMotor3.step(stepsPerRevolution * 4);
    delay(100);
  }
  buzzerAlarm(3);
}

// 블루투스로 입력받은 데이터를 기반으로 모터 동작을 결정해주는 함수
void motor1Operating()
{
  DateTime now = rtc.now();

  if ((now.hour() == motorBreakfastHour && now.minute() == motorBreakfastMinute))
  {
    motor1ReserveStateBreakfast = true;
    motor1ResetTime = now.minute() + 2;

    if (motor1ResetTime >= 60)  // 모터 초기화 시간 저장을 위해 현재 시간에 2분을 더할 때 62분이 아닌 2분으로 처리하기위해 60이 넘으면 60을 빼줌
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

// 블루투스로 입력받은 데이터를 기반으로 모터 동작을 결정해주는 함수
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

// 블루투스로 입력받은 데이터를 기반으로 모터 동작을 결정해주는 함수
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

// 블루투스 통신으로부터 데이터를 입력받아 처리 후 저장 하는 함수
void bluetoothRecieve()
{
  /* 블루투스 통신 수신 데이터 형식 "1/13/00#"
   * / -> 구분자 : 데이터간의 분할 지원
   * 첫번째 구분자 이전 -> 전달될 데이터 형식
   * 1~9 -> 각각 모터의 시간 설정 데이터
   * 11~13 -> 각각 모터의 급식 양 조절
   * 두번째 구분자 이전 -> 시간 설정 및 양
   * 두번째 구분자 이후 -> 분 설정
   */
  String buffer = Serial1.readStringUntil('#'); //블루투스 통신에서 수신된 문자열 끝에 포함된 #까지 데이터를 읽어들임

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

    Serial.print("motorDinner - ");    Serial.print(motorDinnerHour);
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

  rtcCalibration(); // RTC 캘리브레이션 (시간 재설정)
}

void loop()
{
  if (Serial1.available())
    bluetoothRecieve();

  motor1Operating();
  delay(3000);
  motor2Operating();
  delay(3000);
  motor3Operating();
  delay(3000);
  debugLog();
  delay(1000);
}