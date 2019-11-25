#include "pid.h"
#include <iostream>

PID::PID(double Kp_, double Ki_, double Kd_)
{
  Init(Kp_, Ki_, Kd_);
}

PID::~PID() {}

void PID::Init(double Kp_, double Ki_, double Kd_) 
{
  Kp = Kp_;
  Ki = Ki_;
  Kd = Kd_;
  p_error = 0;
  i_error = 0;
  d_error = 0;
  std::cout << "P: " << Kp << "\tI: " << Ki << "\tD: " << Kd << std::endl;
}

void PID::UpdateError(const double *cte) {
  d_error = *cte - p_error;
  p_error = *cte;
  i_error += *cte;
  //std::cout << *cte << " " << i_error << std::endl;
}

double PID::TotalError() {
  return - Kp * p_error - Kd * d_error - Ki * i_error;
}