#include <iostream>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>

const char *shape_predictor_model_path = "shape_predictor_68_face_landmarks.dat";

int main(int argc, const char * argv[]) {
    if (argc < 2) {
        std::cout << "Usage: ./FaceMorphing <file1_path> <file2_path>\n";
        return 0;
    }

    try {
        dlib::frontal_face_detector detector = dlib::get_frontal_face_detector();
        dlib::shape_predictor predictor;
        dlib::deserialize(shape_predictor_model_path) >> predictor;
        dlib::image_window win, win_faces;
        for (size_t i = 1; i < argc; i++) {
            dlib::array2d<dlib::rgb_pixel> img;
            dlib::load_image(img, argv[i]);
            dlib::pyramid_up(img);
            std::vector<dlib::rectangle> dets = detector(img); //face amount
            //std::cout << face
        }

    } catch (std::exception e) {

    }
    return 0;
}
