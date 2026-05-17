import cv2

def find_camera_indices():
    index = 0
    arr = []
    # Checking indices 0 through 4
    while index < 5: 
        cap = cv2.VideoCapture(index)
        
        if cap.isOpened():
            success, frame = cap.read()
            if success:
                arr.append(index)
            
            # This must run even if success is False
            cap.release() 
        
        index += 1
        
    return arr

print("Available camera indices:", find_camera_indices())