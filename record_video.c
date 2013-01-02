//http://www.cs.iit.edu/~agam/cs512/lect-notes/opencv-intro/opencv-intro.html




CvCapture* capture = cvCaptureFromCAM(0); // capture from video device #0
//CvCapture* capture = cvCaptureFromAVI("infile.avi");
IpImage* img = 0;

if(!cvGrabFrame(capture)){              // capture a frame 
  printf("Could not grab a frame\n\7");
  exit(0);
}
img=cvRetrieveFrame(capture);           // retrieve the captured frame

/*Get capture device properties:

cvQueryFrame(capture); // this call is necessary to get correct 
                       // capture properties
int frameH    = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT);
int frameW    = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH);
int fps       = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);
int numFrames = (int) cvGetCaptureProperty(capture,  CV_CAP_PROP_FRAME_COUNT);
*/

//Saving a video file

    //Initializing a video writer:

    CvVideoWriter *writer = 0;
    int isColor = 1;
    int fps     = 25;  // or 30
    int frameW  = 640; // 744 for firewire cameras
    int frameH  = 480; // 480 for firewire cameras
    writer=cvCreateVideoWriter("out.avi",CV_FOURCC('P','I','M','1'),fps,cvSize(frameW,frameH),isColor);

   /* Other possible codec codes:

    CV_FOURCC('P','I','M','1')    = MPEG-1 codec
    CV_FOURCC('M','J','P','G')    = motion-jpeg codec (does not work well)
    CV_FOURCC('M', 'P', '4', '2') = MPEG-4.2 codec
    CV_FOURCC('D', 'I', 'V', '3') = MPEG-4.3 codec
    CV_FOURCC('D', 'I', 'V', 'X') = MPEG-4 codec
    CV_FOURCC('U', '2', '6', '3') = H263 codec
    CV_FOURCC('I', '2', '6', '3') = H263I codec
    CV_FOURCC('F', 'L', 'V', '1') = FLV1 codec

    A codec code of -1 will open a codec selection window (in windows).
*/
    //Writing the video file:

    IplImage* img = 0; 
    int nFrames = 50;
    for(i=0;i<nFrames;i++){
      cvGrabFrame(capture);          // capture a frame
      img=cvRetrieveFrame(capture);  // retrieve the captured frame
      cvWriteFrame(writer,img);      // add the frame to the file
    }


