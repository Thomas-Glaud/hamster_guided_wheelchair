import cv2

import serial
import time

port = 'CoM5'  # Update this to your actual COM port
baudrate = 115200

ser = serial.Serial(
port='COM5',
baudrate=115200,
parity=serial.PARITY_NONE,
stopbits=serial.STOPBITS_ONE,
bytesize=serial.EIGHTBITS,
timeout=1
)

def main():

    camera_index = 0
    cap = cv2.VideoCapture(camera_index)
    ret, frame = cap.read()

    try:
        esp32 = serial.Serial(port='COM5', baudrate=115200, timeout=1)
        time.sleep(2)
    except serial.SerialException as e:
        print(f"Error: Could not open serial port {port}. {e}")
        return
                
    
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

        f_height, f_width = frame.shape[:2]
        #frame = imutils.resize(frame, width=frame.shape[1]//2)  # Resize to half the original width for faster processing

        
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        blurred = cv2.GaussianBlur(gray, (15, 15), 0)

        # 2. Thresholding: Turns the image purely black and white.
        # Adjust '100' depending on whether your hamster is darker or lighter than the floor.
        _, thresh = cv2.threshold(blurred, 50, 255, cv2.THRESH_BINARY_INV)

        # 3. Find the boundaries of the shapes (contours)
        contours, _ = cv2.findContours(thresh, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

        for contour in contours:
            # Ignore tiny spots like shadows or bedding chips
            if cv2.contourArea(contour) < 600: 
                continue

            # Get coordinates for the container holding the hamster blob
            (x, y, w, h) = cv2.boundingRect(contour)
            cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)

            x_center = x + w // 2
            y_center = y + h // 2

            if x_center < f_width // 3:
                position = "LEFT"
            elif x_center > 2 * f_width // 3:
                position = "RIGHT"
            else:
                position = "CENTER"
            
            y_distance = int(f_height // 2 - y_center)
            if abs(y_distance) > 100:
                if y_distance > 0:
                    speed = "FORWARD"
                else:
                    speed = "BACKWARD"
            
            
            print(f"The hamster is in the {position} and moving {speed}.")

            message = f"{position},{y_distance}\n"

            try:
                esp32.write(message.encode('utf-8'))
            except serial.SerialException as e:
                print(f"Error: Could not write to serial port {port}. {e}")
                return




        # Shows the actual tracking window
        cv2.imshow("Hamster Tracker", frame)
        # Visualizes exactly what the computer 'sees' to help you tune the threshold
        cv2.imshow("Thresh Visual", thresh)
          

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cap.release()
    cv2.destroyAllWindows()
    if 'esp32' in locals() and esp32.is_open:
        esp32.close()


if __name__ == "__main__":
    main()