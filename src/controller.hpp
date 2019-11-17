#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include "pid.h"
#include "twiddle.hpp"

const double max_steering_angle = 1;

class Controller
{
public:
  Controller(PID pid, Twiddle twiddle);
  ~Controller();
  double get_steering(double cte);
private:
  void apply_phisical_limitations(double *steering);
private:
  PID pid;
  Twiddle twiddle;
  double steering_angle;
};
#endif // CONTROLLER_H_
