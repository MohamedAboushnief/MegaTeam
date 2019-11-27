#include <unistd.h>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <opencv2/highgui/highgui.hpp>
#include <mpi.h>

using namespace cv;
using namespace std;

int main() {

    int rank, size, tag, rc, i; //1
    MPI_Status status;//1
    rc = MPI_Init(NULL, NULL);//1
    rc = MPI_Comm_size(MPI_COMM_WORLD, &size);
    rc = MPI_Comm_rank(MPI_COMM_WORLD, &rank);


    Mat img;
    int width;
    int height;
    unsigned char *buffer;
    unsigned char *finalBuffer;

    if(rank==0){
        img = imread("./test/Circle.jpg", CV_LOAD_IMAGE_COLOR);
        width = img.cols;
        height = img.rows;

        MPI_Bcast(&width, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&height, 1, MPI_INT, 0, MPI_COMM_WORLD);

        buffer=(unsigned char*) malloc(sizeof(unsigned char)* (width * height * 3)/size);
        MPI_Scatter(img.data, (width * height * 3)/size, MPI_UNSIGNED_CHAR,  buffer, (width * height * 3)/size, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);

    } else {
        MPI_Bcast(&width, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&height, 1, MPI_INT, 0, MPI_COMM_WORLD);

        buffer = (unsigned char *) malloc(sizeof(unsigned char) * (width * height * 3) / size);
        MPI_Scatter(img.data, (width * height * 3) / size, MPI_UNSIGNED_CHAR, buffer, (width * height * 3) / size,
                    MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
    }


    Mat partImage = Mat(height/size, width, CV_8UC3, buffer);
    Mat greyMat;
    cvtColor(partImage, greyMat, CV_BGR2GRAY);

    if(rank == 0){
        finalBuffer = (unsigned char *) malloc(sizeof(unsigned char) * width * height);
    }

    MPI_Gather(greyMat.data, partImage.rows * partImage.cols, MPI_UNSIGNED_CHAR, finalBuffer, partImage.rows * partImage.cols, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);

    if(rank ==0){
        Mat finalImage = Mat(height, width, CV_8UC1, finalBuffer);
        imwrite("./output/output.png",finalImage);
//        namedWindow( "Display window", WINDOW_AUTOSIZE ); // Create a window for display.
//        imshow( "Display window", finalImage );
//        waitKey(0);
    }


    rc = MPI_Finalize();

}