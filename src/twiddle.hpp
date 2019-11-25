#ifndef TWIDDLE_H_
#define TWIDDLE_H_

#include <vector>
#include <iostream>
#include <numeric>  

const int SAMPLE_SIZE = 30;
const int MIN_TORELANCE = 0.2;

class Twiddle
{
public:
    Twiddle(double Kp, double Ki, double Kd);
    ~Twiddle();
    bool optimize(const double *cte);
private:
    void accumulateError(const double *cte);
    void updateParams(const double *cte);
    double getTolerance();
public:
    std::vector<double> params; // p,i,d
    bool is_optimized;
private:
    std::vector<double> param_ds;
    double best_cte;
    double total_cte;
    int accum_count;
    int twiddle_phase;
    int idx;
};

#endif // TWIDDLE_H_