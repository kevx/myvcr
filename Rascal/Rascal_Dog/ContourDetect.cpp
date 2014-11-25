
#include "stdafx.h"
#include "ContourDetect.h"


static int	 _lastContours = 0;		//previous contours count
static IplImage* _prevBinaryImg = NULL;

int	getContoursCount(IplImage* img)
{
	
	double v = (double)g_dogConfig.threshold;

	//IplImage* img = cvCreateImageHeader(cvSize(width, height), DEPTH, bytePerPixel);
	IplImage* imgBw = cvCreateImage(cvGetSize(img), img->depth, 1);
	
	CvMemStorage *imgMem = cvCreateMemStorage(0);
	//cvSetData(img, pIn, width * bytePerPixel);
	cvCvtColor(img, imgBw, CV_RGB2GRAY);
	cvCanny(imgBw, imgBw, v, v*2.5, 3);

	CvSeq* imgSeq = NULL;
	int contoursOfImg = cvFindContours( 
		imgBw, imgMem, &imgSeq, sizeof(CvContour), 
		CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

	cvReleaseImage(&imgBw);
	
	return contoursOfImg;
}

IplImage* getBwImg(IplImage* img)
{
	double v = (double)g_dogConfig.threshold;

	//IplImage* img = cvCreateImageHeader(cvSize(width, height), DEPTH, bytePerPixel);
	IplImage* imgBw = cvCreateImage(cvGetSize(img), img->depth, 1);
	
	CvMemStorage *imgMem = cvCreateMemStorage(0);
	//cvSetData(img, pIn, width * bytePerPixel);
	cvCvtColor(img, imgBw, CV_RGB2GRAY);
	cvCanny(imgBw, imgBw, v, v*2.5, 3);
	return imgBw;
}

double compareContours(IplImage* img)
{
	//int cc = getContoursCount(img);
	//if(_lastContours == 0 || _prevBinaryImg) {
	//	_lastContours = cc;
	//	return 1.0;
	//}

	//return (double)min(cc, _lastContours) / 
	//		(double)max(cc, _lastContours);
	double result = 1.0;
	if(_prevBinaryImg == NULL) {
		_prevBinaryImg = getBwImg(img);
	} else {
		IplImage* currBwImg = getBwImg(img);
		result = cvMatchShapes(_prevBinaryImg, currBwImg, CV_CONTOURS_MATCH_I1);
		cvReleaseImage(&_prevBinaryImg);
		_prevBinaryImg = currBwImg;
	}

	return fabs(1.0 - result);
}
