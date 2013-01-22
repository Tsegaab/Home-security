#http://www.steinm.com/blog/motion-detection-webcam-python-opencv-differential-images/

import cv2

def diffImg(t0, t1, t2):
  d1 = cv2.absdiff(t2, t1)
  d2 = cv2.absdiff(t1, t0)
  return cv2.bitwise_or(d1, d2)

def facedetect(img):
    gray = cv2.cvtColor(img, cv2.COLOR_RGB2GRAY)
    gray = cv2.equalizeHist(gray)
    faces = cascade.detectMultiScale(gray, scaleFactor=1.2, minNeighbors=2, minSize=(80, 80), flags = cv2.cv.CV_HAAR_SCALE_IMAGE)
    if len(faces) == 0:
        return []
    else:
        for f in faces:
            print f
        return faces

cam = cv2.VideoCapture(0)
s, img = cam.read()

winName = "Movement Indicator"
cv2.namedWindow(winName, cv2.CV_WINDOW_AUTOSIZE)

# Read three images first:
t_minus = cv2.cvtColor(cam.read()[1], cv2.COLOR_RGB2GRAY)
t = cv2.cvtColor(cam.read()[1], cv2.COLOR_RGB2GRAY)
t_plus = cv2.cvtColor(cam.read()[1], cv2.COLOR_RGB2GRAY)
 
while s:
  cv2.imshow( winName, diffImg(t_minus, t, t_plus) )
  s, img = cam.read() 
  # Read next image
  t_minus = t
  t = t_plus
  t_plus = cv2.cvtColor(cam.read()[1], cv2.COLOR_RGB2GRAY)
    
  key = cv2.waitKey(10)
  if key == 27:
    cv2.destroyWindow(winName)
    break 
print "Goodbye"
