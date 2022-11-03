import Adafruit_BBIO.GPIO as GPIO
import time

GPIO.setup("PS_10", GPIO.OUT)

while(True):
    for i in range(12):
        if i % 2 == 0: GPIO.output("P8_10", GPIO.HIGH)
        else: GPIO.output("P8_10", GPIO.LOW)
        if i > 4: t = 1
        else: t = 0.2
        time.sleep(t)