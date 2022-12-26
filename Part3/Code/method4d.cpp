#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <iostream>
#include "opencv2/opencv.hpp"
#include <fstream>
using namespace cv;
using namespace std;

struct  dynamic_density_struc
{
    Mat next_ ;
    Mat previous;
    double d_density;
};

bool loop = true;
int threshold_value2 = 10;
int threshold_type = 3;
int const max_value = 255;
int const max_type = 4;
int const max_binary_value = 255;
vector<Point2f> pts_src = { {800,244},{620,725},{1450,730},{1400,230} } ;
vector<Point2f> pts_dst = { {472,52},{472,830},{800,830},{800,52} } ;
Mat h2 = getPerspectiveTransform(pts_src, pts_dst);


void* dynamic_density(void* f)
{
    struct dynamic_density_struc *f_struc = (struct dynamic_density_struc *) f ;
        Mat prev = f_struc->previous;
        Mat next = f_struc->next_ ;
        Mat flow(prev.size(), CV_32FC2);
        calcOpticalFlowFarneback(prev, next, flow, 0.5, 3, 15, 3, 5, 1.2, 0);

        // visualization
        Mat flow_parts[2];
        split(flow, flow_parts);
        Mat magnitude, angle, magn_norm;
        cartToPolar(flow_parts[0], flow_parts[1], magnitude, angle, true);
        normalize(magnitude, magn_norm, 0.0f, 1.0f, NORM_MINMAX);
        angle *= ((1.f / 360.f) * (180.f / 255.f));
        //build hsv image
        Mat _hsv[3], hsv, hsv8, bgr;
        _hsv[0] = angle;
        _hsv[1] = Mat::ones(angle.size(), CV_32F);
        _hsv[2] = magn_norm;
        merge(_hsv, 3, hsv);
        hsv.convertTo(hsv8, CV_8U, 255.0);
        cvtColor(hsv8, bgr, COLOR_HSV2BGR);
        Mat gr;
        cvtColor(bgr, gr, COLOR_BGR2GRAY);

        Mat im2;
        threshold( gr , im2, threshold_value2, max_binary_value, THRESH_BINARY );
        Mat imout2;
   	    warpPerspective(im2,imout2,h2,im2.size());
   
   	    Mat crop2 = imout2(Rect(460,52,340,738));
        int TotalNumberOfPixels2 = crop2.rows * crop2.cols;
	    int nzp2 = countNonZero(crop2);
        double density2;
        density2 = (double) nzp2 / (double) TotalNumberOfPixels2 ;

        f_struc->d_density = density2;

    
    pthread_exit(0);

}

int main(int argc, char** argv)
{
    time_t start, end;
    
    time(&start);
    string x;
    cout<<"enter the number of threads: ";
    cin>>x;
    string flname = x +"mtd4dynamic.txt"  ;
    ofstream outFile;
    outFile.open(flname);
    // if (argc < 2)
    // {
    //     printf("invalid arguments");
    //     exit(-1);
    // }
    int no_of_threads = stoi(x);
    string z = ".mp4";
    VideoCapture vid(argv[1]+z);

    long frame_Number = vid.get(CAP_PROP_FRAME_COUNT);
    //cout << frame_Number << '\n';

    struct dynamic_density_struc frames[no_of_threads] ;
    pthread_t tids[no_of_threads];
    Mat fr;
    vid >> fr;
    cvtColor(fr, fr, COLOR_BGR2GRAY);
    GaussianBlur(fr , fr, Size(5, 5), 0);
    double time_tot = 0.0;
    while(loop){
        Mat pr = fr; 
        for(int i=0; i < no_of_threads;i++){
            frames[i].previous = pr;
            vid>>fr;vid>>fr;vid>>fr;vid>>fr;vid>>fr;
            if(fr.empty()){
                loop = false;
                break;
            }
            cvtColor(fr, fr, COLOR_BGR2GRAY);
            GaussianBlur(fr , fr, Size(5, 5), 0);
            frames[i].next_ = fr;
            pr = fr;
            pthread_attr_t attr;
            pthread_attr_init(&attr);
            pthread_create(&tids[i], &attr, dynamic_density, &frames[i]);
        }
        for(int i=0;i < no_of_threads ; i++){
            pthread_join(tids[i], NULL);
                time_tot += (1.0/3.0);
                ofstream file;
                file.open(flname,ios::app);
                file << time_tot << ","<< frames[i].d_density << '\n';
        }
    }
    time(&end);
    double time_taken = double(end - start);
    cout << "Time taken by program is : " << fixed 
         << time_taken << setprecision(5);
    cout << " sec " << endl;

}
