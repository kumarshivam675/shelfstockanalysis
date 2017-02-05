#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

void find_label(Mat src){
      // Convert to hsv
      Mat3b img_hsv;
      cvtColor(src, img_hsv, COLOR_BGR2HSV);
      Mat1b yellow, white, brown;
      //Yellow
      inRange(img_hsv, Scalar(25, 80, 80), Scalar(36, 255, 255), yellow);
      //White
      //inRange(img_hsv, Scalar(38, 6, 49), Scalar(0, 0, 255), white);
      //Brown
      inRange(img_hsv, Scalar(10, 80, 80), Scalar(30, 200, 200), brown);
      // logical OR mask
      Mat1b mask = white;
      imwrite("yellow.jpg", white);
      waitKey(0);

}


int main(int argc, char** argv){
      Mat src = imread(argv[1]);
      find_label(src);
      return 0;
}
