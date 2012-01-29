#include <stdio.h>
#include "opencv/cv.h"
#include "opencv2/highgui/highgui_c.h"

CvHaarClassifierCascade *cascade;
CvMemStorage *storage;

void detectFaces(IplImage *img);

int main(int argc, char **argv){
    CvCapture *capture = 0;
    IplImage *frame = 0;
    CvFont font, font_small;
    int key = 0;
    char *filename = "/Users/danfinkelstein/Desktop/OpenCV-2.3.1/data/haarcascades/haarcascade_frontalface_alt.xml";
    
    cascade = (CvHaarClassifierCascade*) cvLoad(filename, 0, 0, 0);
    storage = cvCreateMemStorage(0);
    capture = cvCaptureFromCAM(0);
    assert(cascade && storage && capture);
    
    if(!capture){
        fprintf(stderr, "Cannot initialize webcam!\n");
        return 1;
    }
    
    cvNamedWindow("Webcam", CV_WINDOW_AUTOSIZE);
    cvInitFont(&font, CV_FONT_HERSHEY_COMPLEX, 1.0, 1.0, 0, 2, CV_AA);
    cvInitFont(&font_small, CV_FONT_HERSHEY_COMPLEX, .5, .5, 0, 1, CV_AA);
    
    while (key != 'q'){
        frame = cvQueryFrame(capture);
        
        if (!frame) break;
        
        cvFlip(frame, frame, 1);
        frame->origin = 0;
        
        if (key == 'd'){
            detectFaces(frame);
        } else {
            cvPutText(frame, "This is live!", cvPoint(20, 50), &font, cvScalar(255, 255, 255, 0));
            cvPutText(frame, "Press 'Q' to quit.", cvPoint(20, 75), &font_small, cvScalar(255, 255, 255, 0));
            cvPutText(frame, "Press and hold 'D' to detect faces.", cvPoint(20, 100), &font_small, cvScalar(255, 255, 255, 0));
            cvShowImage("Webcam", frame);
        }
        
        key = cvWaitKey(1);
    }
    
    cvDestroyWindow("Webcam");
    cvReleaseCapture(&capture);
    cvReleaseHaarClassifierCascade(&cascade);
    cvReleaseMemStorage(&storage);
    
    return 0;
}

void detectFaces( IplImage *img){
    int i;
    CvFont font, font_small;
    cvInitFont(&font, CV_FONT_HERSHEY_COMPLEX, 1.0, 1.0, 0, 2, CV_AA);
    cvInitFont(&font_small, CV_FONT_HERSHEY_COMPLEX, .5, .5, 0, 1, CV_AA);
    
    CvSeq *faces = cvHaarDetectObjects(img, cascade, storage, 1.1, 3, CV_HAAR_DO_CANNY_PRUNING, cvSize(40, 40),cvSize(40, 40));
    
    cvPutText(img, "This is live facial recognition!", cvPoint(20, 50), &font, cvScalar(255, 255, 255, 0));
    cvPutText(img, "Press 'Q' to quit.", cvPoint(20, 75), &font_small, cvScalar(255, 255, 255, 0));
    
    for (i = 0; i < (faces ? faces->total : 0); i++){
        CvRect *r = (CvRect*) cvGetSeqElem(faces, i);
        cvRectangle(img, cvPoint(r->x, r->y), cvPoint(r->x + r->width, r->y + r->height), CV_RGB(255, 0, 0), 3, 8, 0);
        cvPutText(img, "Face Recognized!", cvPoint(r->x + (r->width/4), r->y + r->height), &font_small, cvScalar(0, 255, 0, 0));
    }

    
    cvShowImage("Webcam", img);                                   
}