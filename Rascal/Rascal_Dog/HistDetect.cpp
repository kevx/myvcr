
#include "stdafx.h"
#include "HistDetect.h"



CvHistogram* _histPrev = NULL;
CvHistogram* _histCurr = NULL;

CvHistogram* getHist(IplImage* src)
{
	IplImage* h_plane = cvCreateImage( cvGetSize(src), 8, 1 );
	IplImage* s_plane = cvCreateImage( cvGetSize(src), 8, 1 );
	IplImage* v_plane = cvCreateImage( cvGetSize(src), 8, 1 );
	IplImage* hsv = cvCreateImage( cvGetSize(src), 8, 3 );
    IplImage* planes[] = { h_plane, s_plane };
    
    int h_bins = 30, s_bins = 42;
    int hist_size[] = {h_bins, s_bins};
    float h_ranges[] = { 0, 180 }; /* hue varies from 0 (~0¡ãred) to 180 (~360¡ãred again) */
    float s_ranges[] = { 0, 255 }; /* saturation varies from 0 (black-gray-white) to 255 (pure spectrum color) */
    float* ranges[] = { h_ranges, s_ranges };
    
    cvCvtColor( src, hsv, CV_BGR2HSV );
    cvCvtPixToPlane( hsv, h_plane, s_plane, v_plane, 0 );
    CvHistogram* hist = cvCreateHist( 2, hist_size, CV_HIST_ARRAY, ranges, 1 );
    cvCalcHist( planes, hist, 0, 0 );

	cvReleaseImage(&h_plane);
	cvReleaseImage(&s_plane);
	cvReleaseImage(&v_plane);
	cvReleaseImage(&hsv);
	return hist;
}


double compareHist(IplImage* img)
{
	double result = 1.0;

	if(_histPrev == NULL) {
		_histPrev = getHist(img);
		return result;
	} else {
		_histCurr = getHist(img);
		result = cvCompareHist(_histPrev, _histCurr, CV_COMP_CORREL);
		cvReleaseHist(&_histPrev);
		_histPrev = _histCurr;
	}

	return result;
}

