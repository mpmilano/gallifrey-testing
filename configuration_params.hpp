#pragma once
#include "mutils/Hertz.hpp"
#include "mutils/mutils.hpp"
#include "mutils/type_utils.hpp"
#include <cassert>
#include <chrono>
#include <locale>

namespace testing {

struct configuration_parameters {
  // required values
  mutils::Frequency client_freq{0};
  std::size_t starting_num_clients{0};
  mutils::Frequency increase_clients_freq{0};
  std::chrono::seconds test_duration{0};
  double percent_dedicated_connections{0};
  std::string output_file;
  std::chrono::seconds log_delay_tolerance{0};
  unsigned short log_every_n{0};
  unsigned short parallel_factor{0};

  // derived values
  std::size_t max_clients() const;

  template <typename T, typename U>
  std::size_t total_clients_at(std::chrono::duration<T, U> s) const {
    return starting_num_clients + (increase_clients_freq.operator*(s));
  }

  template <typename T, typename U>
  mutils::Frequency current_arrival_rate(std::chrono::duration<T, U> s) {
    return client_freq.operator*(total_clients_at(s));
  }

  template <typename T, typename U>
  std::size_t clients_still_inactive_at(std::chrono::duration<T, U> s) const {
    return max_clients() - total_clients_at(s);
  }

  std::size_t num_dedicated_connections() const;

  std::size_t num_spare_connections() const;
};

bool same_run(const configuration_parameters &p1,
              const configuration_parameters &p2);

std::ostream &operator<<(std::ostream &o, const configuration_parameters &p);

std::istream &operator>>(std::istream &i, configuration_parameters &p);

void read_from_args(configuration_parameters &params, char **args);
} // namespace testing
