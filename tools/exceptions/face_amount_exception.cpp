#include "face_amount_exception.h"

face_amount_exception::face_amount_exception(int amount)
{
    this->amount = amount;
}

const char *face_amount_exception::what() const throw()
{
    if (this->amount < 1)
        return "There are no faces in the picture. Please choose another picture with face.";
    return "There are too many faces in the picture. Please choose another picture with one face only.";
}