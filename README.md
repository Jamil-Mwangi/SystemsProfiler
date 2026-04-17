# SystemsProfiler

A high-performance, production-quality C++ RAII profiling utility designed for low-latency systems and High-Frequency Trading (HFT) environments.

## Project Structure

```text
SystemsProfiler/
├── CMakeLists.txt              # Build configuration (C++17, -O3, -Wall)
├── include/
│   └── profiler/
│       └── Timer.hpp           # Header-only RAII Timer utility
├── src/
│   └── main.cpp                # High-fidelity demonstration & workload simulation
└── README.md                   # Project documentation
```

## Technical Design Choices

### 1. RAII for Exception Safety
The `profiler::Timer` utilizes the **Resource Acquisition Is Initialization (RAII)** pattern. The timer starts upon construction and automatically stops and reports upon destruction. This design guarantees that timing results are captured even if an exception is thrown within the measured scope, as stack unwinding will trigger the destructor.

### 2. Compiler Fencing with `std::atomic_signal_fence`
To ensure measurement accuracy, the utility employs `std::atomic_signal_fence(std::memory_order_acq_rel)` at construction and destruction boundaries. This prevents the compiler from reordering code under measurement outside the timing window, ensuring that the captured duration reflects the intended workload without the overhead of a full hardware memory barrier.

### 3. Zero-Allocation Performance (`std::string_view`)
Identifiers in `SystemsProfiler` are handled using `std::string_view`. This avoids heap allocations (`malloc`/`new`) during the critical path of measurement, which is essential for deterministic performance in low-latency environments where memory allocation jitter must be eliminated.

## Getting Started

### Prerequisites
- C++17 compliant compiler (GCC 7+, Clang 5+, or MSVC 2017+)
- CMake 3.10+

### Building the Project
```bash
mkdir build
cd build
cmake ..
cmake --build .
```

### Running the Demonstration
```bash
./ProfilerDemo
```

## Usage Example
```cpp
#include <profiler/Timer.hpp>

void critical_path() {
    profiler::Timer t("Critical Path Loop");
    // ... latency-sensitive work ...
}
```
