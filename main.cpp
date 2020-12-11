#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include "tools/geometry_tools.cpp"
#include "tools/image_tools.cpp"

const char *usage_msg = "Usage: ./<binary_file_name> <file1_path> <file2_path> <shape_predictor.dat_path>";
const char *window_name = "Face Morphing";
const char *start_msg = "Hit any button to start!";
const char *finish_msg = "Hit any button to exit!";
const int fps = 24;

int main(int argc, const char * argv[])
{
    if (argc != 4)
    {
        std::cout << usage_msg << std::endl;
        return 0;
    }

    try
    {
        dlib::frontal_face_detector detector = dlib::get_frontal_face_detector();
        dlib::shape_predictor predictor;
        dlib::deserialize(argv[3]) >> predictor;
        cv::Mat img1, img2;
        img1 = cv::imread(argv[1]);
        img2 = cv::imread(argv[2]);
        check_image_size(img1);
        check_image_size(img2);
        make_equal_size(img1, img2, predictor);

        std::vector<cv::Point2f> landmarks1 = get_landmarks_points(img1, predictor);
        std::vector<cv::Point2f> landmarks2 = get_landmarks_points(img2, predictor);
        std::vector<std::vector<int>> tri_indeces_list = get_tri_list_indices(img1, landmarks1);

        img1.convertTo(img1, CV_32FC3);
        img2.convertTo(img2, CV_32FC3);

        std::vector<cv::Mat> frames;
        for (float t = 0.f; t <= 1.f; t += 0.02f)
        {
            std::vector<cv::Point2f> correspondences;
            cv::Mat morphed_img = cv::Mat::zeros(img1.size(), CV_32FC3);
            for (int i = 0; i < landmarks1.size(); i++)
            {
                float x = (1.f - t) * landmarks1[i].x + t * landmarks2[i].x;
                float y = (1.f - t) * landmarks1[i].y + t * landmarks2[i].y;
                correspondences.push_back(cv::Point2f(cvRound(x), cvRound(y)));
            }

            for (int i = 0; i < tri_indeces_list.size(); i++)
            {
                std::vector<cv::Point2f> mt, t1, t2;
                int x = tri_indeces_list[i][0];
                int y = tri_indeces_list[i][1];
                int z = tri_indeces_list[i][2];
                mt.push_back(correspondences[x]);
                mt.push_back(correspondences[y]);
                mt.push_back(correspondences[z]);
                t1.push_back(landmarks1[x]);
                t1.push_back(landmarks1[y]);
                t1.push_back(landmarks1[z]);
                t2.push_back(landmarks2[x]);
                t2.push_back(landmarks2[y]);
                t2.push_back(landmarks2[z]);
                morph_triangle(img1, img2, morphed_img, t1, t2, mt, t);
            }
            frames.push_back(morphed_img / 255.f);
        }

        cv::namedWindow(window_name, cv::WINDOW_AUTOSIZE);
        cv::imshow(window_name, img1 / 255.f);
        std::cout << start_msg << std::endl;
        cv::waitKey(0);
        for (auto frame : frames)
        {
            cv::imshow(window_name, frame);
            cv::waitKey(1000 / fps);
        }
        std::cout << finish_msg << std::endl;
        cv::waitKey(0);
        cv::destroyAllWindows();
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
        return -1;
    }

    return 0;
}