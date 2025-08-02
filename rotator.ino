#include <Servo.h>

#define SERVO_OUT 6
#define HIGH_SPEED_IN 11
#define LOW_SPEED_IN 9

Servo servo;

int MIN_SERVO = 0;
int MAX_SERVO = 180;

enum SwitchState {
  HIGH_SPEED,
  LOW_SPEED,
  OFF
};

SwitchState currentSwitchState = OFF;

void setup() {
  pinMode(HIGH_SPEED_IN, INPUT_PULLUP);
  pinMode(LOW_SPEED_IN, INPUT_PULLUP);
  pinMode(SERVO_OUT, OUTPUT);
  servo.attach(SERVO_OUT);

  Serial.begin(9600);
}

bool switchState(unsigned int switchPin) {
  return !digitalRead(switchPin);
}

SwitchState getCurrentSwitchState() {
  if (switchState(HIGH_SPEED_IN)) {
    return HIGH_SPEED;
  }
  if (switchState(LOW_SPEED_IN)) {
    return LOW_SPEED;
  }
  return OFF;
}

int servoAngle() {
  return servo.read();
}

const unsigned int FAST_SERVO_DELAY = 50;
const unsigned int SLOW_SERVO_DELAY = 100;

void loop() {

  while (getCurrentSwitchState() == LOW_SPEED) {
    Serial.println("LOW SPEED");
    currentSwitchState = LOW_SPEED;
    moveMotor(SLOW_SERVO_DELAY);
  }

  while (getCurrentSwitchState() == HIGH_SPEED) {
    Serial.println("HIGH SPEED");
    currentSwitchState = HIGH_SPEED;
    moveMotor(FAST_SERVO_DELAY);
  }
}

void moveMotor(unsigned int servoDelay) {
  if (servoAngle() == MIN_SERVO) {
    rotateClockwise(servoDelay);
  } else {
    rotateAntiClockwise(servoDelay);
  }
}

void rotateAntiClockwise(unsigned int servoDelay) {
  const unsigned int initialPosition = servoAngle();
  for (int position = initialPosition; position >= MIN_SERVO; position--) {
    if (currentSwitchState != getCurrentSwitchState()) {
      break;
    }
    servo.write(position);
    delay(servoDelay);
  }
}

void rotateClockwise(unsigned int servoDelay) {
  const unsigned int initialPosition = servoAngle();
  for (int position = initialPosition; position <= MAX_SERVO; position++) {
    if (currentSwitchState != getCurrentSwitchState()) {
      break;
    }
    servo.write(position);
    delay(servoDelay);
  }
}
