#pragma once

//
// ... Standard header files
//
#include <exception>
#include <string>

namespace miscutils::details {
   class help_request : public std::exception {
      std::string m_message;

   public:
      help_request(std::string message) : m_message(message) {}

      const char*
      what() const noexcept override {
         return m_message.c_str();
      }
   };
} // end of namespace miscutils::details
