#include <profiler/Timer.hpp>
#include <vector>
#include <numeric>

/**
 * @brief Simulates a workload that mimics parsing market data packets.
 */
void simulate_packet_parsing(int packet_count) {
  profiler::Timer t("Packet Parsing Loop");
  
  std::vector<int> packet_buffer(1024);
  volatile int checksum = 0;

  for (int i = 0; i < packet_count; ++i) {
    // Sub-task timing: individual packet processing
    // Note: In high-frequency loops, this might be too verbose, 
    // but useful for illustrating nested timing.
    {
      profiler::Timer t2("Single Packet Transform");
      std::iota(packet_buffer.begin(), packet_buffer.end(), i);
      for (auto val : packet_buffer) {
        checksum += val;
      }
    }
  }
}

int main() {
  std::cout << "--- SystemsProfiler Demonstration ---\n";

  {
    profiler::Timer total("Total Execution Time");

    const int total_packets = 5;
    simulate_packet_parsing(total_packets);
  }

  std::cout << "--- Demonstration Complete ---\n";
  return 0;
}
