# -*- coding: utf-8 -*-

from simulator import *
import time
import math

if __name__ == '__main__':
    clientID = start_simulator()

    res, objs = vrep.simxGetObjects(clientID, vrep.sim_handle_all, vrep.simx_opmode_oneshot_wait)
    error_check(clientID, res)

    time.sleep(1)

    res, JointDynamic = vrep.simxGetObjectHandle(clientID, "joint" , vrep.simx_opmode_oneshot_wait)
    error_check(clientID, res)
    res, Potentiometer = vrep.simxGetObjectHandle(clientID, "Potentiometer", vrep.simx_opmode_oneshot_wait)
    error_check(clientID, res)

    vrep.simxSetJointForce(clientID, JointDynamic, 4.8, vrep.simx_opmode_oneshot)

    while True:
        #Get Potentiometer
        res, pot_pos = vrep.simxGetJointPosition(clientID, Potentiometer, vrep.simx_opmode_oneshot)
        print "Potentiometer: %lf[deg]" % math.degrees(pot_pos)
        
        res, servo_pos = vrep.simxGetJointPosition(clientID, JointDynamic, vrep.simx_opmode_oneshot)
        print "Servo: %lf[deg]" % math.degrees(servo_pos)
        
        #Decide about joint velocities
        linearVelocity = 0
        vrep.simxSetJointTargetVelocity(clientID, JointDynamic, linearVelocity, vrep.simx_opmode_oneshot)
        
        #Set pendulum Position
        if math.fabs(pot_pos) >= math.radians(50.0):
            print "Reset positions"
            vrep.simxStopSimulation(clientID, vrep.simx_opmode_oneshot_wait)
            time.sleep(1)
            wait_until_simulator_started(clientID)
        
        time.sleep(0.01)

