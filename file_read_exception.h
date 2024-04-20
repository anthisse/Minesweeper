#ifndef MINESWEEPER_FILE_READ_EXCEPTION_H
#define MINESWEEPER_FILE_READ_EXCEPTION_H

#include <string>

class file_read_exception : public std::exception {
private:
    std::string message;
public:
    explicit file_read_exception(const char* msg) : message(msg){};

    const char* what() const noexcept override;
};


#endif //MINESWEEPER_FILE_READ_EXCEPTION_H
