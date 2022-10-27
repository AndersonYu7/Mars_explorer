import cv2
import urllib.request
import numpy as np
import concurrent.futures
import serial
 
url='http://192.xxx.xx.xxx/cam-hi.jpg'  #從esp32-cam上傳照片的網站擷取圖片 設定url
im=None

COM_PORT = 'COM6'   #從COM_PORT串口傳輸與接收資訊
BAUD_RATES = 115200 #BAUD RATES 設為115200
ser = serial.Serial(COM_PORT, BAUD_RATES)   
 
def run():
    cv2.namedWindow("detection", cv2.WINDOW_AUTOSIZE)
    while ser.in_waiting:          # 若收到序列資料…
            data_raw = ser.readline()  # 讀取一行
            data = data_raw.decode() #用預設的UTF-8解碼 -> arm_done?

    while True and data == '1':
        img_resp=urllib.request.urlopen(url)
        imgnp=np.array(bytearray(img_resp.read()),dtype=np.uint8)
        im = cv2.imdecode(imgnp,-1)
 
        gray = cv2.cvtColor(im, cv2.COLOR_BGR2GRAY)
        stonecascade = cv2.CascadeClassifier('stone.xml')   #用訓練好的資料
        stoneRect = stonecascade.detectMutiScale(gray, 1.1, 3) #縮小1.1倍 當三張圖片都偵測到時 代表此物體已被辨識到

        x = -1  #假設未偵測到物體
        y = -1

        for(x, y, w, h) in stoneRect:
            cv2.rectangle(im, (x, y), (x+w, y+h), (0, 255, 0), 2)

        if(x >= 0 and y >= 0):  #當已偵測到物體 傳輸物體在照片位置的中心點(central_x,central_y)
            central_x = (2*x+w)/2   #算中心點
            central_y = (2*y+h)/2

            str = central_x + ',' + central_y
            ser.write(str)  #傳中心點座標給MEGA
            break
 
        cv2.imshow('detection',im)
        key=cv2.waitKey(5)
        if key==ord('q'):
            break
            
    cv2.destroyAllWindows()
 
 
 
if __name__ == '__main__':
    print("started")
    while True:
        with concurrent.futures.ProcessPoolExecutor() as executer:
                f= executer.submit(run)