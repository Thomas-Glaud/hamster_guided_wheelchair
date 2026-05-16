import cv2

def main(cascade_path):
    # 1. Verify the cascade file loaded correctly
    cascade = cv2.CascadeClassifier(cascade_path)
    if cascade.empty():
        print(f"Error: Could not load cascade classifier from {cascade_path}")
        return

    # 2. Initialize webcam (Change to 0 if 1 doesn't open)
    camera_index = 0
    cap = cv2.VideoCapture(camera_index)
    
    # Check if the webcam resource was successfully gripped by OpenCV
    if not cap.isOpened():
        print(f"Error: Could not open webcam at index {camera_index}.")
        print("Try changing camera_index to 0 if you only have one webcam.")
        return

    print("Press 'q' in the graphics window to quit.")

    while True:
        ret, frame = cap.read()
        if not ret:
            print("Error: Failed to grab frame from camera.")
            break
            
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        
        # 3. Detect objects (Optimized minNeighbors)
        objects = cascade.detectMultiScale(
            gray,
            scaleFactor=1.1,  
            minNeighbors=5,   # Increased to 5 to reduce false-positive boxes
            minSize=(30, 30)  
        )

        for (x, y, w, h) in objects:
            cv2.rectangle(frame, (x, y), (x+w, y+h), (0, 255, 0), 2)
            
        cv2.imshow("Live Detection", frame)
        
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
            
    cap.release()
    cv2.destroyAllWindows()


if __name__ == "__main__":
    haar_cascade_path = cv2.data.haarcascades + "haarcascade_frontalface_default.xml"
    main(haar_cascade_path)