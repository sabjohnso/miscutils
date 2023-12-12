//
// ... miscutils header files
//
#include <miscutils/details/help_request.hpp>

//
// ... Standard header files
//
#include <algorithm>
#include <exception>
#include <iostream>
#include <iterator>

namespace miscutils::details {

   class exception_handler {
      int exit_code = 1;

   public:
      exception_handler(int argc, char** argv) {
         show_invocation(argc, argv);
         process_exception();
      }
      operator int() const { return exit_code; }

   private:
      void
      process_exception() {
         try {
            std::rethrow_exception(std::current_exception());
         } catch(help_request& req) {
            std::cerr << req.what() << std::endl;
            exit_code = 0;
         } catch(std::exception& e) {
            std::cerr << "an unexpected exception was encountered:\n"
                      << "\t" << e.what() << std::endl;
         }
      }

      void
      show_invocation(int argc, char** argv) {
         std::cerr << "Invocation:\n\t";
         auto out = std::ostream_iterator<std::string>{std::cerr, " "};
         std::copy_n(argv, argc, out);
         std::cout << std::endl;
      }
   };
} // end of namespace miscutils::details
