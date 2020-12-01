#ifndef FACEMORPHING_FACE_SIZE_EXCEPTION_H
#define FACEMORPHING_FACE_SIZE_EXCEPTION_H

#include <exception>

class face_size_exception: public std::exception
{
public:
    face_size_exception();
    virtual const char* what() const throw();
};


#endif //FACEMORPHING_FACE_SIZE_EXCEPTION_H
