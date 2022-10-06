// icp.cpp 
// Author: JJ

typedef struct s_globals {
    cv::VideoCapture capture;
} s_globals;

s_globals globals;

//=====================================================================================================

static void init(void)
{
    globals.capture = cv::VideoCapture(cv::CAP_DSHOW);

    if (!globals.capture.isOpened())
    { 
        std::cerr << "no camera" << std::endl;
        exit(EXIT_FAILURE);
    }
    else
    {
        std::cout << "Camera " << 
            ": width=" << globals.capture.get(cv::CAP_PROP_FRAME_WIDTH) <<
            ", height=" << globals.capture.get(cv::CAP_PROP_FRAME_HEIGHT) <<
            ", FPS=" << globals.capture.get(cv::CAP_PROP_FPS) << std::endl;
    }
}

int main(int argc, char * argv[])
{
    cv::Mat frame, scene;

    init();

    while (1)
    {
        globals.capture.read(frame);
        if (frame.empty())
        {
            std::cerr << "Cam disconnected?" << std::endl;
            break;
        }

        cv::imshow("grabbed", frame);

        // analyze the image...


        if (cv::waitKey(1) == 27)
            break;
    }

    if (globals.capture.isOpened())
        globals.capture.release();

    exit(EXIT_SUCCESS);
}

