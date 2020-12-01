#ifndef FACEMORPHING_IMAGE_TOOLS_H
#define FACEMORPHING_IMAGE_TOOLS_H

#include <opencv2/core/mat.hpp>
#include <dlib/image_processing/shape_predictor.h>
#include <opencv2/imgproc.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/opencv/cv_image.h>

#include "exceptions/face_amount_exception.cpp"
#include "exceptions/face_size_exception.cpp"

void make_equal_size(cv::Mat&, cv::Mat&, dlib::shape_predictor&);
void crop_image(cv::Mat&, dlib::shape_predictor&);
int find_point_index(cv::Point, std::vector<cv::Point2f>&);
std::vector<std::vector<int>> get_tri_list_indices(cv::Mat&, std::vector<cv::Point2f>&);
std::vector<cv::Point2f> get_landmarks_points(cv::Mat&, dlib::shape_predictor&);
dlib::rectangle get_face_rectangle(cv::Mat&, dlib::shape_predictor&);

#endif