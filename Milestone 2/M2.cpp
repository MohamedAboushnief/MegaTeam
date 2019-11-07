#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <opencv2/highgui/highgui.hpp>
#include <mpi.h>

using namespace cv;
using namespace std;

int main() {

    int rank, size, tag, rc, i;
    MPI_Status status;
    rc = MPI_Init(NULL, NULL);
    rc = MPI_Comm_size(MPI_COMM_WORLD, &size);
    rc = MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if(size!=4){
        cout <<  "wrong number of cores , must be 4!" << endl ;
        return 0;
    }

    Mat img;
    Mat img2;
    Mat img3;
    Mat img4;

    //uncomment the line that has the name of the test image you want

    //img = imread("./test/dog.jpg", CV_LOAD_IMAGE_COLOR);
    //img = imread("./test/eagle.jpg", CV_LOAD_IMAGE_COLOR);
    img = imread("./test/giraffe.jpg", CV_LOAD_IMAGE_COLOR);   // Read the file

    if(! img.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << endl ;
        return -1;
    }


    int width = img.size().width;
    int height = img.size().height;


    switch(rank)
    {
        case 0:{
            // first quarter
            Rect first(0, 0, width / 2, height / 2);
            Mat firstImage = img(first);

            Mat img1;
            cvtColor(firstImage, img1, CV_BGR2GRAY);

            img2.create(width / 2,height / 2,CV_8UC1);
            img3.create(width / 2,height / 2,CV_8UC1);
            img4.create(width / 2,height / 2,CV_8UC1);


            rc = MPI_Recv(img2.data, width*height*1/4, MPI_UNSIGNED_CHAR, 1, 0, MPI_COMM_WORLD, &status);
            rc = MPI_Recv(img3.data, width*height*1/4, MPI_UNSIGNED_CHAR, 2, 0, MPI_COMM_WORLD, &status);
            rc = MPI_Recv(img4.data, width*height*1/4, MPI_UNSIGNED_CHAR, 3, 0, MPI_COMM_WORLD, &status);

            Mat horizontal1;
            Mat horizontal2;
            Mat final;

            hconcat(img1,img2,horizontal1);
            hconcat(img3,img4,horizontal2);

            vconcat(horizontal1,horizontal2,final);

            imwrite("./output/output.png",final);


            namedWindow( "Display window", WINDOW_AUTOSIZE ); // Create a window for display.
            imshow( "Display window", final );
            waitKey(0);

        }
        case 1:{
            Rect second(width / 2, 0, width / 2, height / 2);
            Mat secondImage = img(second);

            Mat greyMat;
            cvtColor(secondImage, greyMat, CV_BGR2GRAY);

            rc = MPI_Send(greyMat.data, width*height*1/4, MPI_UNSIGNED_CHAR, 0, 0, MPI_COMM_WORLD);
            break;

        }
        case 2:{
            Rect third(0, height / 2, width / 2, height / 2);
            Mat thirdImage = img(third);

            Mat greyMat;
            cvtColor(thirdImage, greyMat, CV_BGR2GRAY);

            rc = MPI_Send(greyMat.data, width*height*1/4, MPI_UNSIGNED_CHAR, 0, 0, MPI_COMM_WORLD);
            break;

        }
        case 3:{
            Rect fourth(width / 2, height / 2, width / 2, height / 2);
            Mat fourthImage = img(fourth);

            Mat greyMat;
            cvtColor(fourthImage, greyMat, CV_BGR2GRAY);

            rc = MPI_Send(greyMat.data, width*height*1/4, MPI_UNSIGNED_CHAR, 0, 0, MPI_COMM_WORLD);
            break;

        }


    }

    rc = MPI_Finalize();

}