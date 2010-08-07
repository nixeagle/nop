#pragma once
namespace kernel {
  namespace panic {
    /** Clear the console and place the boilerplate strings at the top

        \param message[in] Error message to print
        \param function[in] String with details on calling function. In
        GCC this can be generated by __PRETTY_FUNCTION__
     */
    void header (const char* message, const char* function,
                 int line, const char* file, const char* timestamp);
    void kpanic (const char* issue ,const char* message, const char* function,
                 int line, const char* file, const char* timestamp);
  }
}

#define KPANIC(info, message) kernel::panic::kpanic(info, message, __PRETTY_FUNCTION__, __LINE__, __FILE__, __TIMESTAMP__)
