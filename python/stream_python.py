import base64
import Image
import time
import urllib2
import cv

# Basic HTTP Authentication...
url = 'http://192.168.0.11:82/Videostream.cgi'
ww = 'Username:Password'
encodedstring = base64.encodestring(ww)[:-1]
auth = "Basic %s" % encodedstring
req = urllib2.Request(url,None, {"Authorization": auth })
handle = urllib2.urlopen(req)

def read_stream():
    buf = ''
    b = handle.readlines(45)
    for a in b:
        if a.startswith('Content-Length'):
            readlen = str(a).split()[1]
    b1 = handle.read(int(readlen)+4)
    return b1

def test():
    pass

def write_stream():
    imgc = read_stream()
    cv_img = cv.CreateImageHeader((640,480), cv.IPL_DEPTH_8U, 3)
    buf = Image.fromstring('RGB',(640,480),imgc[2:], 'jpeg', 'RGB', None)
    cv.SetData(cv_img, buf.tostring(), (640*3))
    return cv_img

