void draw_box( cv::Mat& img, int top_left_x, int top_left_y, int size )
{
	cv::Point p1 = cv::Point( top_left_x, top_left_y );
	cv::Point p2 = cv::Point( top_left_x+size, top_left_y );
	cv::Point p3 = cv::Point( top_left_x+size, top_left_y+size );
	cv::Point p4 = cv::Point( top_left_x, top_left_y+size );

	cv::line( img, p1, p2, CV_RGB(0,255,0), 2, CV_AA );
	cv::line( img, p2, p3, CV_RGB(0,255,0), 2, CV_AA );
	cv::line( img, p3, p4, CV_RGB(0,255,0), 2, CV_AA );
	cv::line( img, p4, p1, CV_RGB(0,255,0), 2, CV_AA );
}

