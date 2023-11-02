#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

//Global value

//ukuran frame
const int FRAME_WIDTH = 320;
const int FRAME_HEIGHT = 280;

//inisialisasi nilai maks parameter threshold image biner
int ThresholdValue = 50;
const int maxThresholdValue = 255;

//inisialisasi nilai maks parameter threshold
int thresh = 100;
int max_thresh = 255;

Mat imgOriginal; Mat imgGray;

/// Function header
void thresh_callback(int, void* );

/** @function main */
int main( int argc, char** argv )
{
    VideoCapture cap(0); //akses kamera webcam pada video0 (default)
	
    cap.set(CV_CAP_PROP_FRAME_WIDTH,FRAME_WIDTH);    //ukuran lebar gambar
    cap.set(CV_CAP_PROP_FRAME_HEIGHT,FRAME_HEIGHT);  //ukuran tinggi gambar

    //membuka kamera default
	VideoCapture cap(0);
	//pengecekan
    if(!cap.isOpened())
    {
		cout << "Kamera tidak terdeteksi!!" << endl;
        return -1;
    }
	
	//membuat trackbar
	namedWindow("source", CV_WINDOW_AUTOSIZE );
	createTrackbar("Threshold", "source", &Thresholdvalue, maxThresholdValue);
	createTrackbar("acumulator", "source", &thresh, max_thresh, thresh_callback );
	
	 while (true)
    {
		//membaca hasil stream kamera
        bool bSuccess = cap.read(imgOriginal);
        if (!bSuccess)
        {
             cout << "tidak dapat melakukan stream" << endl;
             break;
        }
		// Convert image to gray blur
		cvtColor( imgOriginal, imgGray, CV_BGR2GRAY );
		//blur image
		blur( imgGray, imgGray, Size(3,3) );
		
		//Threshold
		inRange(imgGray, Thresholdvalue, maxThresholdValue, imgGray);
	
		//menampilkan hasil
		imshow("source", imgOriginal );
		
		thresh_callback( 0, 0 );
		
		//tekan esc untuk keluar
        if (waitKey(30) == 27) 
		{
			cout << "esc key is pressed by user" << endl;
			break; 
		}
	}
	return(0);
}


/** @function thresh_callback */
void thresh_callback(int, void* )
{
  Mat threshold_output;
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;

  /// Detect edges using Threshold
  threshold( imgGray, threshold_output, thresh, 255, THRESH_BINARY );
  /// Find contours
  findContours( threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

  /// Find the rotated rectangles and ellipses for each contour
  vector<RotatedRect> minEllipse( contours.size() );

  for( int i = 0; i < contours.size(); i++ )
     { minRect[i] = minAreaRect( Mat(contours[i]) );
       if( contours[i].size() > 5 )
         { minEllipse[i] = fitEllipse( Mat(contours[i]) ); }
     }

  /// Draw contours + rotated rects + ellipses
  Mat drawing = Mat::zeros( threshold_output.size(), CV_8UC3 );
  for( int i = 0; i< contours.size(); i++ )
     {
       drawContours( drawing, contours, i, Scalar(255,255,255), 1, 8, vector<Vec4i>(), 0, Point() );
       // ellipse
       ellipse( imgOriginal, minEllipse[i], Scalar(0,0,255), 2, 8 );
     }

  /// Show in a window
  imshow( "Contours", drawing );
  imshow( "Threshold", imgGray );
}