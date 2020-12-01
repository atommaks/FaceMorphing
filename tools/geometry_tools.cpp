#include "geometry_tools.h"

void morph_triangle(cv::Mat& img1, cv::Mat& img2, cv::Mat &morphed_img, std::vector<cv::Point2f>& tri1,
                    std::vector<cv::Point2f>& tri2, std::vector<cv::Point2f>& tri, float t)
{
    cv::Rect r = cv::boundingRect(tri), r1 = cv::boundingRect(tri1), r2 = cv::boundingRect(tri2);

    std::vector<cv::Point2f> t1_rect, t2_rect, t_rect;
    std::vector<cv::Point> t_rect_int;
    for (int i = 0; i < 3; i++)
    {
        t_rect.push_back(cv::Point2f(tri[i].x - r.x, tri[i].y - r.y));
        t_rect_int.push_back(cv::Point(tri[i].x - r.x, tri[i].y - r.y));
        t1_rect.push_back(cv::Point2f(tri1[i].x - r1.x, tri1[i].y - r1.y));
        t2_rect.push_back(cv::Point2f(tri2[i].x - r2.x, tri2[i].y - r2.y));
    }
    cv::Mat mask = cv::Mat::zeros(r.height, r.width, CV_32FC3);
    cv::fillConvexPoly(mask, t_rect_int, cv::Scalar(1.f, 1.f, 1.f), 16, 0);

    cv::Mat img1_rect, img2_rect;
    img1(r1).copyTo(img1_rect);
    img2(r2).copyTo(img2_rect);

    cv::Mat warp_img1 = cv::Mat::zeros(r.height, r.width, img1_rect.type());
    cv::Mat warp_img2 = cv::Mat::zeros(r.height, r.width, img2_rect.type());

    apply_affine_transform(img1_rect, warp_img1, t1_rect, t_rect);
    apply_affine_transform(img2_rect, warp_img2, t2_rect, t_rect);

    cv::Mat morphed_img_rect = (1.0f - t) * warp_img1 + t * warp_img2;

    cv::multiply(morphed_img_rect, mask, morphed_img_rect);
    cv::multiply(morphed_img(r), cv::Scalar(1.0f, 1.0f, 1.0f) - mask, morphed_img(r));
    morphed_img(r) = morphed_img(r) + morphed_img_rect;
}

void apply_affine_transform(cv::Mat& src, cv::Mat& dst, std::vector<cv::Point2f>& src_tri,
                            std::vector<cv::Point2f>& dst_tri)
{
    cv::Mat warp_mat = cv::getAffineTransform(src_tri, dst_tri);
    cv::warpAffine(src, dst, warp_mat, dst.size(), cv::INTER_LINEAR, cv::BORDER_REFLECT_101);
}

void draw_delaunay(cv::Mat &img, std::vector<std::vector<int>> &tri_indeces_list,std::vector<cv::Point2f> &landmarks, cv::Scalar color)
{
    std::vector<cv::Point> pt(3);
    cv::Size size = img.size();
    cv::Rect rect(0,0, size.width, size.height);
    for (int i = 0; i < tri_indeces_list.size(); i++)
    {
        pt[0] = cv::Point(landmarks[tri_indeces_list[i][0]]);
        pt[1] = cv::Point(landmarks[tri_indeces_list[i][1]]);
        pt[2] = cv::Point(landmarks[tri_indeces_list[i][2]]);
        if ( rect.contains(pt[0]) && rect.contains(pt[1]) && rect.contains(pt[2]))
        {
            line(img, pt[0], pt[1], color, 1, CV_AA, 0);
            line(img, pt[1], pt[2], color, 1, CV_AA, 0);
            line(img, pt[2], pt[0], color, 1, CV_AA, 0);
        }
    }
}