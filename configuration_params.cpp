#include "configuration_params.hpp"

namespace testing {

std::size_t configuration_parameters::max_clients() const {
  return starting_num_clients +
         (increase_clients_freq.operator*(test_duration));
}

std::size_t configuration_parameters::num_dedicated_connections() const {
  return max_clients() * percent_read;
}

std::size_t configuration_parameters::num_spare_connections() const {
  return max_clients() - num_dedicated_connections();
}

bool same_run(const configuration_parameters &p1,
              const configuration_parameters &p2) {
  return p1.client_freq == p2.client_freq &&
         p1.starting_num_clients == p2.starting_num_clients &&
         p1.increase_clients_freq == p2.increase_clients_freq &&
         p1.test_duration == p2.test_duration &&
         p1.percent_read == p2.percent_read &&
         p1.log_every_n == p2.log_every_n &&
         p1.parallel_factor == p2.parallel_factor;
}

std::ostream &operator<<(std::ostream &o, const configuration_parameters &p) {
  using namespace mutils;
  constexpr mutils::comma_space cs{};
  return o << p.client_freq << cs << p.starting_num_clients << cs
           << p.increase_clients_freq << cs << p.test_duration << cs
           << p.percent_read << cs << p.output_file << cs
           << p.log_delay_tolerance << cs << p.log_every_n << cs
           << p.parallel_factor;
}

std::istream &operator>>(std::istream &i, configuration_parameters &p) {
  using namespace mutils;
  i.imbue(std::locale(i.getloc(), new mutils::comma_is_space()));
  constexpr mutils::comma_space cs{};
  i >> p.client_freq >> cs >> p.starting_num_clients >> cs >>
      p.increase_clients_freq >> cs >> p.test_duration >> cs >>
      p.percent_read >> cs >> p.output_file >> cs >>
      p.log_delay_tolerance >> cs >> p.log_every_n >> cs >> p.parallel_factor;
  return i;
} //*/

void read_from_args(configuration_parameters &params, char **args) {
  using namespace std;
  using namespace mutils;

  {
    std::istringstream ss{std::string{args[4]}};
    std::cout << "client_freq decoding: " << args[4] << std::endl;
    ss >> params.client_freq;
  }
  {
    std::istringstream ss{std::string{args[5]}};
    std::cout << "starting_num_clients decoding: " << args[5] << std::endl;
    ss >> params.starting_num_clients;
  }
  {
    std::istringstream ss{std::string{args[6]}};
    std::cout << "increase_clients_freq decoding: " << args[6] << std::endl;
    ss >> params.increase_clients_freq;
  }
  {
    std::istringstream ss{std::string{args[7]}};
    std::cout << "test_duration decoding: " << args[7] << std::endl;
    ss >> params.test_duration;
  }
  {
    std::istringstream ss{std::string{args[8]}};
    std::cout << "percent_read decoding: " << args[8]
              << std::endl;
    ss >> params.percent_read;
  }
  {
    std::cout << "output_file decoding: " << args[11] << std::endl;
    params.output_file = args[11];
  }
  {
    std::istringstream ss{std::string{args[12]}};
    std::cout << "log_delay_tolerance decoding: " << args[12] << std::endl;
    ss >> params.log_delay_tolerance;
  }
  {
    std::istringstream ss{std::string{args[13]}};
    std::cout << "log_every_n decoding: " << args[13] << std::endl;
    ss >> params.log_every_n;
  }
  {
    std::istringstream ss{std::string{args[14]}};
    std::cout << "parallel_factor decoding: " << args[14] << std::endl;
    ss >> params.parallel_factor;
  }
}
} // namespace testing
