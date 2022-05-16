#include <LiquidCrystal_I2C.h> //LCD 라이브러리
#include <RTClib_Johnny.h>
#include <Servo.h>
#include <Wire.h>
#include <EEPROM.h>
#include "SparkFun_Qwiic_Scale_NAU7802_Arduino_Library.h"

NAU7802 myScale;

#define LOCATION_CALIBRATION_FACTOR 0 // Float, requires 4 bytes of EEPROM
#define LOCATION_ZERO_OFFSET 10

bool settingsDetected = false; // Used to prompt user to calibrate their scale

// Create an array to take average of weights. This helps smooth out jitter.
#define AVG_SIZE 4
float avgWeights[AVG_SIZE];

byte avgWeightSpot = 0;

Servo servo; //서보모터 이름 servo로 설정

DS1302 rtc(5, 7, 6);                // RST, CLK, DAT
LiquidCrystal_I2C lcd(0x27, 16, 2); // lcd 주소값 설정

float currentWeight = 0;


int CDSPin = A0; // cds A0핀

int trigPin = 3; //초음파센서 핀 3,4 번
int echoPin = 4;
int buzzerPin = 11; //부저 11번
int waterSensorPin = 12;

int tones[] = {261, 523}; //부저 음계 도,시 설정 4옥타브Hz

float duration;
float distance;

bool operatingState = false;
bool resetState = false;
bool closeState = false;
bool waterState = false;

char buf[20];

int pos = 0;

