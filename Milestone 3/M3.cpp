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

    if(size!=4){
        cout <<  "wrong number of cores , must be 4!" << endl ;
        return 0;
    }

    Mat img;
    Mat img2;
    Mat img3;
    Mat img4;

    bool done = false;

    //uncomment the line that has the name of the test image you want
    // Read the file
    img = imread("./test/Circle.jpg", CV_LOAD_IMAGE_COLOR);
    //img = imread("./test/Anime.jpg", CV_LOAD_IMAGE_COLOR);
    //img = imread("./test/giraffe.jpg", CV_LOAD_IMAGE_COLOR);

    if(! img.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << endl ;
        return -1;
    }


    int width = img.size().width;
    int height = img.size().height;

    //int MPI_Bcast ( void *buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm )

    img2.create(width / 2,height / 2,CV_8UC1);
   img3.create(width / 2,height / 2,CV_8UC1);
    img4.create(width / 2,height / 2,CV_8UC1);

    switch(rank)
    {
        case 0:{
            // first quarter
            Rect first(0, 0, width / 2, height / 2);
            Mat firstImage = img(first);

            Mat img1;
            cvtColor(firstImage, img1, CV_BGR2GRAY);

            printf("%s\n","before");
            MPI_Bcast(img.data,width*height*3,MPI_UNSIGNED_CHAR,0,MPI_COMM_WORLD);
            printf("%s","after");

            MPI_Bcast(img2.data,width*height/4,MPI_UNSIGNED_CHAR,1,MPI_COMM_WORLD);
            printf("%s","after 1");

            MPI_Bcast(img3.data,width*height/4,MPI_UNSIGNED_CHAR,2,MPI_COMM_WORLD);
            printf("%s","after 2");

            MPI_Bcast(img4.data,width*height/4,MPI_UNSIGNED_CHAR,3,MPI_COMM_WORLD);
            printf("%s","after 3");

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

            Mat Image;
            Image.create(width,height ,CV_8UC3);

            MPI_Bcast(Image.data,width*height*3,MPI_UNSIGNED_CHAR,0,MPI_COMM_WORLD);

            Rect second(width / 2, 0, width / 2, height / 2);
            Mat secondImage = Image(second);

            Mat greyMat;
            cvtColor(secondImage, img2, CV_BGR2GRAY);

            MPI_Bcast(img2.data,width*height/4,MPI_UNSIGNED_CHAR,1,MPI_COMM_WORLD);

            break;

        }
        case 2:{
            Mat Image;
            Image.create(width,height ,CV_8UC3);

            MPI_Bcast(Image.data,width*height*3,MPI_UNSIGNED_CHAR,0,MPI_COMM_WORLD);

            Rect third(0, height / 2, width / 2, height / 2);
            Mat thirdImage = Image(third);

            img3.create(width/2,height/2,CV_8UC1);

            Mat greyMat;
            cvtColor(thirdImage, img3, CV_BGR2GRAY);

            MPI_Bcast(img3.data,width*height/4,MPI_UNSIGNED_CHAR,2,MPI_COMM_WORLD);

            break;

        }
        case 3:{
            Mat Image;
            Image.create(width,height ,CV_8UC3);


            MPI_Bcast(Image.data,width*height*3,MPI_UNSIGNED_CHAR,0,MPI_COMM_WORLD);

            Rect fourth(width / 2, height / 2, width / 2, height / 2);
            Mat fourthImage = Image(fourth);

            img4.create(width/2,height/2,CV_8UC1);


            Mat greyMat;
            cvtColor(fourthImage, img4, CV_BGR2GRAY);

            MPI_Bcast(img4.data,width*height/4,MPI_UNSIGNED_CHAR,3,MPI_COMM_WORLD);
            printf("%s","after vwniucbq");

            break;

        }


    }

    rc = MPI_Finalize();

}