const int stepsPerRevolution = 64; // 11.25도

const int motor1In1Pin = 30; 
const int motor1In2Pin = 32; 
const int motor1In3Pin = 34; 
const int motor1In4Pin = 36; 

const int motor2In1Pin = 38; 
const int motor2In2Pin = 40; 
const int motor2In3Pin = 42; 
const int motor2In4Pin = 44; 

const int motor3In1Pin = 46; 
const int motor3In2Pin = 48; 
const int motor3In3Pin = 50; 
const int motor3In4Pin = 52; 

const int buzzerPin = 8;

char buf[20];

bool motor1ReserveState = false;
bool motor1FeedState = false;
bool motor2ReserveState = false;
bool motor2FeedState = false;
bool motor3ReserveState = false;
bool motor3FeedState = false;

int motor1ResetTime = 255;
int motor2ResetTime = 255;
int motor3ResetTime = 255;

const int angleDelay = 55;
const int stopDelay = 500;
const int motorRun = 80;
const int motorStop = 90;

int motor1Gram = 100;

int motor1BreakfastHour = 255;
int motor1BreakfastMinute = 255;
int motor1LunchHour = 255;
int motor1LunchMinute = 255;
int motor1DinnerHour = 255;
int motor1DinnerMinute = 255;

int motor2Count = 1;

int motor2BreakfastHour = 255;
int motor2BreakfastMinute = 255;
int motor2LunchHour = 255;
int motor2LunchMinute = 255;
int motor2DinnerHour = 255;
int motor2DinnerMinute = 255;

int motor3Count = 1;

int motor3BreakfastHour = 255;
int motor3BreakfastMinute = 255;
int motor3LunchHour = 255;
int motor3LunchMinute = 255;
int motor3DinnerHour = 255;
int motor3DinnerMinute = 255;
