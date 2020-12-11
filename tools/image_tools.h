#ifndef FACEMORPHING_IMAGE_TOOLS_H
#define FACEMORPHING_IMAGE_TOOLS_H

#include <map>

#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>

#include <dlib/image_processing/shape_predictor.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/opencv/cv_image.h>

#include "exceptions/face_amount_exception.cpp"
#include "exceptions/face_size_exception.cpp"

void check_image_size(cv::Mat&);
void make_equal_size(cv::Mat&, cv::Mat&, dlib::shape_predictor&);
void crop_image(cv::Mat&, dlib::shape_predictor&);
std::vector<std::vector<int>> get_tri_list_indices(cv::Mat&, std::vector<cv::Point2f>&);
std::vector<cv::Point2f> get_landmarks_points(cv::Mat&, dlib::shape_predictor&);
dlib::rectangle get_face_rectangle(cv::Mat&, dlib::shape_predictor&);

#endif