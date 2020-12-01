# FaceMorphing

# Start #

1. **Install dlib library**:
    1. Download the latest library [archive-release](https://github.com/davisking/dlib/release) 
    2. Extract it
    3. cd <archive directory>/dlib
    4. mkdir build
    5. cd build
    6. cmake . .
    7. make -j4
    8. sudo make install
2. **Install OpenCV library**:
    1. Install dependencies (maybe you'll need more. Shell'll recommend it if it's necessary):
sudo apt-get install python-dev python3-dev libgphoto2-dev python3-numpy python-numpy libv4l-dev libv4l2rds0 libgtk-3-dev libgstreamer-plugins-base1.0-dev libgst-dev cmake-qt-gui build-essential cmake git libgtk2.0-dev pkg-config libavcodec-dev libswscale-dev libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libjasper-dev libdc1394-22-dev libgstreamer-vaapi1.0-dev libavformat-dev libavformat-ffmpeg-dev -y
    2. Download the latest library [archive-release](https://github.com/opencv/opencv/releases)
    3. Download the latest contrib library [archive-release](https://github.com/opencv/opencv_contrib/releases)
    4. Extract archives in one directory
    5. cd opencv<version> && mkdir build && cd build && cmake-gui
    6. Select opencv<version> directory for source code directory
    7. Select opencv<version>/build directory for build the binaries
    8. Press "Configure" and "Finish"
    9. Type in "extra" in the Search field. The OPENCV_EXTRA_MODULES_.. must be shown. Change it's value to opencv<version>-contrib/modules
    10. Press "Configure"
    11. Close the window
    12. make -j4
    13. sudo make install
3. **Run Face Morphing**:
    1. git clone https://github.com/atommaks/FaceMorphing.git to your directory
    2. cd to <project_directory>
    3. cmake .
    4. replace default CMakeLists.txt with my CMakeListst.txt
    5. make
    6. ./main args..

# Usage #
./main <img1_path> <img2_path> <shape_predictor_file>

# Extra #
Shape predictor file can be downloaded [here](https://github.com/atommaks/FaceMorphing/blob/master/shape_predictor_68_face_landmarks.dat)
