#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"
#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT; //  블루투스 객체 생성
MAX30105 particleSensor;  // 심박센서 객체 생성

int VibratorMotorPin = 16; // 진동모터 핀 지정
int MinWarningBPM = 45;    // 최저 위험 심박수 지정
int MaxWarningBPM = 135;   // 최고 위험 심박수 지정

const byte RATE_SIZE = 4; //평균값 산출을 위한 표본 갯수
byte rates[RATE_SIZE];    //심박수 배열
byte rateSpot = 0;
long lastBeat = 0;

float beatsPerMinute;
int beatAvg;

unsigned long previousMillis = 0;

int ValidCheck = 0;

void setup()
{
  Serial.begin(115200);          // 시리얼 모니터 설정
  SerialBT.begin("HeartBeat32"); // 블루투스 장치 이름 설정

  // 심박센서 연결 확인
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST))
  {
    Serial.println("MAX30102 was not found. Please check wiring/power. ");
    while (1)
      ;
  }

  // 심박센서 기본 설정
  particleSensor.setup();
  particleSensor.setPulseAmplitudeRed(0x0A);
  particleSensor.setPulseAmplitudeGreen(0);

  pinMode(VibratorMotorPin, OUTPUT); // 진동모터 핀을 출력으로 설정
}

void loop()
{
  unsigned long currentMillis = millis();

  long irValue = particleSensor.getIR();

  if (checkForBeat(irValue) == true)
  {
    long delta = millis() - lastBeat;
    lastBeat = millis();

    beatsPerMinute = 60 / (delta / 1000.0);

    if (beatsPerMinute < 255 && beatsPerMinute > 20)
    {
      rates[rateSpot++] = (byte)beatsPerMinute;
      rateSpot %= RATE_SIZE;

      beatAvg = 0;
      for (byte x = 0; x < RATE_SIZE; x++)
        beatAvg += rates[x];
      beatAvg /= RATE_SIZE;
    }
  }

  if (currentMillis - previousMillis >= 1000)
  {
    previousMillis = currentMillis;

    // Serial.print("IR=");
    // Serial.print(irValue);
    Serial.print("BPM=");
    Serial.print(beatsPerMinute);
    Serial.print(", Avg BPM=");
    Serial.print(beatAvg);

    if (irValue < 50000)
    {
      Serial.print(" No finger Detected");
      ValidCheck = 0;
    }
    else
    {
      ValidCheck++;
    }

    Serial.println();

    SerialBT.println(beatAvg); // 블루투스로 심박수 전송

    if (ValidCheck > 10)
    {
      //평균 심박수와 설정된 위험 심박수를 비교 및 동작하는 코드
      if (beatAvg >= MaxWarningBPM || beatAvg <= MinWarningBPM)
      {
        digitalWrite(VibratorMotorPin, HIGH);
      }
      else
      {
        digitalWrite(VibratorMotorPin, LOW);
      }
    }
  }
}