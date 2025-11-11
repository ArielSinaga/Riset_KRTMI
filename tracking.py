import cv2
import numpy as np
import time

CASCADES_PATH = '/home/ariel-sinaga/Documents/Opencv/opencv/data/haarcascades/haarcascade_frontalface_default.xml'
face_cascade = cv2.CascadeClassifier(CASCADES_PATH)

cap = cv2.VideoCapture(0)

if not cap.isOpened():
    print("Error: Tidak dapat membuka kamera.")
    exit()

frame_count = 0

while True:
    ret, frame = cap.read()
    if not ret:
        print("Error: Tidak dapat menerima frame. Exiting...")
        break

    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    
    faces = face_cascade.detectMultiScale(
        gray, 
        scaleFactor=1.1,       
        minNeighbors=5,      
        minSize=(30, 30)       
    )

    wajah_terdeteksi = False
   
    if len(faces) > 0:
        wajah_terdeteksi = True
        
        (x, y, w_box, h_box) = faces[0] 
        
        color = (0, 0, 255) 
        cv2.rectangle(frame, (x, y), (x + w_box, y + h_box), color, 2)
        cv2.putText(frame, "Human", (x, y - 10), 
                    cv2.FONT_HERSHEY_SIMPLEX, 0.7, color, 2)
    
    if not wajah_terdeteksi:
         cv2.putText(frame, "", (20, 40), 
                    cv2.FONT_HERSHEY_SIMPLEX, 0.7, (255, 255, 0), 2)

    cv2.imshow('Face Detection (No Tracker)', frame)
    
    frame_count += 1

    if cv2.waitKey(1) & 0xFF == 13: 
        break

cap.release()
cv2.destroyAllWindows()