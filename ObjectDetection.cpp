#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
using namespace cv;
using namespace std;

VideoCapture cam(0);
Mat img, imgGrey;
int thresh = 255;
// void getContour(Mat imgGrey, Mat img)
// {
//     vector<vector<Point>> contours;
//     vector<Vec4i> hierarchy;

//     findContours(imgGrey, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

//     vector<vector<Point>> conPoly(contours.size());

//     for (int i=0; i < contours.size(); i++)
//     {
//         int area = contourArea(contours[i]);

//         if (area > 1000){
//             approxPolyDP(contours[i], conPoly[i], 3, true);
//             if (conPoly[i].size() > 10)
//             {
//                 drawContours(img, conPoly,i, Scalar(0,0,255), 2);
//             }
//         }
//     }
// }
void thresh_callback(int, void* );

int main( int argc, char** argv )
{
    int Hue_Lower_Value = 12;//initial hue value(lower)//
    int Hue_Lower_Upper_Value = 24;//initial hue value(upper)//
    int Saturation_Lower_Value = 91;//initial saturation(lower)//
    int Saturation_Upper_Value = 190;//initial saturation(upper)//
    int Value_Lower = 243;//initial value (lower)//
    int Value_Upper = 255;//initial saturation(upper)//
    Mat HSV_img;
    
    
    while (true) {
        cam.read(img);
        cvtColor(img, HSV_img, COLOR_BGR2HSV);
        inRange(HSV_img,Scalar(Hue_Lower_Value,Saturation_Lower_Value, Value_Lower),Scalar(Hue_Lower_Upper_Value,Saturation_Upper_Value, Value_Upper), HSV_img);
        
        blur(HSV_img, imgGrey, Size(3, 3));
        // Canny(imgGrey,imgGrey, 25, 75);
        // dilate(imgGrey, imgGrey, kernel);

        // imshow("Camera", img);
        // imshow("Adjust", HSV_img);
        
        thresh_callback(0, 0);
        waitKey(1);
    }

    return 0;
}

void thresh_callback(int, void* )
{
    Mat canny_output;
    Canny( imgGrey, canny_output, thresh, thresh*2 );
    vector<vector<Point> > contours;
    findContours( canny_output, contours, RETR_TREE, CHAIN_APPROX_SIMPLE );
    vector<vector<Point> > contours_poly( contours.size() );
    vector<Rect> boundRect( contours.size() );
    vector<Point2f>centers( contours.size() );
    vector<float>radius( contours.size() );

    int maxR = 0;
    for( size_t i = 0; i < contours.size(); i++ )
    {
        approxPolyDP( contours[i], contours_poly[i], 3, true );
        boundRect[i] = boundingRect( contours_poly[i] );
        minEnclosingCircle( contours_poly[i], centers[i], radius[i] );
        
        if (radius[i] > radius[maxR]){
            maxR = i;
        }
    }
    Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
    for( size_t i = 0; i< contours.size(); i++ )
    {
        if (i == maxR){
        Scalar color = Scalar( 0, 0, 256 );
        drawContours( img, contours_poly, (int)i, color );
        rectangle( img, boundRect[i].tl(), boundRect[i].br(), color, 2 );
        circle( img, centers[i], (int)radius[i], color, 2 );
        }
    }
    imshow( "Contours", img );
}