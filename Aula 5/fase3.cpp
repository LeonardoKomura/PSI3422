//fase3.cpp

#include <iostream>
#include <cekeikon.h>
using namespace std;
using namespace cv;

const char* image_window = "Source Image";
const char* result_window = "Result window";

void MatchingMethod(Mat_<FLT> frame, Mat_<FLT> search, int, void* )
{
    Mat img_display;
    Mat result;
    frame.copyTo( img_display );
    int result_cols = frame.cols - search.cols + 1;
    int result_rows = frame.rows - search.rows + 1;
    result.create( result_rows, result_cols, CV_32FC1);
    matchTemplate(frame, search, result, CV_TM_CCOEFF_NORMED);
    normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );
    double minVal; double maxVal; Point minLoc; Point maxLoc;
    Point matchLoc;
    minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
    matchLoc = maxLoc;
    rectangle( img_display, matchLoc, Point( matchLoc.x + search.cols , matchLoc.y + search.rows ), Scalar::all(0), 2, 8, 0 );
    rectangle( result, matchLoc, Point( matchLoc.x + search.cols , matchLoc.y + search.rows ), Scalar::all(0), 2, 8, 0 );
    imshow( image_window, img_display );
    imshow( result_window, result );
    return;
}

int main(int argc, char** argv) {
    if (argc < 3)
    {
        cout << "Not enough parameters" << endl;
        cout << "Usage:\n" << argv[0] << " <input_file> <template_name> <output_file>" << endl;
        return -1;
    }
    VideoCapture vid_capture(argv[1]);
    Mat_<FLT> search; le(search, argv[2]);

    if (!vid_capture.isOpened()) {
        cout << "Error opening video stream or file" << endl;
    }

    else {
        // Obtain fps and frame count by get() method and print
        // You can replace 5 with CAP_PROP_FPS as well, they are enumerations
        int fps = vid_capture.get(5);
        cout << "Frames per second :" << fps << endl;

        // Obtain frame_count using opencv built in frame count reading method
        // You can replace 7 with CAP_PROP_FRAME_COUNT as well, they are enumerations
        int frame_count = vid_capture.get(7);
        cout << "  Frame count :" << frame_count << endl;
    }

    namedWindow(image_window, WINDOW_AUTOSIZE);
    namedWindow(result_window, WINDOW_AUTOSIZE);

    // Read the frames to the last frame
    while (vid_capture.isOpened())
    {
        // Initialise frame matrix
        Mat_<FLT> frame;

        // Initialize a boolean to check if frames are there or not
        bool isSuccess = vid_capture.read(frame);

        MatchingMethod(frame, search, 0, 0);

        /*if (isSuccess == true)
            {
            imshow("Frame", frame);
        }
        */
        // If frames are not there, close it
        if (isSuccess == false)
            {
            cout << "Video camera is disconnected" << endl;
            break;
        }

        //wait 20 ms between successive frames and break the loop if key q is pressed
        int key = waitKey(20);
        if (key == 'q')
            {
            cout << "q key is pressed by the user. Stopping the video" << endl;
            break;
        }


    }
    // Release the video capture object
    vid_capture.release();
    destroyAllWindows();
    return 0;

}