#include "walker.h"
#include "common.h"

Walker::Walker():
  leftWheel(left_motor), rightWheel(right_motor) {
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
  int8_t dAngle = 75; // 45度におけるモーター回転数 (床材によって変わる？)

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
