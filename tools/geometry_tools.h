#ifndef FACEMORPHING_GEOMETRY_TOOLS_H
#define FACEMORPHING_GEOMETRY_TOOLS_H

#include <opencv2/core/mat.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgproc/imgproc_c.h>

void apply_affine_transform(cv::Mat&, cv::Mat&, std::vector<cv::Point2f>&, std::vector<cv::Point2f>&);
void morph_triangle(cv::Mat&, cv::Mat&, cv::Mat&, std::vector<cv::Point2f>&, std::vector<cv::Point2f>&, std::vector<cv::Point2f>&, float);
void draw_delaunay(cv::Mat&, std::vector<std::vector<int>>&,std::vector<cv::Point2f>&, cv::Scalar);

#endif