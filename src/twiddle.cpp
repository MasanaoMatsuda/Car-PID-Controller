#include <vector>
#include <iostream>
#include <numeric>
#include <cmath>
#include "twiddle.hpp"

Twiddle::Twiddle(double Kp, double Ki, double Kd)
    : params({Kp, Ki, Kd})
    , param_ds({(Kp/10), (Ki/10), (Kd/10)})
{
    is_optimized = false;
    best_cte = 1.0;
    total_cte = 0.0;
    accum_count = 0;
    twiddle_phase = 0;
    idx = 0;
}

Twiddle::~Twiddle()
{}

bool Twiddle::optimize(const double *cte)
{
    accumulateError(cte);

    if (accum_count == SAMPLE_SIZE)
    {
        double avg_cte = total_cte / accum_count;
        total_cte = 0;
        accum_count = 0;

        updateParams(&avg_cte);
        return true;
    }
    return false;
}

void Twiddle::accumulateError(const double *cte)
{
    total_cte += fabs(*cte);
    ++accum_count;
}

void Twiddle::updateParams(const double *cte)
{
    if (twiddle_phase == 0)
    {
        params[idx] += param_ds[idx];
        twiddle_phase = 1;
    }
    else if (twiddle_phase == 1)
    {
        if (*cte < best_cte)
        {
            best_cte = *cte;
            param_ds[idx] *= 1.1;
            twiddle_phase = 0;
            ++idx;
        }
        else
        {
            params[idx] -= 2 * param_ds[idx];
            twiddle_phase = 2;
        }
    }
    else if (twiddle_phase == 2)
    {
        if (*cte < best_cte)
        {
            best_cte = *cte;
            param_ds[idx] *= 1.1;
        }
        else
        {
            params[idx] += param_ds[idx];
            param_ds[idx] *= 0.9;
        }
        twiddle_phase = 0;
        ++idx;
    }

    if (idx == 3)
    {
        if (getTolerance() < MIN_TORELANCE)
            is_optimized = true;
        idx = 0;
    }
}

double Twiddle::getTolerance()
{
    return params[0] + params[1] + params[2];
}
