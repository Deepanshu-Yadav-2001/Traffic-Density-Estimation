#include <bits/stdc++.h>
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
    int non_zero_pixel;
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
    
    Mat empty = f_struc->empt;
    Mat gray_f = f_struc->frame ;
    Mat diff;
    absdiff(gray_f, empty, diff);
    Mat im1;
    threshold( diff , im1, threshold_value1, max_binary_value, THRESH_BINARY );	
    
    f_struc->non_zero_pixel = countNonZero(im1);
    pthread_exit(0);

}

int main(int argc, char** argv)
{
    string x;
    cout<<"enter the number of splits: ";
    cin>>x;
    string flname = x +"_3q.txt"  ;
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
    Mat empt =  imread("empty.jpg");
    cvtColor(empt, empt, COLOR_BGR2GRAY);
    GaussianBlur(empt, empt, Size(5, 5), 0);
    Mat temp1;
    warpPerspective(empt,temp1,h1, empt.size());
    Mat img = temp1(Rect(460,52,340,738));
    Mat img_parts[no_of_parts];

    struct queue_density_struc frames[no_of_parts] ;
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
        warpPerspective(fr_ex,temp,h1, empt.size());
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
            frames[i].frame = fr_parts[i];
            frames[i].empt = img_parts[i] ;

            pthread_attr_t attr;
            pthread_attr_init(&attr);
            pthread_create(&tids[i], &attr, queue_density, &frames[i]);
        }
        int TotalNumberOfPixels1 = img.rows * img.cols;;
        int nzp1 = 0 ;
        for(int i=0;i < no_of_parts; i++){
            pthread_join(tids[i], NULL);
            nzp1 += frames[i].non_zero_pixel ;
        }
        double q_d = (double) nzp1 / (double) TotalNumberOfPixels1 ;
        ofstream file;
        file.open(flname,ios::app);
        file << q_d << '\n';

    }
    time(&end);
    double time_taken = double(end - start);
    cout << "Time taken by program is : " << fixed
         << time_taken << setprecision(5);
    cout << " sec " << endl;

}
