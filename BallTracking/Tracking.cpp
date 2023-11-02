#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <errno.h>
#include <wiringSerial.h>

using namespace cv;
using namespace std;

//Global value

//ukuran frame
const int FRAME_WIDTH = 320;
const int FRAME_HEIGHT = 280;

//inisialisasi nilai maks parameter threshold
int canny = 200;
int accumulator = 50;     
const int maxAccumulator = 200;
const int maxCanny = 255;

//inisialisasi nilai dan kontrol window HSV	
int iLowH = 0;
int iHighH = 100;
int iLowS = 0; 
int iHighS = 100;
int iLowV = 0;
int iHighV = 100;

Mat imgOriginal; Mat imgThresholded; Mat imgHSV;

int main( int argc, char** argv )
{

    //membuka kamera default
	VideoCapture cap(0);

    cap.set(CV_CAP_PROP_FRAME_WIDTH,FRAME_WIDTH);    //ukuran lebar gambar
    cap.set(CV_CAP_PROP_FRAME_HEIGHT,FRAME_HEIGHT);  //ukuran tinggi gambar

	//pengecekan
    if(!cap.isOpened())

    {
		cout << "Kamera tidak terdeteksi!!" << endl;
        return -1;
    }

	//membuat trackbar
    namedWindow("Control", CV_WINDOW_AUTOSIZE);

	createTrackbar("LowH", "Control", &iLowH, 360/2); //Hue (0 - 360/2)
	createTrackbar("H", "Control", &iHighH, 360/2);

	createTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
	createTrackbar("S", "Control", &iHighS, 255);

	createTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
	createTrackbar("V", "Control", &iHighV, 255);

	createTrackbar("Canny Threshold", "Control", &canny, maxCanny);
	createTrackbar("Accumulator Threshold", "Control", &accumulator, maxAccumulator);

    while (true)
    {
		//membaca hasil stream kamera
        bool bSuccess = cap.read(imgOriginal);
        if (!bSuccess)
        {
             cout << "tidak dapat melakukan stream" << endl;
             break;
        }

		//convert image BGR ke HSV
		cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV);

		//Threshold
		inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded);
		canny = std::max(canny, 1);
		accumulator = std::max(accumulator, 1);

	std::vector<Vec3f> circles;
	HoughCircles( imgThresholded, circles, HOUGH_GRADIENT, 1, imgThresholded.rows/8, canny, accumulator, 0, 0 );
	// clone the colour, input image for displaying purposes
	Mat display = imgOriginal.clone();
	for( size_t i = 0; i < circles.size(); i++ )
	{
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);
		// pusat lingkaran
		circle( display, center, 3, Scalar(0,255,0), -1, 8, 0 );
		// lingkaran luar
		circle( display, center, radius, Scalar(0,0,255), 3, 8, 0 );
		cout << "X:" << center.x  << endl;

		int target  = center.x;
		int serial = serialOpen("/dev/ttyACM0",9600);
		if (target == 0)
		{
				cout << "target tidak terdeteksi" <<endl;
		} 
		else if (target >= 200)
		{
			if((serial)<0)
			{
				cout << "Arduino tidak terdeteksi" <<endl;
			}
			else
			{
			    serialPrintf(serial,"2\n");
				serialClose(serial);
			}			
		}
		else if (target > 0 && target <=160)
		{
			if((serial)<0)
			{
				cout << "Arduino tidak terdeteksi" <<endl;
			}
			else
			{
			    serialPrintf(serial,"1\n");
				serialClose(serial);
			}
		} 
	}
		//menampilkan hasil
		imshow("Image Thresholded", imgThresholded); 
		imshow("hasil", display);

		//tekan esc untuk keluar
        if (waitKey(30) == 27) 
		{
			cout << "esc key is pressed by user" << endl;
			break; 
		}
    }
	return 0;
}

///////////////////-----------------------------------///////////////////







