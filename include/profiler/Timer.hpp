#pragma once

#include <atomic>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <string_view>

namespace profiler {

/**
 * @brief High-precision RAII Timer for systems profiling.
 * 
 * Uses compiler fences to prevent instruction reordering across timing boundaries.
 * Zero-allocation during construction and destruction by using std::string_view.
 */
class [[nodiscard]] Timer {
public:
  using Clock = std::chrono::high_resolution_clock;
  using TimePoint = Clock::time_point;

  explicit Timer(std::string_view name) noexcept
      : name_(name) {
    std::atomic_signal_fence(std::memory_order_acq_rel);
    start_point_ = Clock::now();
  }

  ~Timer() {
    const auto end_point = Clock::now();
    std::atomic_signal_fence(std::memory_order_acq_rel);

    const auto duration = end_point - start_point_;
    const auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
    const double ms = static_cast<double>(ns) / 1'000'000.0;

    std::cout << "[" << name_ << "] " 
              << std::fixed << std::setprecision(3) << ms << " ms ("
              << ns << " ns)\n";
  }

  // Safety: delete copy operations to prevent dual timing outputs
  Timer(const Timer&) = delete;
  Timer& operator=(const Timer&) = delete;

  // Move operations are permitted for returning guards from factory functions
  Timer(Timer&&) noexcept = default;
  Timer& operator=(Timer&&) noexcept = default;

private:
  std::string_view name_;
  TimePoint start_point_;
};

} // namespace profiler
