#include "face_size_exception.h"

face_size_exception::face_size_exception() {}

const char *face_size_exception::what() const throw()
{
    return "The face in the picture is very big! Please try another picture that has more elements: like background, hair etc...";
}