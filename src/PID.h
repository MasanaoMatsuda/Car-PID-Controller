#ifndef PID_H
#define PID_H

class PID {
 public:
  PID(double Kp_, double Ki_, double Kd_);
  virtual ~PID();
  void Init(double Kp_, double Ki_, double Kd_);
  void UpdateError(double cte);
  double TotalError();

 private:
  double p_error;
  double i_error;
  double d_error;
  double Kp;
  double Ki;
  double Kd;
};

#endif  // PID_H