import cv2
import matplotlib.pyplot as plt
import cvlib as cv
import urllib.request
import numpy as np
from cvlib.object_detection import draw_bbox
import concurrent.futures

#import serial
#import time
#arduino = serial.Serial(port='COM14', baudrate=115200, timeout=.1)
 
url='http://192.168.164.155/cam-mid.jpg'
im=None

pic_path = './take_picture/picture/'
def run1():
    cv2.namedWindow("live transmission", cv2.WINDOW_AUTOSIZE)
    n = 0
    while True:
        img_resp=urllib.request.urlopen(url)
        imgnp=np.array(bytearray(img_resp.read()),dtype=np.uint8)
        im = cv2.imdecode(imgnp,-1)
 
        cv2.imshow('live transmission',im)
        key=cv2.waitKey(5)
        if key==ord('q'):
            break
        elif key == ord('h'):
            print('hi')
            pic = pic_path + 'pic_' + str(n) + '.jpg'
            print(pic)
            cv2.imwrite(pic, im)
            n = n + 1
            
    cv2.destroyAllWindows()
 
if __name__ == '__main__':
    print("started")
    run1()
    
    
