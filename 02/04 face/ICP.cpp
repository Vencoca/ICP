// icp.cpp 
// Author: JJ

//...

int main(int argc, char* argv[])
{
	// load clasifier
	cv::CascadeClassifier face_cascade = cv::CascadeClassifier("resources/haarcascade_frontalface_default.xml");

	init(); //open camera

	cv::Mat frame;
	cv::Point2f center;
	do {
		
		globals.capture.read(frame);
		if (frame.empty())
		{
			std::cerr << "Cam disconnected?" << std::endl;
			break;
		}

		auto start = std::chrono::system_clock::now();

		// find face
		cv::Mat scene_gray;
		cv::cvtColor(frame, scene_gray, cv::COLOR_BGR2GRAY);
		std::vector<cv::Rect> faces;
		face_cascade.detectMultiScale(scene_gray, faces);
		if (faces.size() > 0)
		{
			center.x = (faces[0].x + (faces[0].width / 2.0f))/frame.cols;
			center.y = (faces[0].y + (faces[0].height / 2.0f))/frame.rows;
		}

		auto end = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed_seconds = end - start;
		std::cout << "elapsed time: " << elapsed_seconds.count() << " sec" << std::endl;
		std::cout << "found relative: " << center << std::endl;

		//display result
		cv::Mat scene_cross;
		frame.copyTo(scene_cross);
		draw_cross_relative(scene_cross, center, 30);
		cv::imshow("scene", scene_cross);

		
	} while (cv::waitKey(1) != 27); //message loop with 1ms delay untill ESC

	exit(EXIT_SUCCESS);
}
