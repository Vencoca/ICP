void draw_cross( cv::Mat& img, int x, int y, int size )
{
    cv::Point p1( x-size/2, y );
    cv::Point p2( x+size/2, y );
    cv::Point p3( x, y-size/2 );
    cv::Point p4( x, y+size/2 );

    cv::line( img, p1, p2, CV_RGB(255,0,0), 3, CV_AA );
    cv::line( img, p3, p4, CV_RGB(255,0,0), 3, CV_AA );
}

void draw_cross_relative(cv::Mat& img, cv::Point2f center_relative, int size)
{
    center_relative.x = std::clamp(center_relative.x, 0.0f, 1.0f);
    center_relative.y = std::clamp(center_relative.y, 0.0f, 1.0f);
    size              = std::clamp(size, 1, std::min(img.cols, img.rows));

    cv::Point2f center_absolute(center_relative.x * img.cols, center_relative.y * img.rows);

    cv::Point2f p1(center_absolute.x - size / 2, center_absolute.y);
    cv::Point2f p2(center_absolute.x + size / 2, center_absolute.y);
    cv::Point2f p3(center_absolute.x, center_absolute.y - size / 2);
    cv::Point2f p4(center_absolute.x, center_absolute.y + size / 2);

    cv::line(img, p1, p2, CV_RGB(255, 0, 0), 3);
    cv::line(img, p3, p4, CV_RGB(255, 0, 0), 3);
}
