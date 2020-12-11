#include "image_tools.h"

struct less_point: std::binary_function<const cv::Point2f*, const cv::Point2f*, bool>
{
    bool operator() (const cv::Point2f *a, const cv::Point2f *b) const
    {
        //int x1 = cvRound(a->x), y1 = cvRound(a->y);
        //int x2 = cvRound(b->x), y2 = cvRound(b->y);
        int x1 = a->x, y1 = a->y;
        int x2 = b->x, y2 = b->y;
        return (x1 < x2) || (x1 == x2 && y1 < y2);
    }
};

std::vector<std::vector<int>> get_tri_list_indices(cv::Mat &img, std::vector<cv::Point2f> &landmarks)
{
    cv::Rect rect = cv::Rect(0, 0, img.size().width, img.size().height);
    cv::Subdiv2D subdiv = cv::Subdiv2D(rect);
    std::vector<cv::Vec6f> tri_list;
    subdiv.insert(landmarks);
    subdiv.getTriangleList(tri_list);

    std::map<cv::Point2f*, int, less_point> map;
    for (int i = 0; i < landmarks.size(); i++)
        map[&landmarks[i]] = i;

    std::vector<std::vector<int>> indeces;
    for (int i = 0; i < tri_list.size(); i++)
    {
        std::vector<int> tri;
        for (int j = 0; j < 3; j++)
        {
            cv::Point2f p = cv::Point2f(cvRound(tri_list[i][j * 2]), cvRound(tri_list[i][j * 2 + 1]));
            tri.push_back(map[&p]);
        }
        indeces.push_back(tri);
    }

    return indeces;
}

void check_image_size(cv::Mat &img)
{
    const unsigned int MAX_WIDTH = 1920, MAX_HEIGHT = 1080;
    const float divder = 2.f;
    float width = (float)img.cols, height = (float)img.rows;
    while (width > MAX_WIDTH && height > MAX_HEIGHT)
    {
        width /= divder;
        height /= divder;
    }
    if ((int)width != img.cols || (int)height != img.rows)
        cv::resize(img, img, cv::Size((int)width, (int)height));
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

dlib::rectangle get_face_rectangle(cv::Mat &img, dlib::shape_predictor &predictor)
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