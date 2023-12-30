//
// Created by Kai on 11/15/2023.
//

#ifndef PNG_IO_UTILS_H
#define PNG_IO_UTILS_H

#include <chrono>
#include <cmath>
#include <functional>
#include <iostream>

struct Utils {
    struct TimeitResult {
        // The average times of each run's loops in nanoseconds
        std::vector<double> times;
        // The mean and standard deviation of times in nanoseconds
        double mean_in_nanoseconds;
        double standard_deviation_in_nanoseconds;
        // The mean and standard deviation of times in a suitable unit (always < 1000)
        double mean_in_units;
        double standard_deviation_in_units;
        // The unit of the mean and standard deviation
        std::string mean_unit;               // "s", "ms", "µs", "ns"
        std::string standard_deviation_unit; // "s", "ms", "µs", "ns"
        // The number of loops and runs
        size_t loops; // The number of times the function was called in each run
        size_t runs;  // The number of times the loops were run

        double min_time() const;
        double max_time() const;
        double variance() const;
        double margin_of_error() const;
        double confidence_interval() const;
        double confidence_lower_bound() const;
        double confidence_upper_bound() const;

        friend std::ostream& operator<<(std::ostream& os, const TimeitResult& result);
    };

    static TimeitResult timeit(std::function<void()>& func, size_t loops = 0, size_t runs = 7);
};

#endif // PNG_IO_UTILS_H
