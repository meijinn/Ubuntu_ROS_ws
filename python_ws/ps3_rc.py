#! /usr/bin/env python
# coding: utf-8
# coding=utf-8
# -*- coding: utf-8 -*-


import pygame,serial,sys
from pygame.locals import *

ser = serial.Serial("/dev/ttyUSB0",115200,timeout=1)

pygame.joystick.init()


try:# 例外があるかもしれないが、実行したい処理
    j = pygame.joystick.Joystick(0) # create a joystick instance
    j.init() # init instance
    print ("Joystick name: " + j.get_name())
    print ("Number of buttons: " + str(j.get_numbuttons()))
except pygame.error:# 例外時の処理
    print ('Joystick is not found')
    pygame.quit()
    sys.exit()


def main():

    pygame.init()
    while 1: # ②ループ
        for e in pygame.event.get(): # イベントチェック
            #if e.type == JOYHATMOTION: # 終了が押された？
            #    pygame.quit()
            #    sys.exit()
            #if (e.type == KEYDOWN):
            #    if (e.key  == K_ESCAPE): # ESCが押された？
            #        pygame.quit()
            if e.type == pygame.locals.JOYAXISMOTION: # 7 ゲームパッドのボタンイベント
                gamepad_event()


def gamepad_event():

    Senddata = joyget()
    for i in range(2): # Senddata[0] = Steering Senddata[1] = vel
        low = Senddata[i]&31
        high = (Senddata[i]>>5)&31
        head = ((Senddata[i]>>10)&31)+128+i
        ser.write(chr(head))
        ser.write(chr(high))
        ser.write(chr(low))
    

def joyget():    
 # Joystick関連のイベントチェック
    input_array = []
    input_array = getUserInput(j.get_axis(0),j.get_axis(4),j.get_axis(3))
    # steeringとthrottleの戻り値を得る。

    steering = constrain(input_array[0])
    throttle = input_array[1]
    # 戻り値を配列に代入

    sys.stdout.write('\r'+'steering: '+str(steering)+'  '+'throttle: '+str(throttle) )
    sys.stdout.flush()
    # print('\r' + 'steering: '+str(steering)+' '+'throttle: '+''+str(throttle))

    # print ('steering ' + str(steering))
    # print 'gas and brake ' + str(gas) +' , '+ str(brake)
    # print ('throttle ' + str(throttle))

    return steering,throttle


def getUserInput(ster_ax,gas_ax,brake_ax):
    
    steering = int(((j.get_axis(0)*180)+180)/2)
    gas = (j.get_axis(4))+93
    brake = (j.get_axis(3)*-1*21+207)/2
    throttle = int((gas+brake)/2)
    return steering,throttle


def constrain(ster_data):

    if(ster_data > 180):
        steering = 180
        return steering
    if(ster_data < 0):
        steering = 0
        return steering
    else:
        return ster_data


# def serial_write(ster,vel):


#    ser.write(chr(ster))
#    ser.write(chr(vel))

if __name__ == '__main__': main() # モジュールを直接実行したら、main()を実行
# end of file