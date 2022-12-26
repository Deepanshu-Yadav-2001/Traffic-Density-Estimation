#include <bits/stdc++.h>
#include <pthread.h>
#include <iostream>
#include "opencv2/opencv.hpp"
#include <fstream>
using namespace cv;
using namespace std;

struct  dynamic_density_struc
{
    Mat frame;
    Mat previous;
    double d_density;
    int tot;
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

void* dynamic_density(void* f)
{
    struct dynamic_density_struc *f_struc = (struct dynamic_density_struc *) f ;
    
    Mat prev = f_struc->previous ;
    Mat next = f_struc->frame ;
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
    
    double non_zero_pixel = countNonZero(im2);
    double total = f_struc->tot ;
    f_struc->d_density = non_zero_pixel/total;
    pthread_exit(0);

}

int main(int argc, char** argv)
{
    string x;
    cout<<"enter the number of splits: ";
    cin>>x;
    string flname = x +"_3d.txt"  ;
    ofstream outFile;
    outFile.open(flname);
    time_t start, end;
    
    time(&start);

    int no_of_parts = stoi(x);
    // if (argc < 2)
    // {
    //     printf("invalid arguments");
    //     exit(-1);
    // }
    string z = ".mp4";
    VideoCapture vid(argv[1]+z);
    Mat fr_ex;
    vid >> fr_ex ;
    cvtColor(fr_ex, fr_ex, COLOR_BGR2GRAY);
    GaussianBlur(fr_ex, fr_ex, Size(5, 5), 0);
    Mat temp1;
    warpPerspective(fr_ex,temp1,h1, fr_ex.size());
    Mat img = temp1(Rect(460,52,340,738));
    Mat img_parts[no_of_parts];

    struct dynamic_density_struc frames[no_of_parts] ;
    pthread_t tids[no_of_parts];
    bool loop = true;
    if(no_of_parts== 4)
    {
            // 4 parts
            img_parts[0]
            = img(cv::Range(0, img.rows / 2 - 1), cv::Range(0, img.cols / 2 - 1));
            img_parts[2]
            = img(cv::Range(0, img.rows / 2 - 1), cv::Range(img.cols / 2, img.cols - 1));
            img_parts[1]
            = img(cv::Range(img.rows / 2, img.rows - 1), cv::Range(0, img.cols / 2 - 1));
            img_parts[3]
            = img(cv::Range(img.rows / 2, img.rows - 1), cv::Range(img.cols / 2, img.cols - 1));
    }
    else if(no_of_parts== 1)
    {
            // 1 part
            img_parts[0]
            = img;
    }
    else if(no_of_parts== 2)
    {
            // 2 parts
            img_parts[0]
            = img(cv::Range(0, img.rows / 2 - 1), cv::Range(0, img.cols - 1));
            img_parts[1]
            = img(cv::Range(img.rows / 2, img.rows - 1), cv::Range(0, img.cols - 1));
    }
    else if(no_of_parts == 3)
    {
            // 3 parts
            img_parts[0]
            = img(cv::Range(0, img.rows / 3 - 1), cv::Range(0, img.cols - 1));
            img_parts[1]
            = img(cv::Range(img.rows / 3, 2*(img.rows/3) - 1), cv::Range(0, img.cols - 1));
            img_parts[2]
            = img(cv::Range(2*(img.rows/3), img.rows - 1), cv::Range(0, img.cols - 1));
    }
    else if(no_of_parts == 5)
    {
            // 5 parts
            img_parts[0]
            = img(cv::Range(0, img.rows / 5 - 1), cv::Range(0, img.cols - 1));
            img_parts[1]
            = img(cv::Range(img.rows / 5, 2*(img.rows/5) - 1), cv::Range(0, img.cols - 1));
            img_parts[2]
            = img(cv::Range(2*(img.rows / 5), 3*(img.rows/5) - 1), cv::Range(0, img.cols - 1));
            img_parts[3]
            = img(cv::Range(3*(img.rows / 5), 4*(img.rows/5) - 1), cv::Range(0, img.cols - 1));
            img_parts[4]
            = img(cv::Range(4*(img.rows/5), img.rows - 1), cv::Range(0, img.cols - 1));
    }
    else if(no_of_parts == 7)
    {
            // 7 parts
            img_parts[0]
            = img(cv::Range(0, img.rows / 7 - 1), cv::Range(0, img.cols - 1));
            img_parts[1]
            = img(cv::Range(img.rows / 7, 2*(img.rows/7) - 1), cv::Range(0, img.cols - 1));
            img_parts[2]
            = img(cv::Range(2*(img.rows / 7), 3*(img.rows/7) - 1), cv::Range(0, img.cols - 1));
            img_parts[3]
            = img(cv::Range(3*(img.rows / 7), 4*(img.rows/7) - 1), cv::Range(0, img.cols - 1));
            img_parts[4]
            = img(cv::Range(4*(img.rows/7), 5*(img.rows /7)- 1), cv::Range(0, img.cols - 1));
            img_parts[5]
            = img(cv::Range(5*(img.rows/7), 6*(img.rows /7)- 1), cv::Range(0, img.cols - 1));
            img_parts[6]
            = img(cv::Range(6*(img.rows/7), (img.rows)- 1), cv::Range(0, img.cols - 1));
    }
    else if(no_of_parts== 6)
    {
            // 6 parts
            img_parts[0]
            = img(cv::Range(0, img.rows / 3 - 1), cv::Range(0, img.cols / 2 - 1));
            img_parts[1]
            = img(cv::Range(img.rows / 3 , 2* (img.rows/3) - 1 ), cv::Range(0, img.cols / 2 - 1));
            img_parts[2]
            = img(cv::Range(2* (img.rows/3), img.rows- 1), cv::Range(0, img.cols / 2 - 1));
            img_parts[3]
            = img(cv::Range(0, img.rows / 3 - 1), cv::Range(img.cols / 2, img.cols-1));
            img_parts[4]
            = img(cv::Range(img.rows / 3 , 2* (img.rows/3) - 1 ), cv::Range(img.cols / 2, img.cols-1));
            img_parts[5]
            = img(cv::Range(2* (img.rows/3), img.rows- 1), cv::Range(img.cols / 2, img.cols-1));
    }
    else if(no_of_parts== 8)
    {
            // 8 parts
            img_parts[0]
            = img(cv::Range(0, img.rows / 4 - 1), cv::Range(0, img.cols / 2 - 1));
            img_parts[1]
            = img(cv::Range(img.rows / 4 , (img.rows/2) - 1 ), cv::Range(0, img.cols / 2 - 1));
            img_parts[2]
            = img(cv::Range((img.rows/2 ), 3*(img.rows/4)- 1), cv::Range(0, img.cols / 2 - 1));
            img_parts[3]
            = img(cv::Range(3*(img.rows/4), img.rows- 1), cv::Range(0, img.cols / 2 - 1));
            img_parts[4]
            = img(cv::Range(0, img.rows / 4 - 1), cv::Range(img.cols / 2, img.cols-1));
            img_parts[5]
            = img(cv::Range(img.rows / 4 , (img.rows/2) - 1 ), cv::Range(img.cols / 2, img.cols-1));
            img_parts[6]
            = img(cv::Range((img.rows/2 ), 3*(img.rows/4)- 1), cv::Range(img.cols / 2, img.cols-1));
            img_parts[7]
            = img(cv::Range(3*(img.rows/4), img.rows- 1), cv::Range(img.cols / 2, img.cols-1));
    }
    else if(no_of_parts == 9)
    {
            // 7 parts
            img_parts[0]
            = img(cv::Range(0, img.rows / 9 - 1), cv::Range(0, img.cols - 1));
            img_parts[1]
            = img(cv::Range(img.rows / 9, 2*(img.rows/9) - 1), cv::Range(0, img.cols - 1));
            img_parts[2]
            = img(cv::Range(2*(img.rows / 9), 3*(img.rows/9) - 1), cv::Range(0, img.cols - 1));
            img_parts[3]
            = img(cv::Range(3*(img.rows / 9), 4*(img.rows/9) - 1), cv::Range(0, img.cols - 1));
            img_parts[4]
            = img(cv::Range(4*(img.rows/9), 5*(img.rows /9)- 1), cv::Range(0, img.cols - 1));
            img_parts[5]
            = img(cv::Range(5*(img.rows/9), 6*(img.rows /9)- 1), cv::Range(0, img.cols - 1));
            img_parts[6]
            = img(cv::Range(6*(img.rows/9), 7*(img.rows / 9)- 1), cv::Range(0, img.cols - 1));
            img_parts[7]
            = img(cv::Range(7*(img.rows/9), 8*(img.rows / 9)- 1), cv::Range(0, img.cols - 1));
            img_parts[8]
            = img(cv::Range(8*(img.rows/9), (img.rows)- 1), cv::Range(0, img.cols - 1));
  
    }
    else{
            cout << "Cannot be divided into these many parts" << "\n";
            cout << "Try using 1, 2, 3, 4, 5, 6, 7, 8, 9 instead" << '\n';
            return -1;
    }

    while(loop){
        
        vid >> fr_ex; vid >> fr_ex ; vid >> fr_ex ; vid >> fr_ex ; vid >> fr_ex;
        if(fr_ex.empty()){ 
            loop = false;
            break;
        }
        Mat temp;
        cvtColor(fr_ex, fr_ex, COLOR_BGR2GRAY);
        GaussianBlur(fr_ex, fr_ex, Size(5, 5), 0);
        warpPerspective(fr_ex,temp,h1, fr_ex.size());
        Mat fr = temp(Rect(460,52,340,738));
        Mat fr_parts[no_of_parts];
        if(no_of_parts== 4)
        {
            // 4 parts
            fr_parts[0]
            = fr(cv::Range(0, fr.rows / 2 - 1), cv::Range(0, fr.cols / 2 - 1));
            fr_parts[2]
            = fr(cv::Range(0, fr.rows / 2 - 1), cv::Range(fr.cols / 2, fr.cols - 1));
            fr_parts[1]
            = fr(cv::Range(fr.rows / 2, fr.rows - 1), cv::Range(0, fr.cols / 2 - 1));
            fr_parts[3]
            = fr(cv::Range(fr.rows / 2, fr.rows - 1), cv::Range(fr.cols / 2, fr.cols - 1));
        }
        else if(no_of_parts== 1)
        {
            // 1 part
            fr_parts[0]
            = fr;
        }
        else if(no_of_parts== 2)
        {
            // 2 parts
            fr_parts[0]
            = fr(cv::Range(0, fr.rows / 2 - 1), cv::Range(0, fr.cols - 1));
            fr_parts[1]
            = fr(cv::Range(fr.rows / 2, fr.rows - 1), cv::Range(0, fr.cols - 1));
        }
        else if(no_of_parts== 6)
        {
            // 6 parts
            fr_parts[0]
            = fr(cv::Range(0, fr.rows / 3 - 1), cv::Range(0, fr.cols / 2 - 1));
            fr_parts[1]
            = fr(cv::Range(fr.rows / 3 , 2* (fr.rows/3) - 1 ), cv::Range(0, fr.cols / 2 - 1));
            fr_parts[2]
            = fr(cv::Range(2* (fr.rows/3), fr.rows- 1), cv::Range(0, fr.cols / 2 - 1));
            fr_parts[3]
            = fr(cv::Range(0, fr.rows / 3 - 1), cv::Range(fr.cols / 2, fr.cols-1));
            fr_parts[4]
            = fr(cv::Range(fr.rows / 3 , 2* (fr.rows/3) - 1 ), cv::Range(fr.cols / 2, fr.cols-1));
            fr_parts[5]
            = fr(cv::Range(2* (fr.rows/3), fr.rows- 1), cv::Range(fr.cols / 2, fr.cols-1));
        }
        else if(no_of_parts== 8)
        {
            // 8 parts
            fr_parts[0]
            = fr(cv::Range(0, fr.rows / 4 - 1), cv::Range(0, fr.cols / 2 - 1));
            fr_parts[1]
            = fr(cv::Range(fr.rows / 4 , (fr.rows/2) - 1 ), cv::Range(0, fr.cols / 2 - 1));
            fr_parts[2]
            = fr(cv::Range((fr.rows/2 ), 3*(fr.rows/4)- 1), cv::Range(0, fr.cols / 2 - 1));
            fr_parts[3]
            = fr(cv::Range(3*(fr.rows/4), fr.rows- 1), cv::Range(0, fr.cols / 2 - 1));
            fr_parts[4]
            = fr(cv::Range(0, fr.rows / 4 - 1), cv::Range(fr.cols / 2, fr.cols-1));
            fr_parts[5]
            = fr(cv::Range(fr.rows / 4 , (fr.rows/2) - 1 ), cv::Range(fr.cols / 2, fr.cols-1));
            fr_parts[6]
            = fr(cv::Range((fr.rows/2 ), 3*(fr.rows/4)- 1), cv::Range(fr.cols / 2, fr.cols-1));
            fr_parts[7]
            = fr(cv::Range(3*(fr.rows/4), fr.rows- 1), cv::Range(fr.cols / 2, fr.cols-1));
        }
        else if(no_of_parts == 9)
        {
            // 7 parts
            fr_parts[0]
            = img(cv::Range(0, fr.rows / 9 - 1), cv::Range(0, fr.cols - 1));
            fr_parts[1]
            = fr(cv::Range(fr.rows / 9, 2*(fr.rows/9) - 1), cv::Range(0, fr.cols - 1));
            fr_parts[2]
            = fr(cv::Range(2*(fr.rows / 9), 3*(fr.rows/9) - 1), cv::Range(0, fr.cols - 1));
            fr_parts[3]
            = fr(cv::Range(3*(fr.rows / 9), 4*(fr.rows/9) - 1), cv::Range(0, fr.cols - 1));
            fr_parts[4]
            = fr(cv::Range(4*(fr.rows/9), 5*(fr.rows /9)- 1), cv::Range(0, fr.cols - 1));
            fr_parts[5]
            = fr(cv::Range(5*(fr.rows/9), 6*(fr.rows /9)- 1), cv::Range(0, fr.cols - 1));
            fr_parts[6]
            = fr(cv::Range(6*(fr.rows/9), 7*(fr.rows / 9)- 1), cv::Range(0, fr.cols - 1));
            fr_parts[7]
            = fr(cv::Range(7*(fr.rows/9), 8*(fr.rows / 9)- 1), cv::Range(0, fr.cols - 1));
            fr_parts[8]
            = fr(cv::Range(8*(fr.rows/9), (fr.rows)- 1), cv::Range(0, fr.cols - 1));
  
        }
        else if(no_of_parts == 3)
        {
            // 3 parts
            fr_parts[0]
            = fr(cv::Range(0, fr.rows / 3 - 1), cv::Range(0, fr.cols - 1));
            fr_parts[1]
            = fr(cv::Range(fr.rows / 3, 2*(fr.rows/3) - 1), cv::Range(0, fr.cols - 1));
            fr_parts[2]
            = fr(cv::Range(2*(fr.rows/3), fr.rows - 1), cv::Range(0, fr.cols - 1));
        }
        else if(no_of_parts == 5)
        {
            // 5 parts
            fr_parts[0]
            = fr(cv::Range(0, fr.rows / 5 - 1), cv::Range(0, fr.cols - 1));
            fr_parts[1]
            = fr(cv::Range(fr.rows / 5, 2*(fr.rows/5) - 1), cv::Range(0, fr.cols - 1));
            fr_parts[2]
            = fr(cv::Range(2*(fr.rows / 5), 3*(fr.rows/5) - 1), cv::Range(0, fr.cols - 1));
            fr_parts[3]
            = fr(cv::Range(3*(fr.rows / 5), 4*(fr.rows/5) - 1), cv::Range(0, fr.cols - 1));
            fr_parts[4]
            = fr(cv::Range(4*(fr.rows/5), fr.rows - 1), cv::Range(0, fr.cols - 1));
        }
        else if(no_of_parts == 7)
        {
            // 7 parts
            fr_parts[0]
            = fr(cv::Range(0, fr.rows / 7 - 1), cv::Range(0, fr.cols - 1));
            fr_parts[1]
            = fr(cv::Range(fr.rows / 7, 2*(fr.rows/7) - 1), cv::Range(0, fr.cols - 1));
            fr_parts[2]
            = fr(cv::Range(2*(fr.rows / 7), 3*(fr.rows/7) - 1), cv::Range(0, fr.cols - 1));
            fr_parts[3]
            = fr(cv::Range(3*(fr.rows / 7), 4*(fr.rows/7) - 1), cv::Range(0, fr.cols - 1));
            fr_parts[4]
            = fr(cv::Range(4*(fr.rows/7), 5*(fr.rows /7)- 1), cv::Range(0, fr.cols - 1));
            fr_parts[5]
            = fr(cv::Range(5*(fr.rows/7), 6*(fr.rows /7)- 1), cv::Range(0, fr.cols - 1));
            fr_parts[6]
            = fr(cv::Range(6*(fr.rows/7), (fr.rows)- 1), cv::Range(0, fr.cols - 1));
        }

        for(int i=0; i< no_of_parts ;i++){
            frames[i].tot = fr.rows * fr.cols ;
            frames[i].frame = fr_parts[i];
            frames[i].previous = img_parts[i];
            img_parts[i] = fr_parts[i];

            pthread_attr_t attr;
            pthread_attr_init(&attr);
            pthread_create(&tids[i], &attr, dynamic_density, &frames[i]);
        }
        double d_d =  0;
        for(int i=0;i < no_of_parts; i++){
            pthread_join(tids[i], NULL);
            d_d += frames[i].d_density;
        }
        ofstream file;
        file.open(flname,ios::app);
        file << d_d << '\n';

    }
    time(&end);
    double time_taken = double(end - start);
    cout << "Time taken by program is : " << fixed
         << time_taken << setprecision(5);
    cout << " sec " << endl;

}
