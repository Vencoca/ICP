#include <iostream>
#include <numeric>
#include <opencv2/opencv.hpp>

void draw_cross_relative(cv::Mat& img, cv::Point2f center_relative, int size);
void draw_cross(cv::Mat& img, int x, int y, int size);
cv::Point2f find_center_HSV(cv::Mat& frame);


cv::Point2f find_center_HSV(cv::Mat& frame) {
    cv::Mat frame_hsv;
    cv::cvtColor(frame, frame_hsv, cv::COLOR_BGR2HSV);

    cv::Scalar lower_bound(25, 50, 50);
    cv::Scalar upper_bound(65, 255, 255);
    cv::Mat frame_treshold;
    cv::inRange(frame_hsv, lower_bound, upper_bound, frame_treshold);

    cv::namedWindow("frametr");
    cv::imshow("frametr", frame_treshold);

    std::vector<cv::Point> white_pixels;
    cv::findNonZero(frame_treshold, white_pixels);
    cv::Point white_reduced = std::reduce(white_pixels.begin(), white_pixels.end());

    cv::Point2f center_relative((float)white_reduced.x / white_pixels.size()/frame.cols, (float)white_reduced.y / white_pixels.size()/frame.rows);
    return center_relative;
}

typedef struct s_globals {
    cv::VideoCapture capture;
} s_globals;

s_globals globals;

int main()
{
    /*
    cv::Mat frame = cv::imread("resources/lightbulb.jpg");
    cv::Mat frame2;
    frame.copyTo(frame2);
    */

    cv::Mat frame;

    globals.capture = cv::VideoCapture(cv::CAP_DSHOW);
    if (!globals.capture.isOpened()) {
        std::cerr << "Faile :(" << "\n";
        exit(EXIT_FAILURE);
    }
    
    while (true) {
        globals.capture.read(frame);
        if (frame.empty()) {
            std::cerr << "device closed (or video at the end)" << "\n";
            break;
        }

        cv::Point2f center_relative = find_center_HSV(frame);
        std::cout << "stred" << center_relative << "\n";
        draw_cross_relative(frame, center_relative, 25);
        cv::namedWindow("frame");
        cv::imshow("frame", frame);

        cv::waitKey(1);
    }

    

    /*
    int sx = 0, sy = 0, sw = 0;
    for (int i = 0; i < frame.rows; i++) 
    {
        for (int j = 0; j < frame.cols; j++)
        {
            cv::Vec3b pixel = frame.at<cv::Vec3b>(i, j);
            unsigned char Y = 0.299 * pixel[2] + 0.587 * pixel[1] + 0.114 * pixel[0];

            if (Y < 228) {
                Y = 0;
            }
            else {
                Y = 255;
                sx += j;
                sy += i;
                sw++;

            }
            frame2.at<cv::Vec3b>(i, j) = cv::Vec3b(Y, Y, Y);
        }
    }

   

    cv::Point2i center((float)sx / sw / frame.cols, (float)sy / sw / frame.rows);
    cv::Point2f center_relative((float)center.x / frame.cols, (float)center.y / frame.rows);
    
    std::cout << "sted zarovky abs" << center << '\n';
    std::cout << "sted zarovky rel" << center_relative << '\n';std::cout << "Hello World!\n";
    std::cout << "sx-a-sd-a-sd-a-sd-a-s-d-a-sd-a" << (float)sx/sw/frame.cols << '\n';

    draw_cross_relative(frame, center_relative, 25);
    cv::namedWindow("frame");
    cv::imshow("frame", frame);
    cv::imshow("frame2", frame2);

    while (true)
    {
        
    }
    */
    return(EXIT_SUCCESS);
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