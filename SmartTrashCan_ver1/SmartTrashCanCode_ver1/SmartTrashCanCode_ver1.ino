#include <Servo.h> // 서보 라이브러리 사용
#include <SoftwareSerial.h>

Servo servo; // 서보 객체 선언

SoftwareSerial btSerial(2, 3); // 블루투스 소프트웨어 시리얼 객체 선언

const int servoPin = 4;    // 서보모터 핀
const int irLedPin = 7;    // 적외선 센서 핀
const int trigPin = 8;     // 초음파센서의 trig핀을 아두이노 8번 핀 연결
const int echoPin = 9;     // 초음파센서의 echo핀을 아두이노 9번 핀 연결
const int blueLedPin = 12; // 파란색 LED 핀
const int redLedPin = 13;  // 빨간색 LED 핀

int fullState = 0;
int count = -1;
int delayClose = 5;

void setup()
{
    Serial.begin(9600);   // 시리얼 통신 시작
    btSerial.begin(9600); // 블루투스 소프트웨어 시리얼 시작

    pinMode(irLedPin, INPUT);    // 7번핀 INPUT 설정
    pinMode(trigPin, OUTPUT);    // 8번핀 OUTPUT 설정
    pinMode(echoPin, INPUT);     // 9번핀 INPUT 설정
    pinMode(redLedPin, OUTPUT);  // 12번핀 OUTPUT 설정
    pinMode(blueLedPin, OUTPUT); // 13번핀 OUTPUT 설정

    servo.attach(servoPin); //아두이노의 3번 핀에서 서보모터 동
}

void loop()
{
    // 초음파 출력
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // 외부 표면에 반사된 초음파 왕복 시간을 계산해 거리를 산출함
    int distance = pulseIn(echoPin, HIGH) * 340 / 10000 / 2;

    // 시리얼 모니터로 거리 출력
    Serial.print(distance);
    Serial.println(" cm");

    if (distance < 10)
    {
        count = delayClose * 2;
    }
    else if (distance >= 10)
    {
        if (count >= 0)
            count--;
    }

    if (count >= 0)
    {
        servo.write(170);
    }
    else
    {
        servo.write(0);
    }

    // 이전 값을 저장
    bool preFullState = fullState;
    
    // 적외선 센서 값 읽어오기
    fullState = digitalRead(irLedPin);

    // 깂이 바뀌면 실행
    if (preFullState != fullState)
    {
        if (fullState == HIGH)
        {
            //장애물 없는 상태
            btSerial.println("1");
        }
        else if (fullState == LOW)
        {
            //장애물 있는 상태
            btSerial.println("2");
        }
    }

    //장애물 유뮤에 따른 LED 제어
    if (fullState == HIGH)
    {
        //장애물 없는 상태
        digitalWrite(redLedPin, LOW);
        digitalWrite(blueLedPin, HIGH);
    }
    else if (fullState == LOW)
    {
        //장애물 있는 상태
        digitalWrite(redLedPin, HIGH);
        digitalWrite(blueLedPin, LOW);

    }

    delay(500);
}
