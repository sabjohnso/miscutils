/**
 * @brief Replace the pattern byte with replacement byte
 *
 * @details If the replacement is not provided, the matching bytes
 * will be deleted, otherwise matching bytes are replaced by the replacement
 * byte.
 */

//
// ... miscutils header files
//
#include <miscutils/miscutils.hpp>

//
// ... Thrid-party header files
//
#include <boost/program_options.hpp>
#include <nlohmann/json.hpp>

//
// ... Standard header files
//
#include <algorithm>
#include <fstream>
#include <iostream>

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
        ("pat",
         po::value<std::size_t>(),
         "required pattern byte") //
        ("rep", po::value<std::size_t>(), "required replacement byte");
      return desc;
   }

   json
   process_command_line(int argc, char** argv) {
      po::options_description desc = make_options_description();
      po::variables_map vm{};
      po::store(po::parse_command_line(argc, argv, desc), vm);
      if(vm.count("help") == 0) {
         json config = json::object();
         if(vm.count("input")) {
            config[ "input" ] = vm[ "input" ].as<std::string>();
         }
         if(vm.count("output")) {
            config[ "output" ] = vm[ "output" ].as<std::string>();
         }
         config[ "pat" ] = vm[ "pat" ].as<std::size_t>();
         if(vm.count("rep")) {
            config[ "rep" ] = vm[ "rep" ].as<std::size_t>();
         }
         return config;
      } else {
         std::ostringstream oss;
         oss << desc;
         throw miscutils::help_request(oss.str());
      }
   }

   void
   run_aux(std::istream& is, std::ostream& os, char pat, char rep) {
      std::transform(
        std::istreambuf_iterator<char>{is},
        std::istreambuf_iterator<char>{},
        std::ostreambuf_iterator<char>{os},
        [ = ](char c) { return c == pat ? rep : c; });
   }

   void
   run(const json& config) {
      if(config.contains("input") && config.contains("output")) {
         std::ifstream ifs{config.at("input"), std::ios::binary};
         std::ofstream ofs{config.at("output"), std::ios::binary};
         run_aux(
           ifs,
           ofs,
           config.at("pat").get<std::uint8_t>(),
           config.at("rep").get<std::uint8_t>());
      } else if(config.contains("input")) {
         std::ifstream ifs{config.at("input"), std::ios::binary};
         run_aux(
           ifs,
           std::cout,
           config.at("pat").get<std::uint8_t>(),
           config.at("rep").get<std::uint8_t>());
      } else if(config.contains("output")) {
         std::ofstream ofs{config.at("output"), std::ios::binary};
         run_aux(
           std::cin,
           ofs,
           config.at("pat").get<std::uint8_t>(),
           config.at("rep").get<std::uint8_t>());
      } else {
         run_aux(
           std::cin,
           std::cout,
           config.at("pat").get<std::uint8_t>(),
           config.at("rep").get<std::uint8_t>());
      }
   }

} // end of namespace

int
main(int argc, char** argv) {
   try {
      run(process_command_line(argc, argv));
      return 0;
   } catch(...) { return miscutils::exception_handler(argc, argv); }
}
