#pragma once
#include "types.h"
#include "kernel/memory/memory.h"
#include "kernel/virtual-console/virtual-console.h"

namespace kernel {
  namespace string {

    class String {
    public:
      String(const char* string)
        : _length(length(string)),
          string(reinterpret_cast<char*>(kernel::memory::kmalloc(_length))) {
        for(size_t i = 0; i < _length; i++) {
          this->string[i] = string[i];
        }
      }
      String(uint32_t length)
        : _length(length)
        , string(new char[length]) {}

      static size_t length(const char* string) {
        size_t length = 0;
        while('\0' != string[length]) {
          length++;
        }
        kernel::VirtualConsole::currentConsole()->put("LENGTH:");
        kernel::VirtualConsole::currentConsole()->put(length);
        return length;
      }

        size_t length() const {  return _length; };
      inline static size_t length(const String* string) {
        return string->length();
      }
      int toInt() {
        int number = 0;
        for(int i = this->length(),  power=1; i>=0; i++, power*=10) {
          number+=(this->string[i] - 0x30) * power;
        }
        return number;
      }

      char const &operator[] (int i) const {
        return string[i];
      }
      char& operator[] (int i) {
        return string[i];
      }
      char  &at(int i)  {
        return string[i];
      }
    private:
      /// @todo Make accessor function for the length.
      size_t _length;
    public:
      char* string;
    };
    // String strcat(String *string1, String *string2) {
    //   char temp_c_string[string1->length() + string2->length() + 1];
    //   int indice1=0;
    //   int indice2=0;
    //   while(string1->string[indice1]!="\0") {
    //     temp_c_string[indice2] = string1->string[indice1];
    //     indice1++;
    //     indice2++;
    //   }
    //   indice1=0;
    //   while(string2->string[indice1]!="\0") {
    //     temp_c_string[indice2] = string1->string[indice1];
    //     indice1++;
    //     indice2++;
    //   }
    //   String *tempString = new String(temp_c_string);
    //   return *tempString;
    // }
    // int str2int(String *string) {
    //   return string->toInt();
    // }
    // string make_string(const char* string);
    //      Length of a string, C and pascal styles respectively.
  }
}

