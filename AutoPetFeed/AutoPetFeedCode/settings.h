// 스텝모터 64스텝 = 11.25도
const int stepsPerRevolution = 64;

// 첫번째 스텝모터 핀 번호
const int motor1In1Pin = 30; 
const int motor1In2Pin = 32; 
const int motor1In3Pin = 34; 
const int motor1In4Pin = 36; 

// 두번째 스텝모터 핀 번호
const int motor2In1Pin = 38; 
const int motor2In2Pin = 40; 
const int motor2In3Pin = 42; 
const int motor2In4Pin = 44; 

// 세번째 스텝모터 핀 번호
const int motor3In1Pin = 46; 
const int motor3In2Pin = 48; 
const int motor3In3Pin = 50; 
const int motor3In4Pin = 52; 

// 부저 핀 번호
const int buzzerPin = 7;

char buf[20]; // RTC 데이터 출력 버퍼

// 모터 동작 체크를 위한 체크
bool motor1ReserveStateBreakfast = false;
bool motor1ReserveStateLunch = false;
bool motor1ReserveStateDinner = false;
bool motor1FeedState = false;
bool motor2ReserveStateBreakfast = false;
bool motor2ReserveStateLunch = false;
bool motor2ReserveStateDinner = false;
bool motor2FeedState = false;
bool motor3ReserveStateBreakfast = false;
bool motor3ReserveStateLunch = false;
bool motor3ReserveStateDinner = false;
bool motor3FeedState = false;

// 모터 상태 초기화 시간 저장을 위한 변수
int motor1ResetTime = 255;
int motor2ResetTime = 255;
int motor3ResetTime = 255;

// 모터 동작 시간 저장을 위한 변수
int motorBreakfastHour = 255;
int motorBreakfastMinute = 255;
int motorLunchHour = 255;
int motorLunchMinute = 255;
int motorDinnerHour = 255;
int motorDinnerMinute = 255;

// 첫번째 스텝모터 양 조절
int motor1GramBreakfast = -100;
int motor1GramLunch = -100;
int motor1GramDinner = -100;

// 두번째 스텝모터 양 조절
int motor2CountBreakfast = 0;
int motor2CountLunch = 0;
int motor2CountDinner = 0;

// 세번째 스텝모터 양 조절
int motor3CountBreakfast = 0;
int motor3CountLunch = 0;
int motor3CountDinner = 0;
