#include "file_read_exception.h"

const char* file_read_exception::what() const noexcept {
    return message.c_str();
}
