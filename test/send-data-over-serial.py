from serial import Serial
import time
serial = Serial('/dev/ttyUSB0', 9600)
for val1 in range(0, 255):
    for val in range(0,255):
        serial.write(val)
        print(f"Sending {val}. Iteration {val1}")

print("Done")