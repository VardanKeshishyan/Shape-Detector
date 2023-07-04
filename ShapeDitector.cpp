#include<opencv2/imgcodecs.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/objdetect.hpp>
#include<iostream>

using namespace std;
using namespace cv;

void getCotours(Mat imgDil, Mat img) {
	vector<vector<Point>>contours;
	vector<Vec4i>hierarchy;
	string objType;
	findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	drawContours(img, contours, -1, Scalar(255, 0, 255), 1);

	for (int i = 0; i < contours.size(); i++)
	{
		int area = contourArea(contours[i]);
		cout << area << endl;
		vector<vector<Point>>conPoly(contours.size());
		vector<Rect>boundRect(contours.size());

		if (area > 1000) {

			float peri = arcLength(contours[i], true);
			approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);
			drawContours(img, conPoly, i, Scalar(255, 0, 255), 2);
			cout << conPoly[i].size() << endl;
			boundRect[i] = boundingRect(conPoly[i]);

		//	rectangle(img, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 5);

			int objCor = (int)conPoly[i].size();
			if (objCor == 3) { objType = "Triangle"; }
			if (objCor == 4) { 
				float aspRatio = (float)boundRect[i].width / (float)boundRect[i].height;
				if (aspRatio > 0.8 && aspRatio < 1.2) { objType = "Square"; }
				else { objType = "Rectangle"; }
			}
			if (objCor == 5) { objType = "Pentagon"; }
			if (objCor == 6) { objType = "Hexagon"; }
			if (objCor > 6) { objType = "Circle"; }

			putText(img, objType, { boundRect[i].x, boundRect[i].y - 5 }, FONT_HERSHEY_PLAIN, 1, Scalar(111, 65, 255), 1);

		}
	}

}

void main() {
	string path = "photo1.jpg";
	Mat	img = imread(path);
	Mat imgGray, imgBlur, imgCanny, imgDil, imgErode;
	cvtColor(img, imgGray, COLOR_BGR2GRAY);
	GaussianBlur(imgGray, imgBlur, Size(7, 7), 5, 0);
	Canny(imgBlur, imgCanny, 25, 75);
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
	dilate(imgCanny, imgDil, kernel);
	getCotours(imgDil, img);

	imshow("Frame", img);

	waitKey(0);
}


