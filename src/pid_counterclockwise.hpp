#include "pid.h"

class PIDCounterclockwize : public PID
{
public:
    PIDCounterclockwize(double Kp_, double Ki_, double Kd_);
    ~PIDCounterclockwize();
    void UpdateError(const double *cte);
private:
    const double BIAS_P = 0.0; // Fixed
    const double BIAS_D = 0.0;

    int count = 0;
    const int PRINT = 50;
};