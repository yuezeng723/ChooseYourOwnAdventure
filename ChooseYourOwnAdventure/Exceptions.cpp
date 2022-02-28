#include "Exceptions.hpp"

const char * myException::what() const throw(){
    return errInfo.c_str();
}
