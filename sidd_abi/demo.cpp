
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <string>
using namespace cv;
using namespace std;

/// Global variables
Mat src, dst, dst_resized;
RNG rng(12345);
String file_name;

// variables for customizing
// changing this values gives different results
int scale = 4;
int vertical_divide = 1;
int canny_threshold1 = 0;
int canny_threshold2 = 255;

const char* window_name = "divideHW Demo";

/** Function Headers */
void process_image( int, void* );
vector<Rect> divideHW( Mat src, int dim, double threshold1, double threshold2 );


void find_label(Mat src){
      // Convert to hsv
      Mat3b img_hsv;
      cvtColor(src, img_hsv, COLOR_BGR2HSV);

      Mat1b yellow, green, brown;

      //Yellow
      inRange(img_hsv, Scalar(25, 80, 80), Scalar(36, 255, 255), yellow);
      //Green
      inRange(img_hsv, Scalar(37, 80, 80), Scalar(70, 255, 255), green);
      //Brown
      inRange(img_hsv, Scalar(10, 80, 80), Scalar(30, 200, 200), brown);

      // logical OR mask
      Mat1b mask = yellow;

      imwrite("yellow.jpg", yellow);
      // Find non zero pixels
      vector<Point> pts;
      findNonZero(mask, pts);
      /*
      cout << "Width: " << box.width;
      cout << "Height: " << box.height << endl;
      */
      // Show box
      //rectangle(src, box, Scalar(0,0,255), 3);
      //imshow("Box", src);

      Mat canny_output;
      vector<vector<Point> > contours;
      vector<Vec4i> hierarchy;
      int thresh = 100;


      /// Detect edges using canny
      Canny( mask, canny_output, thresh, thresh*2, 3 );
      /// Find contours
      findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

      /// Draw contours
      Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
      for( int i = 0; i< contours.size(); i++ )
         {
           Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
           drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
         }

      /// Show in a window
      //namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
      //imshow( "Contours", drawing );



      //waitKey(0);

}
/**
 * @function main
 */
int main( int, char** argv )
{
    /// Load an image
    file_name = argv[1];
    src = imread( argv[1], IMREAD_COLOR );

    if( src.empty() )
    {
        return -1;
    }

    /// Create window
    //namedWindow( window_name, WINDOW_NORMAL );
    scale = 6;
    vertical_divide = 0;
    canny_threshold1 = 172;
    canny_threshold2 = 177;
    process_image( 0, 0 );
    waitKey(0);
    return 0;
}

/**
 * @function process_image
 */
void process_image( int, void* )
{

    string tmp_file_name = "shelf/" + file_name.substr(8, (file_name.size() - 8)) + "_";
    cout << tmp_file_name << endl;
    dst = src.clone();
    if( scale < 1)
    {
        scale = 1;
    }
    resize( dst, dst_resized, Size(), (double) 1 / scale, (double) 1 / scale );

    vector<Rect> rois_h = divideHW( dst_resized, 1, canny_threshold1, canny_threshold2 );
    vector<Rect> rect_list;
    vector<Rect> shelf_tag;
    vector<Rect> shelf;
    int count = 0;
    for( size_t i=0; i< rois_h.size(); i++ )
    {
        if( vertical_divide )
        {
            Mat roi_h = dst_resized( rois_h[i]);

            vector<Rect> rois_w = divideHW( roi_h, 0, canny_threshold1, canny_threshold2 );

            for( size_t j=0; j< rois_w.size(); j++ )
            {
                rois_w[j].y += rois_h[i].y;
                rectangle( dst_resized, rois_w[j], Scalar( 0, 255, 0 ), 1 );
                rois_w[j].x = rois_w[j].x * scale;
                rois_w[j].y = rois_w[j].y * scale;
                rois_w[j].width = rois_w[j].width * scale;
                rois_w[j].height = rois_w[j].height * scale;

                rectangle( dst, rois_w[j], Scalar( 0, 255, 0 ), 3 );
            }
        }
        rectangle( dst_resized, rois_h[i], Scalar( 0, 0, 255 ), 2 );
        rois_h[i].x = rois_h[i].x * scale;
        rois_h[i].y = rois_h[i].y * scale;
        rois_h[i].width = rois_h[i].width * scale;
        rois_h[i].height = rois_h[i].height * scale;
        rois_h[i].height += 15;
        rois_h[i].y-=12;
        //rectangle( dst, rois_h[i], Scalar( 0, 0, 255), 3 );
        rect_list.push_back(rois_h[i]);
        cout << rois_h[i];
        if(rois_h[i].height < 1000 && rois_h[i].height > 700){
          shelf.push_back(rois_h[i]);
        }

        else if(rois_h[i].height < 200 && rois_h[i].height > 70){
          shelf_tag.push_back(rois_h[i]);
          rois_h[i].width-=100;

          string tag_file_name = tmp_file_name + to_string(count) + ".jpg";
          count++;
          cout << tag_file_name<< endl;
          Mat crop = dst(rois_h[i]);
          imwrite(tag_file_name, crop);

          //imwrite("", crop);
        }
        //imshow("", dst_resized(rois_h[i]));
    }

    //imshow( "resized", dst_resized );
    //imshow( window_name, dst );

}

// helper function returns rectangles according horizontal or vertical projection of given image
// parameters:
// src source image
// dim dimension 1 for horizontal 0 for vertical projection
// threshold1 first threshold for the hysteresis procedure ( used by internal Canny )
// threshold2 second threshold for the hysteresis procedure ( used by internal Canny )

vector<Rect> divideHW( Mat src, int dim, double threshold1, double threshold2 )
{
    Mat gray, reduced, canny;

    if( src.channels() == 1 )
    {
        gray = src;
    }

    if( src.channels() == 3 )
    {
        cvtColor( src, gray, CV_BGR2GRAY );
    }

    reduce( gray, reduced, dim, REDUCE_AVG );

    //GaussianBlur( reduced, reduced, Size(),3);

    Canny( reduced, canny, threshold1, threshold2 );

    vector<Point> pts;
    findNonZero( canny, pts);

    vector<Rect> rects;

    Rect rect(0,0,gray.cols,gray.rows);
    if( !pts.size() )
    {
        rects.push_back( rect );
    }
    int ref_x = 0;
    int ref_y = 0;

    for( size_t i=0; i< pts.size(); i++ )
    {
        if( dim )
        {
            rect.height = pts[i].y-ref_y;
            rects.push_back( rect );
            rect.y = pts[i].y;
            ref_y = rect.y;
            if( i == pts.size()-1 )
            {
                rect.height = gray.rows - pts[i].y;
                rects.push_back( rect );
            }
        }

        else
        {
            rect.width = pts[i].x-ref_x;
            rects.push_back( rect );
            rect.x = pts[i].x;
            ref_x = rect.x;
            if( i == pts.size()-1 )
            {
                rect.width = gray.cols - pts[i].x;
                rects.push_back( rect );
            }
        }

    }
    return rects;
}
