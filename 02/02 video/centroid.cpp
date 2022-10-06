// shorter code 
#include <numeric>


 //generate threshhold image
	cv::cvtColor(scene, scene_hsv, cv::COLOR_BGR2HSV);

	// HSV ranges between (0-179, 0-255, 0-255).
	cv::Scalar lower_threshold = cv::Scalar( ,  , );
	cv::Scalar upper_threshold = cv::Scalar( ,  , );
	cv::Mat scene_threshold;
	cv::inRange(scene_hsv, lower_threshold, upper_threshold, scene_threshold);
 
 //find centroid 
    std::vector<cv::Point> whitePixels;
    cv::findNonZero(scene, whitePixels);
    size_t cnt_white = whitePixels.size();
    cv::Point acc_white = std::reduce(whitePixels.begin(), whitePixels.end());

    if (cnt_white > 0)
        average = acc_white / cnt_white;
    else
        average = { 0,0 };

...
