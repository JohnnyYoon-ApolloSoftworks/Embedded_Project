//디버깅 모드 20씩 빼세요
const int servo1Pin = 28; 
const int servo2Pin = 29;
const int servo3Pin = 30;
const int buzzerPin = 32;

char buf[20];

bool servo1ReserveState = false;
bool servo1RunState = false;
bool servo2ReserveState = false;
bool servo2RunState = false;
bool servo3ReserveState = false;
bool servo3RunState = false;

int servo1ResetTime = 255;
int servo2ResetTime = 255;
int servo3ResetTime = 255;

const int angle225Delay = 3000;
const int angle450Delay = 4500;
const int stopDelay = 1000;
const int servoRun = 80;
const int servoStop = 90;

int servo1Gram = 100;

int servo1BreakfastHour = 255;
int servo1BreakfastMinute = 255;
int servo1LunchHour = 255;
int servo1LunchMinute = 255;
int servo1DinnerHour = 255;
int servo1DinnerMinute = 255;

int servo2Count = 1;

int servo2BreakfastHour = 255;
int servo2BreakfastMinute = 255;
int servo2LunchHour = 255;
int servo2LunchMinute = 255;
int servo2DinnerHour = 255;
int servo2DinnerMinute = 255;

int servo3Count = 1;

int servo3BreakfastHour = 255;
int servo3BreakfastMinute = 255;
int servo3LunchHour = 255;
int servo3LunchMinute = 255;
int servo3DinnerHour = 255;
int servo3DinnerMinute = 255;
