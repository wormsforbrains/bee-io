//
// Created by Kai on 11/15/2023.
//

#include "utils.h"

double Utils::TimeitResult::min_time() const {
  double min = times[0];
  for (auto time : times) {
    if (time < min) {
      min = time;
    }
  }
  return min;
}

double Utils::TimeitResult::max_time() const {
  double max = times[0];
  for (auto time : times) {
    if (time > max) {
      max = time;
    }
  }
  return max;
}

double Utils::TimeitResult::variance() const {
  double variance = 0;
  for (auto time : times) {
    variance += (time - mean_in_nanoseconds) * (time - mean_in_nanoseconds);
  }
  variance /= times.size();
  return variance;
}

double Utils::TimeitResult::margin_of_error() const {
  return 1.96 * standard_deviation_in_nanoseconds / sqrt(times.size());
}

double Utils::TimeitResult::confidence_interval() const {
  return 2 * margin_of_error();
}

double Utils::TimeitResult::confidence_lower_bound() const {
  return mean_in_nanoseconds - margin_of_error();
}

double Utils::TimeitResult::confidence_upper_bound() const {
  return mean_in_nanoseconds + margin_of_error();
}

std::ostream &operator<<(std::ostream &os, const Utils::TimeitResult &result) {
  os << "TimeitResult { " << std::endl;
  os << "  times: [";
  for (size_t i = 0; i < result.times.size(); i++) {
    os << result.times[i];
    if (i != result.times.size() - 1) {
      os << ", ";
    }
  }
  os << "]," << std::endl;
  os << "  mean_in_nanoseconds: " << result.mean_in_nanoseconds << "," << std::endl;
  os << "  standard_deviation_in_nanoseconds: " << result.standard_deviation_in_nanoseconds << "," << std::endl;
  os << "  mean_in_units: " << result.mean_in_units << "," << std::endl;
  os << "  standard_deviation_in_units: " << result.standard_deviation_in_units << "," << std::endl;
  os << "  mean_unit: " << result.mean_unit << "," << std::endl;
  os << "  standard_deviation_unit: " << result.standard_deviation_unit << "," << std::endl;
  os << "  loops: " << result.loops << "," << std::endl;
  os << "  runs: " << result.runs << "," << std::endl;
  os << "  min_time: " << result.min_time() << "," << std::endl;
  os << "  max_time: " << result.max_time() << "," << std::endl;
  os << "}";
  return os;
}

Utils::TimeitResult Utils::timeit(std::function<void()> &func, size_t loops, size_t runs) {
  // timeit gives output like this:
  //     2.61 s ± 17.5 ms per loop (mean ± std. dev. of 7 runs, 100 loops each)
  //     2.61 ms ± 17.5 µs per loop (mean ± std. dev. of 7 runs, 100 loops each)
  //     25.4 µs ± 90.6 ns per loop (mean ± std. dev. of 7 runs, 10,000 loops each)
  //     25.4 ns ± 1.41 ns per loop (mean ± std. dev. of 7 runs, 100,000 loops each)
  //
  // If the number of loops is not specified, timeit will automatically determine
  // the number of loops to run by repeatedly calling the function an

  // Determine the number of loops to run
  if (loops == 0) {
    // Keep doubling the number of loops until the total time is at least 0.2 seconds
    size_t total_loops = 1;
    auto start = std::chrono::high_resolution_clock::now();
    while (true) {
      for (size_t i = 0; i < total_loops; i++) {
        func();
      }
      auto end = std::chrono::high_resolution_clock::now();
      size_t total_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
      if (total_time >= 2e8) {
        loops = total_loops;
        break;
      }
      total_loops *= 2;
    }
  }

  // Run the function once to warm up the cache
  func();

  // Run the function multiple times and time it
  std::vector<double> times(runs);
  for (size_t i = 0; i < runs; i++) {
    auto start = std::chrono::high_resolution_clock::now();
    for (size_t j = 0; j < loops; j++) {
      func();
    }
    auto end = std::chrono::high_resolution_clock::now();
    times[i] = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / static_cast<double>(loops);
  }

  // Calculate the mean and standard deviation
  double mean = 0;
  for (auto time : times) {
    mean += time;
  }
  mean /= times.size();

  double std_dev = 0;
  for (auto time : times) {
    std_dev += (time - mean) * (time - mean);
  }
  std_dev /= times.size();
  std_dev = sqrt(std_dev);

  // Determine the units for the mean and standard deviation
  std::string mean_units;
  double mean_in_units;
  if (mean >= 1e9) {
    mean_units = "s";
    mean_in_units = mean / 1e9;
  } else if (mean >= 1e6) {
    mean_units = "ms";
    mean_in_units = mean / 1e6;
  } else if (mean >= 1e3) {
    mean_units = "µs";
    mean_in_units = mean / 1e3;
  } else {
    mean_units = "ns";
    mean_in_units = mean;
  }

  std::string std_dev_units;
  double std_dev_in_units;
  if (std_dev >= 1e9) {
    std_dev_units = "s";
    std_dev_in_units = std_dev / 1e9;
  } else if (std_dev >= 1e6) {
    std_dev_units = "ms";
    std_dev_in_units = std_dev / 1e6;
  } else if (std_dev >= 1e3) {
    std_dev_units = "µs";
    std_dev_in_units = std_dev / 1e3;
  } else {
    std_dev_units = "ns";
    std_dev_in_units = std_dev;
  }

  // Format the mean and standard deviation
  std::cout << std::fixed << std::setprecision(1);

  // Format the number of runs
  std::string runs_str = std::to_string(runs);
  std::string formatted_runs;
  for (size_t i = 0; i < runs_str.size(); i++) {
    formatted_runs += runs_str[i];
    if ((runs_str.size() - i - 1) % 3 == 0 && i != runs_str.size() - 1) {
      formatted_runs += ",";
    }
  }

  // Format the number of loops
  std::string loops_str = std::to_string(loops);
  std::string formatted_loops;
  for (size_t i = 0; i < loops_str.size(); i++) {
    formatted_loops += loops_str[i];
    if ((loops_str.size() - i - 1) % 3 == 0 && i != loops_str.size() - 1) {
      formatted_loops += ",";
    }
  }

  // Print the results
  std::cout << mean_in_units << " " << mean_units << " ± " << std_dev_in_units << " " << std_dev_units << " per loop "
            << "(mean ± std. dev. of " << formatted_runs << " runs, " << formatted_loops << " loops each)" << std::endl;

  return { times, mean, std_dev, mean_in_units, std_dev_in_units, mean_units, std_dev_units, loops, runs };
}
