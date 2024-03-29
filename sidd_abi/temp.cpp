/**
 * @file MatchTemplate_Demo.cpp
 * @brief Sample code to use the function MatchTemplate
 * @author OpenCV team
 */

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>

using namespace std;
using namespace cv;

/// Global Variables
bool use_mask;
Mat img; Mat templ; Mat mask; Mat result, img_to_display;
const char* image_window = "Source Image";
const char* result_window = "Result window";

int match_method;
int max_Trackbar = 5;

/// Function Headers
void MatchingMethod(Mat, Mat);

/**
 * @function main
 */
int main( int argc, char** argv )
{
  if (argc < 3)
  {
    cout << "Not enough parameters" << endl;
    cout << "Usage:\n./MatchTemplate_Demo <image_name> <template_name> [<mask_name>]" << endl;
    return -1;
  }

  /// Load image and template
  img = imread( argv[1], IMREAD_COLOR );
  templ = imread( argv[2], IMREAD_COLOR );

  if(argc > 3) {
    use_mask = true;
    mask = imread( argv[3], IMREAD_COLOR );
  }

  if(img.empty() || templ.empty() || (use_mask && mask.empty()))
  {
    cout << "Can't read one of the images" << endl;
    return -1;
  }

  /// Create windows
  namedWindow( image_window, WINDOW_AUTOSIZE );
  namedWindow( result_window, WINDOW_AUTOSIZE );

  /// Create Trackbar
  /*
  const char* trackbar_label = "Method: \n \
                                0: SQDIFF \n \
                                1: SQDIFF NORMED \n \
                                2: TM CCORR \n \
                                3: TM CCORR NORMED \n \
                                4: TM COEFF \n \
                                5: TM COEFF NORMED";

  */
  //createTrackbar( trackbar_label, image_window, &match_method, max_Trackbar, MatchingMethod );

  vector<string> tmpl_list = {"tmpl1.png", "tmpl2.png", "tmpl3.png", "tmpl4.png", "tmpl5.png", "tmpl6.png", "tmpl7.png", \

                              "tmpl8.png", "tmpl9.png", "tmpl10.png", "tmpl11.png", "tmpl12.png", "tmpl13.png" ,"tmpl16.png", \

                              "tmpl16.png", "tmpl17.png", "tmpl19.png", "tmpl20.png", "tmpl21.png"}; //"tmpl20.png", "tmpl21.png"};

  img_to_display = img;

  for(int i=0; i < tmpl_list.size(); i ++){
    Mat tmpl = imread(tmpl_list[i], IMREAD_COLOR);
    MatchingMethod(tmpl, img_to_display);

  }
  /*
  Mat tmpl = imread("tmpl11.png", IMREAD_COLOR);
  tmpl = imread("tmpl12.png", IMREAD_COLOR);
  MatchingMethod(tmpl, img_to_display);
  */
  waitKey(0);
  return 0;
}

/**
 * @function MatchingMethod
 * @brief Trackbar callback
 */
void MatchingMethod(Mat tmpl,Mat img_to_display)
{
  /// Source image to display
  templ = tmpl;
  Mat img_display ;
  img.copyTo( img_display );

  /// Create the result matrix
  int result_cols =  img.cols - templ.cols + 1;
  int result_rows = img.rows - templ.rows + 1;

  result.create( result_rows, result_cols, CV_32FC1 );

  /// Do the Matching and Normalize
  bool method_accepts_mask = (CV_TM_SQDIFF == match_method || match_method == CV_TM_CCORR_NORMED);
  if (use_mask && method_accepts_mask)
    { matchTemplate( img, templ, result, match_method, mask); }
  else
    { matchTemplate( img, templ, result, match_method); }

  normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );

  /// Localizing the best match with minMaxLoc
  double minVal; double maxVal; Point minLoc; Point maxLoc;
  Point matchLoc;

  minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );


  /// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
  if( match_method  == TM_SQDIFF || match_method == TM_SQDIFF_NORMED )
    { matchLoc = minLoc; }
  else
    { matchLoc = maxLoc; }

  /// Show me what you got
  rectangle( img_to_display, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar::all(0), 2, 8, 0 );
  rectangle( result, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar::all(0), 2, 8, 0 );

  imshow( image_window, img_to_display );
  imwrite("final.png", img_to_display);
  imshow( result_window, result );

  return;
}
