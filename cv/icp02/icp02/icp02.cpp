#include <iostream>
#include <numeric>
#include <thread>
#include <opencv2/opencv.hpp>

void draw_cross_relative(cv::Mat& img, cv::Point2f center_relative, int size);
void draw_cross(cv::Mat& img, int x, int y, int size);
void img_process_code();
cv::Point2f find_center_HSV(cv::Mat& frame);

struct img_data_s {
    cv::Point2f center_relative;
    cv::Mat frame;
};

typedef struct s_globals {
    cv::VideoCapture capture;
} s_globals;

s_globals globals;
std::unique_ptr<img_data_s> img_data_p;
std::mutex my_mutex;

int main(){
    
    cv::Mat frame;
    globals.capture = cv::VideoCapture(cv::CAP_DSHOW);
    std::unique_ptr<img_data_s> img_data_local;
    cv::namedWindow("frame");
    bool new_data = false;
    std::thread img_thread(img_process_code);
    while (globals.capture.isOpened()) {
        my_mutex.lock();
        if (img_data_p && !img_data_p->frame.empty()) {
            img_data_local = std::move(img_data_p);
            new_data = true;
        }
        else {
            new_data = false;
        }
        my_mutex.unlock();
        if (new_data) {
            std::cout << "sted zarovky relativne" << img_data_local->center_relative << "\n";
            draw_cross_relative(img_data_local->frame, img_data_local->center_relative, 25);
            cv::imshow("frame", img_data_local->frame);
        }
        else {
            std::cout << ".";
        }
        cv::waitKey(16);
    }
    return(EXIT_SUCCESS);
}

void img_process_code() {
    cv::Mat frame;
    while (globals.capture.isOpened()) {
        globals.capture.read(frame);
        if (frame.empty()) {
            std::cerr << "device closed" << "\n";
            globals.capture.release();
            break;
        }
        //create new result
        std::unique_ptr<img_data_s> img_data_local = std::make_unique<img_data_s>();
        //compute new result
        img_data_local->center_relative = find_center_HSV(frame);
        frame.copyTo(img_data_local->frame);

        //synced send
        my_mutex.lock();
        img_data_p = std::move(img_data_local);
        my_mutex.unlock();
        //simulate 10 FPS
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void draw_cross(cv::Mat& img, int x, int y, int size)
{
    cv::Point p1(x - size / 2, y);
    cv::Point p2(x + size / 2, y);
    cv::Point p3(x, y - size / 2);
    cv::Point p4(x, y + size / 2);

    cv::line(img, p1, p2, CV_RGB(255, 0, 0), 3);
    cv::line(img, p3, p4, CV_RGB(255, 0, 0), 3);
}

void draw_cross_relative(cv::Mat& img, cv::Point2f center_relative, int size)
{
    center_relative.x = std::clamp(center_relative.x, 0.0f, 1.0f);
    center_relative.y = std::clamp(center_relative.y, 0.0f, 1.0f);
    size = std::clamp(size, 1, std::min(img.cols, img.rows));

    cv::Point2f center_absolute(center_relative.x * img.cols, center_relative.y * img.rows);

    cv::Point2f p1(center_absolute.x - size / 2, center_absolute.y);
    cv::Point2f p2(center_absolute.x + size / 2, center_absolute.y);
    cv::Point2f p3(center_absolute.x, center_absolute.y - size / 2);
    cv::Point2f p4(center_absolute.x, center_absolute.y + size / 2);

    cv::line(img, p1, p2, CV_RGB(255, 0, 0), 3);
    cv::line(img, p3, p4, CV_RGB(255, 0, 0), 3);
}

cv::Point2f find_center_HSV(cv::Mat& frame) {
    cv::Mat frame_hsv;
    cv::cvtColor(frame, frame_hsv, cv::COLOR_BGR2HSV);

    cv::Scalar lower_bound(55, 50, 50);
    cv::Scalar upper_bound(65, 255, 255);
    cv::Mat frame_treshold;
    cv::inRange(frame_hsv, lower_bound, upper_bound, frame_treshold);

    //cv::namedWindow("frametr");
    //cv::imshow("frametr", frame_treshold);

    std::vector<cv::Point> white_pixels;
    cv::findNonZero(frame_treshold, white_pixels);
    cv::Point white_reduced = std::reduce(white_pixels.begin(), white_pixels.end());

    cv::Point2f center_relative((float)white_reduced.x / white_pixels.size() / frame.cols, (float)white_reduced.y / white_pixels.size() / frame.rows);
    return center_relative;
}