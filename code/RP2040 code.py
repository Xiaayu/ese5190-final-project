'''
test code to control the servo
'''

import time
from board import SCL, SDA
import busio
from adafruit_motor import servo
from adafruit_pca9685 import PCA9685
import board
from digitalio import DigitalInOut, Direction

#uart = busio.UART(board.TX, board.RX, baudrate=115200)
A0 = DigitalInOut(board.A0)
A1 = DigitalInOut(board.A1)
A2 = DigitalInOut(board.A2)
A3 = DigitalInOut(board.A3)

A0.direction = Direction.INPUT
A1.direction = Direction.INPUT
A2.direction = Direction.INPUT
A3.direction = Direction.INPUT

i2c = busio.I2C(SCL, SDA)
# PCA9685
pca = PCA9685(i2c, address=0x40) #default 0x40
pca.frequency = 50

# set the degree of channel num and angle
def set_angle(ID, angle):
    servo_angle = servo.Servo(pca.channels[ID], min_pulse=500, max_pulse=2400,actuation_range=180)
    servo_angle.angle = angle

# initialize angle variables
# servo0 (lmax = 15, midd = 80, rmax = 145)
servo0 = 80
# servo1 ()
servo1 = 140
servo2 = 160

# set angle smoothly
def set_angle_smooth(ID, angle):
    global servo0
    global servo1
    global servo2
    # set servo0
    if (ID == 0):
        if (angle > servo0):
            while (angle > servo0):
                servo0 += 1;
                set_angle(ID, servo0);
                time.sleep(0.02);
        elif (angle < servo0):
            while (angle < servo0):
                servo0 -= 1;
                set_angle(ID, servo0);
                time.sleep(0.02);
    # set servo1
    elif (ID == 1):
        if (angle > servo1):
            while (angle > servo1):
                servo1 += 1;
                set_angle(ID, servo1);
                time.sleep(0.02);
        elif (angle < servo1):
            while (angle < servo1):
                servo1 -= 1;
                set_angle(ID, servo1);
                time.sleep(0.02);
    # set servo2
    elif (ID == 2):
        if (angle > servo2):
            while (angle > servo2):
                servo2 += 1;
                set_angle(ID, servo2);
                time.sleep(0.02);
        elif (angle < servo2):
            while (angle < servo2):
                servo2 -= 1;
                set_angle(ID, servo2);
                time.sleep(0.02);
    time.sleep(0.01)


# test code
def test(channel):
    for i in range(180):
        set_angle(channel , i)
        time.sleep(0.03)
    time.sleep(0.1)
    for i in range(180):
        set_angle(channel , 180 - i)
        time.sleep(0.03)
    time.sleep(0.1)

# initialize
def init_smooth():
    global servo0
    global servo1
    global servo2
    set_angle_smooth(0, 80)
    set_angle_smooth(1, 140)
    set_angle_smooth(2, 160)
    servo0 = 80
    servo1 = 140
    servo2 = 160
    time.sleep(0.02)

def init():
    set_angle(0, 80)
    set_angle(1, 140)
    set_angle(2, 160)
    time.sleep(0.01)

# up
def up():
    global servo1
    global servo2
    if (servo1 > 100 and servo2 > 120):
        servo1 -= 1
        servo2 -= 1
    set_angle(1, servo1)
    set_angle(2, servo2)
    time.sleep(0.03)

# down
def down():
    global servo1
    global servo2
    if (servo1 < 160 and servo2 < 180):
        servo1 += 1
        servo2 += 1
    set_angle(1, servo1)
    set_angle(2, servo2)
    time.sleep(0.03)

# left
def left():
    global servo0
    if (servo0 < 145):
        servo0 += 1
    set_angle(0, servo0)
    time.sleep(0.03)

# right
def right():
    global servo0
    if (servo0 > 15):
        servo0 -= 1
    set_angle(0, servo0)
    time.sleep(0.03)



if __name__ == "__main__":
    # initialize the system
    init()
    # things are ready
    time.sleep(1)

    # define two counter to change mode
    counter = 0 # horizontal or vertical roration mode
    initialcounter = 0 # initial rotation mode
    jud = 0 # 0 turn right; 1 turn left

    #set_angle_smooth(0, 80)
    #set_angle_smooth(1, 30)
    #set_angle_smooth(2, 50)
    while (1):
        if ((not A0.value) and (not A1.value) and (not A2.value) and (not A3.value)):

            initialcounter += 1

            if (initialcounter > 99):
                if (initialcounter == 100):
                    init_smooth()

                print(str(initialcounter) + ' and ' + str(counter))
                # horizontal rotation
                if (counter < 3):

                    if (servo0 == 80):
                        counter += 1
                    if (jud == 0 and servo0 > 15 and counter < 3):
                        servo0 -= 1
                        if (servo0 == 15):
                            jud = 1
                    elif (jud == 1 and servo0 < 145 and counter < 3):
                        servo0 += 1
                        if (servo0 == 145):
                            jud = 0


                # vertical rotation
                elif (counter > 2):
                    if (servo2 == 160):
                        counter += 1
                        if (counter == 5):
                            counter = 0

                    if (jud == 0 and servo2 > 120 and counter < 5):
                        servo0 -= 1
                        if (servo0 == 120):
                            jud = 1
                    elif (jud == 1 and servo2 < 180 and counter < 5):
                        servo0 += 1
                        if (servo0 == 180):
                            jud = 0

            time.sleep(0.01)


        elif ((not A0.value) and (not A1.value) and (not A2.value) and (A3.value)):
            print('left up')
            left()
            up()

        elif ((not A0.value) and (not A1.value) and (A2.value) and (not A3.value)):
            initialcounter = 0
            counter = 0
            print('up')
            up()

        elif ((not A0.value) and (not A1.value) and (A2.value) and (A3.value)):
            initialcounter = 0
            counter = 0
            print('right up')
            right()
            up()

        elif ((not A0.value) and (A1.value) and (not A2.value) and (not A3.value)):
            initialcounter = 0
            cpunter = 0
            print('left')
            left()

        elif ((not A0.value) and (A1.value) and (not A2.value) and (A3.value)):
            print('middle')
            initialcounter = 0
            counter = 0

        elif ((not A0.value) and (A1.value) and (A2.value) and (not A3.value)):
            initialcounter = 0
            counter = 0
            print('right')
            right()

        elif ((not A0.value) and (A1.value) and (A2.value) and (A3.value)):
            initialcounter = 0
            counter = 0
            print('left down')
            left()
            down()

        elif ((A0.value) and (not A1.value) and (not A2.value) and (not A3.value)):
            initialcounter = 0
            counter = 0
            print('down')
            down()

        elif ((A0.value) and (not A1.value) and (not A2.value) and (A3.value)):
            initialcounter = 0
            counter = 0
            print('right down')
            right()
            down()


