#include "image_tools.h"

std::vector<std::vector<int>> get_tri_list_indices(cv::Mat& img, std::vector<cv::Point2f>& landmarks)
{
    cv::Rect rect = cv::Rect(0, 0, img.size().width, img.size().height);
    cv::Subdiv2D subdiv = cv::Subdiv2D(rect);
    std::vector<cv::Vec6f> tri_list;
    subdiv.insert(landmarks);
    subdiv.getTriangleList(tri_list);

    std::vector<std::vector<int>> indeces;
    std::vector<cv::Point> pt(3);
    for (int i = 0; i < tri_list.size(); i++)
    {
        cv::Vec6f t = tri_list[i];
        pt[0] = cv::Point(cvRound(t[0]), cvRound(t[1]));
        pt[1] = cv::Point(cvRound(t[2]), cvRound(t[3]));
        pt[2] = cv::Point(cvRound(t[4]), cvRound(t[5]));
        std::vector<int> tri;
        for (int j = 0; j < 3; j++)
        {
            int index = find_point_index(pt[j], landmarks);
            if (index != -1)
                tri.push_back(index);
        }
        if (tri.size() == 3)
            indeces.push_back(tri);
    }

    return indeces;
}

int find_point_index(cv::Point point, std::vector<cv::Point2f>& points)
{
    for (int i = 0; i < points.size(); i++)
        if (abs(cvRound(points[i].x) - point.x) < 1.0 && abs(cvRound(points[i].y) - point.y) < 1.0)
            return i;

    return -1;
}

void make_equal_size(cv::Mat &img1, cv::Mat &img2, dlib::shape_predictor &predictor)
{
    int width1 = img1.cols, height1 = img1.rows;
    int width2 = img2.cols, height2 = img2.rows;
    float ratio1 = (float)width1 / (float)height1, ratio2 = (float)width2 / (float)height2;
    if (ratio1 == ratio2)
    {
        if (width1 == width2 && height1 == height2)
            return;
        else if (width1 >= width2 && height1 >= height2)
            cv::resize(img1, img1, cv::Size(width2, height2));
        else
            cv::resize(img2, img2, cv::Size(width1, height1));
    }
    else
    {
        crop_image(img1, predictor);
        crop_image(img2, predictor);
        make_equal_size(img1, img2, predictor);
    }
}

void crop_image(cv::Mat &img, dlib::shape_predictor &predictor)
{
    int width = img.cols, height = img.rows;
    int new_width = width > height ? height : width;
    int new_height = width > height ? height : width;
    dlib::rectangle face_rectangle = get_face_rectangle(img, predictor);
    if (new_width >= face_rectangle.width() && new_height >= face_rectangle.height())
    {
        int left_free_space = face_rectangle.tl_corner().x();
        int right_free_space = width - face_rectangle.tr_corner().x();
        int top_free_space = face_rectangle.tl_corner().y();
        int bottom_free_space = height - face_rectangle.bl_corner().y();

        int left_empty_space = (new_width - face_rectangle.width() + 1) / 2;
        int right_empty_space =  (new_width - face_rectangle.width() + 1) / 2;
        int top_empty_space = (new_height - face_rectangle.height() + 1) / 2;
        int bottom_empty_space = (new_height - face_rectangle.height() + 1) / 2;

        int x = face_rectangle.tl_corner().x() - left_empty_space;
        int y = face_rectangle.tl_corner().y() - top_empty_space;
        if (left_empty_space > left_free_space || right_empty_space > right_free_space
            || top_empty_space > top_free_space || bottom_empty_space > bottom_free_space)
        {
            if (left_empty_space > left_free_space || right_empty_space > right_free_space)
            {
                if (left_empty_space > left_free_space)
                    x = 0;
                else
                    x = face_rectangle.tl_corner().x() - left_empty_space - (right_empty_space - (new_width - face_rectangle.br_corner().x()));
            }
            if (top_empty_space > top_free_space || bottom_empty_space > bottom_free_space)
            {
                if (top_empty_space > top_free_space)
                    y = 0;
                else
                    y = face_rectangle.tl_corner().y() - top_empty_space - (bottom_empty_space - (new_height - face_rectangle.br_corner().y()));
            }
        }

        if (x + new_width > width)
            x -= (new_width + x - width);
        if (y + new_height > height)
            y -= (new_height + y - height);

        img = img(cv::Rect(x, y, new_width, new_height));
    }
    else
    {
        throw face_size_exception();
    }
}

dlib::rectangle get_face_rectangle(cv::Mat& img, dlib::shape_predictor& predictor)
{
    dlib::frontal_face_detector detector = dlib::get_frontal_face_detector();
    dlib::array2d<dlib::rgb_pixel> dlib_img;
    dlib::assign_image(dlib_img, dlib::cv_image<dlib::rgb_pixel>(img));
    std::vector<dlib::rectangle> dets = detector(dlib_img);
    if (dets.size() != 1)
        throw face_amount_exception(dets.size());

    return dets[0];
}

std::vector<cv::Point2f> get_landmarks_points(cv::Mat &img, dlib::shape_predictor &predictor)
{
    std::vector<cv::Point2f> landmarks_points;
    dlib::frontal_face_detector detector = dlib::get_frontal_face_detector();

    dlib::array2d<dlib::rgb_pixel> dlib_img;
    dlib::assign_image(dlib_img, dlib::cv_image<dlib::rgb_pixel>(img));
    std::vector<dlib::rectangle> dets = detector(dlib_img);
    if (dets.size() != 1)
        throw face_amount_exception(dets.size());
    dlib::full_object_detection shape = predictor(dlib_img, dets[0]);
    for (size_t i = 0; i < shape.num_parts(); i++)
        landmarks_points.push_back(cv::Point2f(shape.part(i).x(), shape.part(i).y()));
    landmarks_points.push_back(cv::Point2f(1, 1));
    landmarks_points.push_back(cv::Point2f(img.cols - 1, 1));
    landmarks_points.push_back(cv::Point2f(img.cols - 1, img.rows / 2));
    landmarks_points.push_back(cv::Point2f(img.cols - 1, img.rows - 1));
    landmarks_points.push_back(cv::Point2f(img.cols / 2, img.rows - 1));
    landmarks_points.push_back(cv::Point2f(1, img.rows - 1));
    landmarks_points.push_back(cv::Point2f(1, img.rows / 2));
    landmarks_points.push_back(cv::Point2f(img.cols / 2, 1));

    return landmarks_points;
}