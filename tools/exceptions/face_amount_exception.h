#ifndef FACEMORPHING_FACEAMOUNTEXCEPTION_H
#define FACEMORPHING_FACEAMOUNTEXCEPTION_H

#include <exception>

class face_amount_exception: public std::exception
{
private:
    int amount;
public:
    face_amount_exception(int);
    virtual const char* what() const throw();
};

#endif //FACEMORPHING_FACEAMOUNTEXCEPTION_H
