#include <Servo.h>
#include <RTClib.h> // RTCLib_by_NeiroN

const int servo1Pin = 8;
const int servo2Pin = 9;
const int servo3Pin = 10;
const int buzzerPin = 12;

DS1302 rtc;
char buf[20];

Servo servo1;
Servo servo2;
Servo servo3;

const int angle225Delay = 3000;
const int angle450Delay = 4500;
const int stopDelay = 1000;
const int servoRun = 80;
const int servoStop = 90;
class Hx711
{
public:
  Hx711(uint8_t pin_din, uint8_t pin_slk);
  virtual ~Hx711();
  long value();
  long nomalvalue(byte times = 8);
  void setOffset(long offset);
  void setScale(float scale = 742.f);
  float gram();

private:
  const uint8_t DOUT;
  const uint8_t SCK;
  long _offset;
  float _scale;
};

Hx711 scale(A1, A0);

Hx711::Hx711(uint8_t pin_dout, uint8_t pin_slk)
    : DOUT(pin_dout), SCK(pin_slk)
{
  pinMode(SCK, OUTPUT);
  pinMode(DOUT, INPUT);

  digitalWrite(SCK, HIGH);
  delayMicroseconds(100);
  digitalWrite(SCK, LOW);

  nomalvalue();
  this->setOffset(nomalvalue());
  this->setScale();
}

Hx711::~Hx711()
{
}

long Hx711::nomalvalue(byte times)
{
  long sum = 0;
  for (byte i = 0; i < times; i++)
  {
    sum += value();
  }

  return sum / times;
}

long Hx711::value()
{
  byte data[3];

  while (digitalRead(DOUT))
    ;

  for (byte j = 3; j--;)
  {
    for (char i = 8; i--;)
    {
      digitalWrite(SCK, HIGH);
      bitWrite(data[j], i, digitalRead(DOUT));
      digitalWrite(SCK, LOW);
    }
  }

  digitalWrite(SCK, HIGH);
  digitalWrite(SCK, LOW);

  data[2] ^= 0x80;

  return ((uint32_t)data[2] << 16) | ((uint32_t)data[1] << 8) | (uint32_t)data[0];
}

void Hx711::setOffset(long offset)
{
  _offset = offset;
}

void Hx711::setScale(float scale)
{
  _scale = scale;
}

float Hx711::gram()
{
  long val = (nomalvalue() - _offset);
  return (float)val / _scale;
}

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

void servo1Run(int gram)
{
  servo1.attach(servo1Pin);
  if ((scale.gram() * 2) < gram)
  {
    servo1.write(servoRun);
    delay(angle225Delay);
    servo1.write(servoStop);
    delay(stopDelay);
  }
  servo1.detach();
}

void servo2Run(int count = 1)
{
  servo2.attach(servo2Pin);
  for (int i = 0; i < count; i++)
  {
    servo2.write(servoRun);
    delay(angle225Delay);
    servo2.write(servoStop);
    delay(stopDelay);
  }
  servo2.detach();
}

void servo3Run(int count = 1)
{
  servo3.attach(servo3Pin);
  for (int i = 0; i < count; i++)
  {
    servo3.write(servoRun);
    delay(angle450Delay);
    servo3.write(servoStop);
    delay(stopDelay);
  }
  servo3.detach();
}

void bluetoothCommunication()
{
  if (Serial1.available())
  {
    Serial.write(Serial1.read());
  }
  if (Serial.available())
  {
    Serial1.write(Serial.read());
  }
}

void buzzerAlarm(int num = 1)
{
  for(int i = 0; i < num; i++)
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
  rtcCalibration();
}

void loop()
{
  // buzzerAlarm(2);
  debug();
  delay(100);
}