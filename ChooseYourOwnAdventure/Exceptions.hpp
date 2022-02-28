#ifndef __EXCEPTIONS_H__
#define __EXCEPTIONS_H__
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>


class myException : public std::exception{
private:
    std::string errInfo;
public:
    myException(std::string str) : errInfo(str){}
    ~myException() throw (){}
    virtual const char * what() const throw();
};

#endif
