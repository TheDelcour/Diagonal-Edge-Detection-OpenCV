#include<iostream>
#include<cmath>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;


// computes the x component of the gradient vector at a given point in an image, then returns gradient in the x direction
int xGradient(Mat image, int x, int y)
{
	return image.at<uchar>(y - 1, x - 1) +
		2 * image.at<uchar>(y, x - 1) +
		image.at<uchar>(y + 1, x - 1) -
		image.at<uchar>(y - 1, x + 1) -
		2 * image.at<uchar>(y, x + 1) -
		image.at<uchar>(y + 1, x + 1);
}

// computes the y component of the gradient vector at a given point in an image, then returns gradient in the y direction

int yGradient(Mat image, int x, int y)
{
	return image.at<uchar>(y - 1, x - 1) +
		2 * image.at<uchar>(y - 1, x) +
		image.at<uchar>(y - 1, x + 1) -
		image.at<uchar>(y + 1, x - 1) -
		2 * image.at<uchar>(y + 1, x) -
		image.at<uchar>(y + 1, x + 1);
}

int main(){

	//initialise variables
	Mat image, imageC;
	int gx, gy, sum;
	int window[9];
	int value;
	int TH = 130;

	// load an image
	image = imread("C:/Users/Tobia/Desktop/rod/pic.jpg", CV_LOAD_IMAGE_GRAYSCALE);

	//create a clone of the image
	imageC = image.clone();

	//error check
	if (!image.data)
	{
		return -1;
	}

	for (int y = 2; y < image.rows - 2; y++){
		for (int x = 2; x < image.cols - 2; x++){

			// pick up window element using diagonal sobel kernel

			window[0] = imageC.at<uchar>(y, x + 2);
			window[1] = imageC.at<uchar>(y + 1, x + 1);
			window[2] = imageC.at<uchar>(y + 2, x);
			window[3] = imageC.at<uchar>(y - 1, x + 1);
			window[4] = imageC.at<uchar>(y, x);
			window[5] = imageC.at<uchar>(y + 1, x - 1);
			window[6] = imageC.at<uchar>(y - 2, x);
			window[7] = imageC.at<uchar>(y - 1, x - 1);
			window[8] = imageC.at<uchar>(y, x - 2);

			// find average of 3x3 kernel
			int sum = 0;
			for (int i = 0; i < 8; i++){
				sum += window[i];
			}

			// assign the average to centered element of the matrix
			imageC.at<uchar>(y, x) = sum / 9;
		}
	}

	imshow("after mean filter", imageC);

	//edge enhancement, using x & y gradient function above then takes the abs(the absolute value) of gy and gx and adds them together to define the gradient direction
	for (int y = 0; y < image.rows; y++)
		for (int x = 0; x < image.cols; x++)
			imageC.at<uchar>(y, x) = 0.0;

	for (int y = 1; y < image.rows - 1; y++){
		for (int x = 1; x < image.cols - 1; x++){
			gx = xGradient(image, x, y);
			gy = yGradient(image, x, y);
			sum = abs(gx) + abs(gy);

			//handle pixel values above 255 and below 0 
			if (sum > 255){
				sum = 255;
			}
			else{
				sum;
			}

			if (sum < 0){
				sum = 0;
			}
			else{
				sum;
			}

			imageC.at<uchar>(y, x) = sum;
		}
	}

	imshow("after edge enhancement", imageC);

	//edge localisation, value is the image and TH = threshold
	for (int y = 0; y < image.rows; ++y) {
		for (int x = 0; x < image.cols; ++x){
			value = imageC.at<uchar>(y, x);
			if (value >= TH){
				imageC.at<uchar>(y, x) = 255;
			}
			else{
				imageC.at<uchar>(y, x) = 0;
			}
		}
	}

	imshow("initial", image);
	imshow("final", imageC);

	waitKey();

	return 0;
}