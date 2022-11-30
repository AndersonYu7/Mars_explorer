import cv2
import urllib.request
import numpy as np
import serial
import time
arduino = serial.Serial(port='COM11', baudrate=115200, timeout=.1)

# 讀取中文路徑圖檔(圖片讀取為BGR)
def cv_imread(image_path):
    image = cv2.imdecode(np.fromfile(image_path, dtype=np.uint8), -1)
    image = cv2.cvtColor(image, cv2.COLOR_BGRA2BGR)
    return image

# 顯示圖檔
def show_img(name, image):
    cv2.imshow(name, image)
    cv2.waitKey(0)

# 讀取模型架構與權重
def initNet():
    CONFIG = './teddy_4/teddy_4.cfg'
    WEIGHT = './teddy_4/teddy_4_final.weights'
    net = cv2.dnn.readNet(CONFIG, WEIGHT)
    model = cv2.dnn_DetectionModel(net)
    # 若以yolov4-tiny進行物件偵測，預設size=(416, 416)
    model.setInputParams(size=(416, 416), scale=1/255.0)
    model.setInputSwapRB(True)
    return model

# 物件偵測
def nnProcess(image, model):
    classes, confs, boxes = model.detect(image, 0.3, 0.2)
    return classes, confs, boxes

# 在原圖上畫出被偵測的物件
def drawBox(image, classes, confs, boxes):
    image1 = image.copy()
    center_x = 0
    center_y = 0
    for (classid, conf, box) in zip(classes, confs, boxes):
        print(classid)
        x, y, w, h = box
        cv2.rectangle(image1, (x, y), (x + w, y + h), (180, 0, 0), 3)
        center_x = int(x+w/2)
        center_y = int(y+h/2)
        cv2.circle(image1,(center_x,center_y), 3, (0, 255, 255), 3)
        if(center_x or center_y):
            break
    return image1, center_x, center_y
 
url='http://192.168.50.44/cam-mid.jpg'
im=None
 
def run1():
    while True:
        model_YOLOV4 = initNet()
        img_resp=urllib.request.urlopen(url)
        imgnp=np.array(bytearray(img_resp.read()),dtype=np.uint8)
        image = cv2.imdecode(imgnp,-1)
        
        classes, confs, boxes = nnProcess(image, model_YOLOV4)
        final_image, center_x, center_y = drawBox(image, classes, confs, boxes)
        cv2.imshow('test', final_image)
        if(center_x):
            cv2.destroyAllWindows() 
            cv2.rectangle(final_image, (1, 1), (638, 478), (180, 0, 0), 3) #640 * 480
            cv2.circle(final_image,(320,240), 3, (0, 255, 255), 3)
            cv2.imshow('ts', final_image)
            cv2.waitKey(3000)#3s
            print(center_x, center_y)
            value = write_read(center_x, center_y)
            value = value.decode()
            data = value.split(',')
            return data

        key=cv2.waitKey(5)  #5ms
        if key==ord('q'):
            break

    cv2.destroyAllWindows()  
 
def write_read(x, y):
    sum = str(x) + ','+ str(y) + '\r'
    arduino.write(sum.encode())    #b'x' 如果要控制arduino 必須使用utf-8, 
    time.sleep(0.05)                    #訊息必須是位元組類型
    data = arduino.readline()
    print(data)
    return data

if __name__ == '__main__':
    print("started")
    data = run1()
    print(int(data[0]))
    print(int(data[1]))
    
    
