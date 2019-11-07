//
// Created by mohamed on 11/6/19.
//

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;


int main() {

    Mat img;
    img = imread("test/giraffe.jpg", CV_LOAD_IMAGE_COLOR);   // Read the file



    if(! img.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << endl ;
        return -1;
    }


    int width = img.size().width;
    int height = img.size().height;

    Mat greyMat;
    cvtColor(img, greyMat, CV_BGR2GRAY);
    imwrite("./output/output.png",greyMat);

    namedWindow( "Display window", WINDOW_AUTOSIZE ); // Create a window for display.
    imshow( "Display window", greyMat );
    waitKey(0);

}