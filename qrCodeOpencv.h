#ifndef GROUP3_QRCODE
    #define GROUP3_QRCODE

#include <opencv2/objdetect.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

/*!
 \class DisplayUI
 @brief The front-end for the security system that performs all necessary functions.
 The DisplayUI class displays the UI to HTML, with functionalities of displaying live security footage obtained from a camera, 
 displaying and updating the user log database, displaying a user's QR code and displaying weather obtained from the weather sensor.
*/
    class displayUI {

        public:
            void display(Mat, Mat);
            int main(int, char*);
    };

#endif //GROUP3_QRCODE_H

