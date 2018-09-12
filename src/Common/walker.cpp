#include "walker.h"
#include "common.h"

Walker::Walker():
  leftWheel(left_motor), rightWheel(right_motor) ,
  steering(leftWheel, rightWheel){
    reset();
}

void Walker::init() {

}

void Walker::terminate() {
  leftWheel.stop();
  rightWheel.stop();
}

void Walker::reset() {
  leftWheel.reset();
  rightWheel.reset();
}

void Walker::stop() {
  run(0, 0);

  reset();
}

void Walker::run(int8_t pwm, int8_t turn) {
  setBrakeMotor(false);
  leftWheel.setPWM(pwm - turn);
  rightWheel.setPWM(pwm + turn);
  
}


void Walker::runSteering(int power, int turnRatio) {
    steering.setPower(power, turnRatio);
}

void Walker::rotationRight(int8_t pwm) {
  rightWheel.setBrake(false);
  leftWheel.setPWM(pwm);
}

void Walker::rotationLeft(int8_t pwm) {
  leftWheel.setBrake(false);
  rightWheel.setPWM(pwm);
}

void Walker::turnRight(int32_t angle) {
  int32_t oldAngle = getCountL();
  int32_t direction = 0;
  char msg[32];

  while(1) {
    rotationRight(60);

    sprintf(msg, "getCountL():%d", (int)getCountL());
    msg_f(msg, 4);

    sprintf(msg, "oldAngle:%d", (int)oldAngle);
    msg_f(msg, 5);

    direction = ((TIRE_DIAMETER / 2) * (getCountL() - oldAngle)) / (TREAD);
    
    sprintf(msg, "direction:%d", (int)direction);
    msg_f(msg, 2);

    sprintf(msg, "anglen:%d", (int)angle);
    msg_f(msg, 1);

    if (direction > angle ) {
      break;
    }

    sprintf(msg, "direction2:%d", (int)direction);
    msg_f(msg, 3);

    clock.sleep(4);
  }

  stop();
}

void Walker::setBrakeMotor(bool brake) {
  leftWheel.setBrake(brake);
  rightWheel.setBrake(brake);
}

int32_t Walker::getCountL() {
  return leftWheel.getCount();
}

int32_t Walker::getCountR() {
  return rightWheel.getCount();
}

void Walker::moveAngle(int8_t pwm, int angle) {
  leftWheel.reset();
  rightWheel.reset();

  leftWheel.setPWM(pwm);
  rightWheel.setPWM(pwm);

  while(1) {
    if (leftWheel.getCount() >= angle && rightWheel.getCount() >= angle) break;
    clock.sleep(4);
  }
}

void Walker::angleChange(int angle, int rotation) {
  int32_t defaultAngleL;
  int8_t dAngle = 72; // 45度におけるモーター回転数 (床材によって変わる？)

  if (rotation >= 0) {
    if (isRight) {
      rotation = 1;
    } else {
      rotation = -1;
    }
  } else {
    if (isRight) {
      rotation = -1;
    } else {
      rotation = 1;
    }
  }

  /*
 * 本来は45度単位だから、angleは45で割る
 * ベータ機能として5度単位でも曲がれるようにしている
 * そのため、もしangleが5度単位である場合はdAngleを9分割する
 */
  if (angle % 5 == 0 && angle % 45 != 0) {
    dAngle = 8;
    angle /= 5;
  } else {
    angle -= angle % 45;
    angle /= 45;
  }

  defaultAngleL = getCountL();

  while(1) {
    run(0, 10 * rotation);

    if (rotation >= 0) {
      if (getCountL() - defaultAngleL < -dAngle * angle * rotation ||
        getCountL() - defaultAngleL > dAngle * angle * rotation) {
        break;
      }
    } else {
      if (getCountL() - defaultAngleL > -dAngle * angle * rotation ||
        getCountL() - defaultAngleL < dAngle * angle * rotation) {
        break;
      }
    }

    clock.sleep(4);
  }

  stop();
}
