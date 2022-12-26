#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;
#include <fstream>

int main(int argc, char** argv){
    string z = ".mp4";
    VideoCapture vid(argv[1]+z);
     if (vid.isOpened()==false) { 
        cout << "video is not present" << endl; 
        return -1; 
    } 
    Mat fr;
    ofstream outFile;
    outFile.open("myfile.txt");
    int threshold_value1 = 40;
    int threshold_value2 = 10;
	int threshold_type = 3;
	int const max_value = 255;
	int const max_type = 4;
	int const max_binary_value = 255;

    Mat empty;
    double tm = 1.0/3;
    empty = imread("empty.jpg");
    Mat empt,prvs;
    vid>>empt;
    cvtColor(empt, prvs, COLOR_BGR2GRAY);
    cout<<"Time"<<"    "<<"Queue Density"<<"    "<<"Dynamic Density"<<"\n";

    for(; ;){
        vid >> fr;
        vid >> fr;
        vid >> fr;
        vid >> fr;
        vid >> fr;
        if(fr.empty()){
            break;
        }
        Mat grayA;
        cvtColor(empty, grayA, COLOR_BGR2GRAY);
        GaussianBlur(grayA, grayA, Size(5, 5), 0);
        Mat grayB;
        cvtColor(fr, grayB, COLOR_BGR2GRAY);
        Mat diff;
        GaussianBlur(grayB, grayB, Size(5, 5), 0);
        absdiff(grayB,grayA,diff);
        Mat im1;
        threshold( diff , im1, threshold_value1, max_binary_value, THRESH_BINARY );	

        GaussianBlur(prvs, prvs, Size(5, 5), 0);
        Mat next = grayB;

        Mat flow(prvs.size(), CV_32FC2);
        calcOpticalFlowFarneback(prvs, next, flow, 0.5, 3, 15, 3, 5, 1.2, 0);

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

        vector<Point2f> pts_src = { {800,244},{620,725},{1450,730},{1400,230} } ;
        
        vector<Point2f> pts_dst = { {472,52},{472,830},{800,830},{800,52} } ;
        Mat h1 = getPerspectiveTransform(pts_src, pts_dst);
    	Mat imout1;
   	    warpPerspective(im1,imout1,h1,fr.size());
   
   	    Mat crop1 = imout1(Rect(460,52,340,738));

        int TotalNumberOfPixels1 = crop1.rows * crop1.cols;
	    int nzp1 = countNonZero(crop1);

        double density1;
        density1 = (double) nzp1 / (double) TotalNumberOfPixels1 ;

        Mat h2 = getPerspectiveTransform(pts_src, pts_dst);
    	Mat imout2;
   	    warpPerspective(im2,imout2,h2,im2.size());
   
   	    Mat crop2= imout2(Rect(460,52,340,738));

        int TotalNumberOfPixels2 = crop2.rows * crop2.cols;
	    int nzp2 = countNonZero(crop2);
        double density2;
        density2 = (double) nzp2 / (double) TotalNumberOfPixels2 ;
        string tim = to_string(tm);
        string dnst1 = to_string(density1);
        string dnst2 = to_string(density2);
        cout << tim <<"\t"<<dnst1<<"\t"<<dnst2<<"\n";

        prvs = next;
        vector<string> vc;
        ofstream file;
        file.open("myfile.txt",ios::app);
        file<<tim<<"    "<<dnst1<<"    "<<dnst2<<"\n";
        tm += (1.0/3);
    }
    //outFile.close();
    return 1;
}