void setup()
{
  Serial.begin(9600);
  rtc.begin();
  servo.attach(10); //서보모터 10번 핀
  lcd.init();       // lcd초기화
  lcd.backlight();  // lcd 바탕을 밝게하기
  Wire.begin();
  Wire.setClock(400000); // Qwiic Scale is capable of running at 400kHz if desired

  if (myScale.begin() == false)
  {
    Serial.println("Scale not detected. Please check wiring. Freezing...");
    while (1)
      ;
  }
  Serial.println("Scale detected!");

  readSystemSettings(); // Load zeroOffset and calibrationFactor from EEPROM

  myScale.setSampleRate(NAU7802_SPS_320); // Increase to max sample rate
  myScale.calibrateAFE();                 // Re-cal analog front end when we change gain, sample rate, or channel

  Serial.print("Zero offset: ");
  Serial.println(myScale.getZeroOffset());
  Serial.print("Calibration factor: ");
  Serial.println(myScale.getCalibrationFactor());

  if (rtc.isrunning())
  {
    Serial.println("RTC is running!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(__DATE__, __TIME__));
  }

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(waterSensorPin, INPUT);

  spray();
  BUZZER(1);
}

void loop()
{
  readLoadcell();
  readCds();
  readRtc();
  readWaterSensor();

  if(closeState == true){
    operating();
    readDistance();
  }
  delay(500);
}

//-----------------------------------문 개폐여부 판단 코드-----------------------------------

void readCds()
{
  int cds = analogRead(CDSPin);

  if (cds < 600) //뚜껑 열림
  {
    lcd.setCursor(0, 1);
    lcd.print("OPEN ");

    closeState = false;
    Serial.println("Door : Open");
  }
  else if (cds >= 600) //뚜껑 닫음
  {
    lcd.setCursor(0, 1);
    lcd.print("CLOSE");

    closeState = true;
    Serial.println("Door : Close");
  }
}

//-----------------------------------쓰레기 양 판단 코드-----------------------------------

void readDistance()
{
  digitalWrite(trigPin, HIGH);
  delay(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);                   // pulseIn함수 (핀, 상태) 핀상태가 바뀌면 그때까지 경과된 시간을 ms단위로 바꿔줌.
  distance = ((float)(340 * duration) / 10000) / 2; //계산한 거리를 다시 cm로 바꿔줌
  
  if (distance <= 4)
  {
    lcd.setCursor(12, 1);
    lcd.print("FULL");

    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
  }
  else if (distance > 4 && distance <= 8)
  {
    lcd.setCursor(12, 1);
    lcd.print("MID ");

    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
  }
  else if (distance > 8)
  {
    lcd.setCursor(12, 1);
    lcd.print("LESS");

    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
  }
}

//-----------------------------------계절 판단 코드----------------------------------

void readRtc()
{
  DateTime now = rtc.now();
  Serial.println(now.tostr(buf));

  if (now.month() == 3 || now.month() == 4 || now.month() == 5)
  {
    lcd.setCursor(10, 0);
    lcd.print("SPRING");

    Serial.println("SPRING");
  }
  else if (now.month() == 6 || now.month() == 7 || now.month() == 8)
  {
    lcd.setCursor(10, 0);
    lcd.print("SUMMER");
    
    Serial.println("SUMMER");
  }
  else if (now.month() == 9 || now.month() == 10 || now.month() == 11)
  {
    lcd.setCursor(10, 0);
    lcd.print("AUTUMN");

    Serial.println("AUTUMN");
  }
  else if (now.month() == 12 || now.month() == 1 || now.month() == 2)
  {
    lcd.setCursor(10, 0);
    lcd.print("WINTER");

    Serial.println("WINTER");
  }
}

//-----------------------------------부저 동작 코드-----------------------------------

void BUZZER(int num) //몇 번 반복할지 설정
{                    //설정 한 수 많큼 알람 울림
  for (int i = 0; i < num; i++)
  {
    for (int j = 0; j < 2; j++)
    {
      tone(buzzerPin, tones[j]);
      delay(100);
      noTone(buzzerPin);
    }
  }
}

//-----------------------------------물 분사 동작 코드----------------------------------

void spray()
{
    for (pos = 0; pos <= 180; pos += 1)
    {
      servo.write(pos); // tell servo to go to position in variable 'pos'
      delay(15);        // waits 15 ms for the servo to reach the position
    }
    for (pos = 180; pos >= 0; pos -= 1)
    {
      servo.write(pos);
      delay(15);
    }
}

//-----------------------------------메인 동작 코드-----------------------------------

void operating()
{
  DateTime now = rtc.now();
  
  int minute = now.minute();
  if (minute % 5 == 0)
    operatingState = true;
  else
  {
    operatingState = false;
    resetState = false;
  }
  if (currentWeight >= 20 && operatingState == true && resetState == false) // 20g 이상 , 30분 이면 모터 동작
  {
    if (waterState == true)
    {
      spray();
      BUZZER(1);
    }
    else
    {
      BUZZER(3);
    }
    operatingState = false;
    resetState = true;
  }
}

//-----------------------------------물 부족 감지-----------------------------------

void readWaterSensor()
{
  int water = digitalRead(waterSensorPin);

  if (water == HIGH)
    waterState = true;
  else
    waterState = false;
}

//-----------------------------------로드셀 앰프 관련 함수-----------------------------------

void readLoadcell()
{
  if (myScale.available() == true)
  {
    long currentReading = myScale.getReading();
    currentWeight = myScale.getWeight();

    Serial.print("Reading: ");
    Serial.print(currentReading);
    Serial.print("\tWeight: ");
    Serial.print(currentWeight, 2); // Print 2 decimal places

    avgWeights[avgWeightSpot++] = currentWeight;
    if (avgWeightSpot == AVG_SIZE)
      avgWeightSpot = 0;

    float avgWeight = 0;
    for (int x = 0; x < AVG_SIZE; x++)
      avgWeight += avgWeights[x];
    avgWeight /= AVG_SIZE;

    Serial.print("\tAvgWeight: ");
    Serial.print(avgWeight, 2); // Print 2 decimal places

    if (settingsDetected == false)
    {
      Serial.print("\tScale not calibrated. Press 'c'.");
    }

    Serial.println();
  }

  if (Serial.available())
  {
    byte incoming = Serial.read();

    if (incoming == 't') // Tare the scale
      myScale.calculateZeroOffset();
    else if (incoming == 'c') // Calibrate
    {
      calibrateScale();
    }
  }
}

void calibrateScale(void)
{
  Serial.println();
  Serial.println();
  Serial.println(F("Scale calibration"));

  Serial.println(F("Setup scale with no weight on it. Press a key when ready."));
  while (Serial.available())
    Serial.read(); // Clear anything in RX buffer
  while (Serial.available() == 0)
    delay(10); // Wait for user to press key

  myScale.calculateZeroOffset(64); // Zero or Tare the scale. Average over 64 readings.
  Serial.print(F("New zero offset: "));
  Serial.println(myScale.getZeroOffset());

  Serial.println(F("Place known weight on scale. Press a key when weight is in place and stable."));
  while (Serial.available())
    Serial.read(); // Clear anything in RX buffer
  while (Serial.available() == 0)
    delay(10); // Wait for user to press key

  Serial.print(F("Please enter the weight, without units, currently sitting on the scale (for example '4.25'): "));
  while (Serial.available())
    Serial.read(); // Clear anything in RX buffer
  while (Serial.available() == 0)
    delay(10); // Wait for user to press key

  // Read user input
  float weightOnScale = Serial.parseFloat();
  Serial.println();

  myScale.calculateCalibrationFactor(weightOnScale, 64); // Tell the library how much weight is currently on it
  Serial.print(F("New cal factor: "));
  Serial.println(myScale.getCalibrationFactor(), 2);

  Serial.print(F("New Scale Reading: "));
  Serial.println(myScale.getWeight(), 2);

  recordSystemSettings(); // Commit these values to EEPROM
}

// Record the current system settings to EEPROM
void recordSystemSettings(void)
{
  // Get various values from the library and commit them to NVM
  EEPROM.put(LOCATION_CALIBRATION_FACTOR, myScale.getCalibrationFactor());
  EEPROM.put(LOCATION_ZERO_OFFSET, myScale.getZeroOffset());
}

// Reads the current system settings from EEPROM
// If anything looks weird, reset setting to default value
void readSystemSettings(void)
{
  float settingCalibrationFactor; // Value used to convert the load cell reading to lbs or kg
  long settingZeroOffset;         // Zero value that is found when scale is tared

  // Look up the calibration factor
  EEPROM.get(LOCATION_CALIBRATION_FACTOR, settingCalibrationFactor);
  if (settingCalibrationFactor == 0xFFFFFFFF)
  {
    settingCalibrationFactor = 0; // Default to 0
    EEPROM.put(LOCATION_CALIBRATION_FACTOR, settingCalibrationFactor);
  }

  // Look up the zero tare point
  EEPROM.get(LOCATION_ZERO_OFFSET, settingZeroOffset);
  if (settingZeroOffset == 0xFFFFFFFF)
  {
    settingZeroOffset = 1000L; // Default to 1000 so we don't get inf
    EEPROM.put(LOCATION_ZERO_OFFSET, settingZeroOffset);
  }

  // Pass these values to the library
  myScale.setCalibrationFactor(settingCalibrationFactor);
  myScale.setZeroOffset(settingZeroOffset);

  settingsDetected = true; // Assume for the moment that there are good cal values
  if (settingCalibrationFactor < 0.1 || settingZeroOffset == 1000)
    settingsDetected = false; // Defaults detected. Prompt user to cal scale.
}
