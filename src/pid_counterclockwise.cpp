#include "pid_counterclockwise.hpp"
#include <iostream>

PIDCounterclockwize::PIDCounterclockwize(double Kp_, double Ki_, double Kd_)
    : PID(Kp_, Ki_, Kd_)
{}

PIDCounterclockwize::~PIDCounterclockwize()
{}

void PIDCounterclockwize::UpdateError(const double *cte)
{
    d_error = *cte - (p_error - BIAS_P) + BIAS_D;
    p_error = *cte + BIAS_P;
    i_error += *cte * 0.001; 
    //if (*cte >= 0.1)
    //    i_error += *cte * 0.002;
    //else if (*cte < 0)
    //    i_error += *cte * 0.006;
    
    if (++count % PRINT == 0)
        std::cout << *cte << "\t" << p_error << "\t" << i_error << "\t" << d_error << std::endl;
}