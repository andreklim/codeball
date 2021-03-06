#ifndef CODEBALL_MYTIMER_H
#define CODEBALL_MYTIMER_H

//#define CHRONO

#ifdef CHRONO
#include <chrono>
#else
#ifdef LOCAL
#include <model/getCPUTime.h>
#else
#include "getCPUTime.h"
#endif
#endif

struct MyTimer {
#ifdef CHRONO
  std::chrono::time_point<std::chrono::high_resolution_clock> _start, _end;
#else
  double _start = 0, _end = 0;
#endif
  MyTimer() {
    start();
    end();
  }

  double cur_ = 0;
  double cumulative = 0;
  int k = 0;
  double max_ = 0;
  int64_t sum_calls = 0;
  int64_t captures = 0;
  int64_t last_calls = 0;

  void start() {
#ifdef CHRONO
    _start = std::chrono::high_resolution_clock::now();
#else
    _start = CPUTime::getCPUTime();
#endif
  }
  void end() {
#ifdef CHRONO
    _end = std::chrono::high_resolution_clock::now();
#else
    _end = CPUTime::getCPUTime();
#endif
  }

  double delta(bool accumulate = false) {
#ifdef CHRONO
    double res = 1e-9 * std::chrono::duration_cast<std::chrono::nanoseconds>(_end - _start).count();
#else
    double res = _end - _start;
#endif
    if (accumulate) {
      cumulative += res;
      k++;
      return cumulative;
    }
    return res;
  }
  double cur(bool accumulate = false, bool counter = false, bool upd_max_ = false) {
#ifdef CHRONO
    double res = 1e-9 * std::chrono::duration_cast<std::chrono::nanoseconds>
        (std::chrono::high_resolution_clock::now() - _start).count();
    cumulative -= 640e-10;
#else
    double res = CPUTime::getCPUTime() - _start - 0.0000008715;
#endif
    if (counter) {
      k++;
    }
    if (upd_max_) {
      max_ = std::max(max_, res);
    }
    if (accumulate) {
      cumulative += res;
      cur_ += res;
    }
    return res;
  }

  void clear() {
    cumulative = 0;
    k = 0;
    max_ = 0;
    cur_ = 0;
  }

  void clearCur() {
    cur_ = 0;
  }

  double getCur() {
    return cur_;
  }

  double avg() {
    return cumulative / (k == 0 ? 1 : k);
  }

  double max() {
    return max_;
  }

  double getCumulative(bool with_cur = false) {
    if (!with_cur) {
      return cumulative;
    }
    return cumulative + cur();
  }

  void call() {
    sum_calls++;
    last_calls++;
  }

  void capture() {
    captures++;
  }

  void init_calls() {
    last_calls = 0;
  }

  int64_t avg_() {
    return captures == 0 ? 0 : sum_calls / captures;
  }

  int64_t last_() {
    return last_calls;
  }

};

#endif //CODEBALL_MYTIMER_H
