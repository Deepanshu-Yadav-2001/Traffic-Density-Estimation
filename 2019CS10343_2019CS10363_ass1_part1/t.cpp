#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

static vector<Point2f> pts_src(4);
static int cnt = 0;
void click(int event, int x, int y, int flags, void* userdata){
	if(event==EVENT_LBUTTONDOWN){
		cout<<x<<","<<y<<endl;
        pts_src[cnt] = {(float)x,(float)y}; 
        cnt++;
	}
}

int main( int argc, char** argv)
{ 
    vector<Point2f> pts_dst = { {472,52},{472,830},{800,830},{800,52} } ;

    String z = ".jpg";
    Mat imge = imread(argv[1]+z);
    if (imge.empty()) { 
        cout << "image is not present" << endl; 
        //cin.get(); 
        return -1; 
    } 
    Mat grey_image;
    cvtColor( imge, grey_image, COLOR_BGR2GRAY ) ;
    resize(grey_image,grey_image,{1320,730});
    imshow("Original Frame",grey_image);
    for(int i{0};i<4;i++){
	namedWindow("Original Frame",1);
        setMouseCallback("Original Frame",click,NULL);
        imshow("Original Frame",grey_image);
    }
    waitKey(0);
    vector<Point2f> pts_crp = {{0,0},{0,700}, {500,700},{500,0}};
     Mat matrix = getPerspectiveTransform(pts_src, pts_crp);
     Mat crop;
     warpPerspective(grey_image,crop,matrix,{500,700});
     imwrite("croppedFrame.jpg", crop);
    Mat h = findHomography(pts_src, pts_dst);
    Mat imout;
   warpPerspective(grey_image,imout,h,grey_image.size());
   imwrite("projectedFrame.jpg", imout) ;
   imshow("Projected Frame",imout);
   waitKey(0);
    imshow("Cropped Frame",crop);
    waitKey(0);
    
    //g++ t.cpp -o empty -std=c++11 `pkg-config --cflags --libs opencv`
}
