// This code is from an open source on GitHub: https://github.com/spmallick/learnopencv/commit/4a0f3933fc7c663a5630d4e980b874d725060404

#include "qrCodeOpencv.h"

using namespace cv;
using namespace std;

/*!
 \fn display
 @author asmorkalov
*/
void qrCodeOpencv::display(Mat &im, Mat &bbox)
{
  int n = bbox.rows;
  for(int i = 0 ; i < n ; i++)
  {
    line(im, Point2i(bbox.at<float>(i,0),bbox.at<float>(i,1)), Point2i(bbox.at<float>((i+1) % n,0), bbox.at<float>((i+1) % n,1)), Scalar(255,0,0), 3);
  }
  imshow("Result", im);
}

/*!
 \fn main
 @author asmorkalov
*/
int qrCodeOpencv::main(int argc, char* argv[])
{
  // Read image
  Mat inputImage;
  if(argc>1)
    inputImage = imread(argv[1]);
  else
    inputImage = imread("qrcode-learnopencv.jpg");

  QRCodeDetector qrDecoder;

  Mat bbox, rectifiedImage;

  std::string data = qrDecoder.detectAndDecode(inputImage, bbox, rectifiedImage);
  if(data.length()>0)
  {
    cout << "Decoded Data : " << data << endl;

    display(inputImage, bbox);
    rectifiedImage.convertTo(rectifiedImage, CV_8UC3);
    imshow("Rectified QRCode", rectifiedImage);

    waitKey(0);
  }
  else
    cout << "QR Code not detected" << endl;
}
