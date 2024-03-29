/**
 * @brief Throttle the rate of data transfer between two pipes.
 *
 * @details The throttle command accepts two pipes, which may be
 * provided as named pipes or through standard input and standard
 * output. The default is to use standard input and stdandard output
 * unless the paths of the input or output pipe is provided.
 */

//
// ... miscutils header files
//
#include <miscutils/miscutils.hpp>

//
// ... Third-party header files
//
#include <boost/program_options.hpp>
#include <nlohmann/json.hpp>

//
// ... Standard header files
//
#include <chrono>
#include <cstdint>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <thread>

namespace {
   namespace po = boost::program_options;
   using nlohmann::json;

   po::options_description
   make_options_description() {
      po::options_description desc("Allowed options");
      desc.add_options()                                     //
        ("help", "show this help message and exit")          //
        ("input", "optional path to the named input pipe")   //
        ("output", "optional path to the named output pipe") //
        ("rate",
         po::value<std::size_t>()->default_value(1),
         "the rate expressed in bytes per second");
      return desc;
   }

   json
   process_command_line(int argc, char** argv) {
      po::options_description desc = make_options_description();
      po::variables_map vm{};
      po::store(po::parse_command_line(argc, argv, desc), vm);
      po::notify(vm);

      if(vm.count("help") == 0) {
         json config = json::object();
         if(vm.count("input")) {
            config[ "input" ] = vm[ "input" ].as<std::string>();
         }
         if(vm.count("output")) {
            config[ "output" ] = vm[ "output" ].as<std::string>();
         }
         config[ "rate" ] = vm[ "rate" ].as<std::size_t>();
         return config;
      } else {
         std::ostringstream oss;
         oss << desc;
         throw miscutils::help_request(oss.str());
      }
   }

   void
   run_with_throttle(std::istream& is, std::ostream& os, std::size_t target_rate) {
      using namespace std::literals;
      using time_point = std::chrono::time_point<std::chrono::steady_clock>;
      using clock = std::chrono::steady_clock;

      const time_point start(clock::now());

      std::for_each(
        std::istreambuf_iterator<char>{is},
        std::istreambuf_iterator<char>{},
        [ &, count = 0UL ](char x) mutable {
           double current_rate = count / static_cast<double>((clock::now() - start).count());
           while(current_rate > (target_rate / 1.0E09)) {
              os << std::flush;
              std::this_thread::yield();
              current_rate = count / static_cast<double>((clock::now() - start).count());
           }
           os << x;
           ++count;
        });
   }

   void
   run_without_throttle(std::istream& is, std::ostream& os) {
      std::copy(
        std::istreambuf_iterator<char>{is},
        std::istreambuf_iterator<char>{},
        std::ostreambuf_iterator<char>{os});
   }

   void
   run_aux(std::istream& is, std::ostream& os, std::size_t target_rate) {
      if(target_rate > 0) {
         run_with_throttle(is, os, target_rate);
      } else {
         run_without_throttle(is, os);
      }
   }

   void
   run(const json& config) {
      if(config.contains("input") && config.contains("output")) {
         std::ifstream ifs{config.at("input"), std::ios::binary};
         std::ofstream ofs{config.at("output"), std::ios::binary};
         run_aux(ifs, ofs, config.at("rate"));
      } else if(config.contains("input")) {
         std::ifstream ifs{config.at("input"), std::ios::binary};
         run_aux(ifs, std::cout, config.at("rate"));
      } else if(config.contains("output")) {
         std::ofstream ofs{config.at("output"), std::ios::binary};
         run_aux(std::cin, ofs, config.at("rate"));
      } else {
         run_aux(std::cin, std::cout, config.at("rate"));
      }
   }

} // namespace

int
main(int argc, char** argv) {
   try {
      run(process_command_line(argc, argv));
      return 0;
   } catch(...) { return miscutils::exception_handler(argc, argv); }
}
