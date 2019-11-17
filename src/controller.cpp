#include "controller.hpp"
#include <iostream>
#include <cmath>

Controller::Controller(PID pid, Twiddle twiddle)
  : pid(pid)
  , twiddle(twiddle)
{}

Controller::~Controller()
{}

double Controller::get_steering(double cte)
{
  if (!twiddle.is_optimized)
    if (twiddle.optimize(cte))
      pid.Init(twiddle.params[0], twiddle.params[1], twiddle.params[2]);
  
  pid.UpdateError(cte);
  steering_angle = pid.TotalError();

  apply_phisical_limitations(&steering_angle);

  return steering_angle;
}

void Controller::apply_phisical_limitations(double *steering)
{
  if (*steering > max_steering_angle)
    *steering = max_steering_angle;
  else if (*steering < -max_steering_angle)
    *steering = -max_steering_angle;
}