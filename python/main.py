# -*- coding: utf-8 -*-

try:
    import vrep
except:
    print '--------------------------------------------------------------'
    print '"vrep.py" could not be imported. This means very probably that'
    print 'either "vrep.py" or the remoteApi library could not be found.'
    print 'Make sure both are in the same folder as this file,'
    print 'or appropriately adjust the file "vrep.py"'
    print '--------------------------------------------------------------'

import time
import math
import sys

def start_simulator():
    print 'Program started'
    vrep.simxFinish(-1)
    clientID = vrep.simxStart('127.0.0.1', 19999, True, False, 5000, 5)

    if clientID != -1:
        print 'Connected to remote API server'
    else:
        print 'Failed connecting to remote API server'
        sys.exit('Program Ended')

    return clientID

def error_check(clientID, res):
    if res != vrep.simx_return_ok:
        print 'Failed to get sensor Handler'
        vrep.simxFinish(clientID)
        sys.exit('Program ended')

if __name__ == '__main__':
    clientID = start_simulator()

    res, objs = vrep.simxGetObjects(clientID, vrep.sim_handle_all, vrep.simx_opmode_blocking)
    error_check(clientID, res)

    time.sleep(1)

    res, JointDynamic = vrep.simxGetObjectHandle(clientID, "joint" , vrep.simx_opmode_blocking)
    error_check(clientID, res)
    res, Potentiometer = vrep.simxGetObjectHandle(clientID, "Potentiometer", vrep.simx_opmode_blocking)
    error_check(clientID, res)

    vrep.simxSetJointForce(clientID, JointDynamic, 4.8, vrep.simx_opmode_oneshot)

    while True:
        #Get Potentiometer
        res, pot_pos = vrep.simxGetJointPosition(clientID, Potentiometer, vrep.simx_opmode_oneshot)
        print "Potentiometer: %lf[deg]" % math.degrees(pot_pos)
        
        res, servo_pos = vrep.simxGetJointPosition(clientID, JointDynamic, vrep.simx_opmode_oneshot)
        print "Servo: %lf[deg]" % math.degrees(servo_pos)
        
        #Decide about joint velocities:
        linearVelocity = 0
        vrep.simxSetJointTargetVelocity(clientID, JointDynamic, linearVelocity, vrep.simx_opmode_oneshot)
        
        #Set pendulum Position
        if math.fabs(pot_pos) >= math.radians(50.0):
            print "Reset positions"
            """
            vrep.simxStopSimulation(clientID, vrep.simx_opmode_oneshot_wait)
            time.sleep(1)
            vrep.simxStartSimulation(clientID, vrep.simx_opmode_oneshot_wait)
            """
        
        time.sleep(0.001)

