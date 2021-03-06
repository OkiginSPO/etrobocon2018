/*****************************
 * self_localization.cpp
 *
 * Author Hiroki Tachiyama
 * Katayama Laboratory
 * Department of Computer Secience and Systems Engineering
 * Faculty of Engineering, University of Miyazaki
 *
 * Created at: Mon Jul 10 3:34:25 JST 2017
 *
 * Coryrights (c) 2017 Katayama Laboratory
 *
 *****************************/
#include "SelfLocalization.h"

FILE* SelfLocalization::fp = fopen("traveling_route.txt", "w");
bool SelfLocalization::isSave;

SelfLocalization::SelfLocalization (std::int32_t left_motor_sl, std::int32_t right_motor_sl, bool save):
  left(left_motor_sl), right(right_motor_sl){
  //メンバ変数の初期化 基本的に0
  between_wheels = TREAD;
  moving_distance_mean = 0;
  turning_angle = 0;
  current_x = current_y = current_angle = 0;
  // errno = 0;
  isSave = save;
}

void SelfLocalization::update (std::int32_t left_motor_sl, std::int32_t right_motor_sl) {

  //左車輪の回転角
  left.update(left_motor_sl);

  //右車輪の回転角
  right.update(right_motor_sl);

  //移動距離と、車体の回転角
  moving_distance_mean = (right.moving_distance + left.moving_distance) / 2.0;
  turning_angle = (right.moving_distance - left.moving_distance) / between_wheels;

  //座標
  current_x += (moving_distance_mean * std::cos(current_angle + (turning_angle/2.0)));
  current_y += (moving_distance_mean * std::sin(current_angle + (turning_angle/2.0)));
  current_angle += turning_angle;

  //保存
  if(isSave == true){
    writing_current_coordinates();
  }
}

float SelfLocalization::getPointX(){
  return current_x;
}

float SelfLocalization::getPointY(){
  return current_y;
}

void SelfLocalization::writing_current_coordinates() {

  fprintf(fp, "%f %f\n", current_x, current_y);

  return;
}

bool SelfLocalization::approached_target_coordinates (float target_x, float target_y, float target_radius) {
  float distance = std::sqrt(
		      (target_x - current_x) * (target_x - current_x) +
		      (target_y - current_y) * (target_y - current_y)   );
  if(distance < target_radius)
    return true;
  return false;
}

//高校の時の数学で出た点と直線の距離の公式
//点(x0, y0)と直線ax + by + c = 0の距離dは、d = |a*x0 + by0 + c| / (a^2 + b^2)^(1/2)
//式の整理は自分で計算したやつ
float SelfLocalization::calculate_between_ev3_and_border
(float _start_x, float _start_y, float _goal_x, float _goal_y, float _current_x, float _current_y) {
  float a, b, c;

  a = _goal_y - _start_y;
  b = -(_goal_x - _start_x);
  c = -b * _start_y - a * _start_x;
  
  return (a*_current_x + b*_current_y + c) / std::sqrt(a*a + b*b);
}

void SelfLocalization::calculate_current_angle(){
  current_angle_degree = int(current_angle*180/3.14);
}

bool SelfLocalization::is_over_target_line_of_x(float target_x) {
  return target_x < current_x;
}
bool SelfLocalization::is_over_target_line_of_y(float target_y) {
  return target_y < current_y;
}

bool SelfLocalization::is_below_target_line_of_x(float target_x) {
  return target_x > current_x;
}
bool SelfLocalization::is_below_target_line_of_y(float target_y) {
  return target_y > current_y;
}


//スタートとゴールの登録、現在の機体位置のyがゴールの法線の下か否かの登録
void SelfLocalization::init_normal_vector
(float _start_x, float _start_y, float _goal_x, float _goal_y, float _current_x, float _current_y) {
  start_x = _start_x; start_y = _start_y;
  goal_x = _goal_x; goal_y = _goal_y;

  is_below_normal_vector = (start_y < goal_y);
}

//指定した二点 (start, goal)を結んだ直線の点goalにおける法線 (normal vector)
bool SelfLocalization::is_over_normal_vector
(float _current_x, float _current_y) {
  float k = (start_x - goal_x) / (goal_y - start_y);
  float border_y = k * _current_x + goal_y - k * goal_x;

  if (is_below_normal_vector) { //init時に機体が法線より下の場合
    return _current_y >= border_y;
  } else { //init時に機体が法線以上にある場合
    return _current_y < border_y;
  }
}

void SelfLocalization::file_close(){
  fclose(fp);
}
