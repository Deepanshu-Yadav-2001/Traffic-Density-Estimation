#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <iostream>
#include "opencv2/opencv.hpp"
#include <fstream>
using namespace cv;
using namespace std;

struct  queue_density_struc
{
    Mat frame;
    Mat empt;
    double q_density;
};

int threshold_value1 = 40;
int threshold_value2 = 10;
int threshold_type = 3;
int const max_value = 255;
int const max_type = 4;
int const max_binary_value = 255;
vector<Point2f> pts_src = { {800,244},{620,725},{1450,730},{1400,230} } ;
vector<Point2f> pts_dst = { {472,52},{472,830},{800,830},{800,52} } ;
Mat h1 = getPerspectiveTransform(pts_src, pts_dst);

void* queue_density(void* f)
{
    struct queue_density_struc *f_struc = (struct queue_density_struc *) f ;
    
    double q_d = 0.0;
    Mat empty = f_struc->empt;
    Mat gray_f = f_struc->frame ;
    cvtColor(gray_f, gray_f, COLOR_BGR2GRAY);
    GaussianBlur(gray_f, gray_f, Size(5, 5), 0);
    Mat diff;
    absdiff(gray_f, empty, diff);
    Mat im1;
    threshold( diff , im1, threshold_value1, max_binary_value, THRESH_BINARY );	
    
    Mat imout1;
    warpPerspective(im1,imout1,h1, empty.size());
    Mat crop1 = im1(Rect(460,52,340,738));
    int TotalNumberOfPixels1 = crop1.rows * crop1.cols;
	int nzp1 = countNonZero(crop1);
    q_d = (double) nzp1 / (double) TotalNumberOfPixels1 ;
    
    f_struc->q_density = q_d ;
    pthread_exit(0);

}

int main(int argc, char** argv)
{
    string x;
    cout<<"enter the number of threads: ";
    cin>>x;
    string flname = x +"mtd4queue.txt"  ;
    ofstream outFile;
    outFile.open(flname);
    int no_of_threads = stoi(x);
    time_t start, end;
    
    time(&start);
    // if (argc < 2)
    // {
    //     printf("invalid arguments");
    //     exit(-1);
    // }
    string z = ".mp4";
    VideoCapture vid(argv[1]+z);
    Mat fr;
    Mat empt =  imread("empty.jpg");
    cvtColor(empt, empt, COLOR_BGR2GRAY);

    struct queue_density_struc frames[no_of_threads] ;
    pthread_t tids[no_of_threads];
    bool loop = true;
    double time_tot = 0.0;
    while(loop){
        for(int i=0; i<no_of_threads;i++){
            vid >> fr; vid >> fr ; vid >> fr ; vid >> fr ; vid >> fr ;
            if(fr.empty()){
                loop = false;
                break;
            }
            frames[i].frame = fr;
            frames[i].empt = empt ;

            pthread_attr_t attr;
            pthread_attr_init(&attr);
            pthread_create(&tids[i], &attr, queue_density, &frames[i]);
        }
        for(int i=0;i < no_of_threads ; i++){
            time_tot += (1.0/3.0);
            pthread_join(tids[i], NULL);
            ofstream file;
            file.open(flname,ios::app);
            file << time_tot << ","<< frames[i].q_density << '\n';
        }

    }
    time(&end);
    double time_taken = double(end - start);
    cout << "Time taken by program is : " << fixed 
         << time_taken << setprecision(5);
    cout << " sec " << endl;

}
