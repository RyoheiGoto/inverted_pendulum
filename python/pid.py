# -*- coding: utf-8 -*-

from simulator import *
import time
import math

class PID(object):
    def __init__(self):
        self._kp = 790.0
        self._ki = 1.0
        self._kd = 3.0
        
        self._target_theta = 0.0
        self._sampling_time = 0.01
        
        self._theta0 = 0.0
        self._thetai = 0.0

    def init_status(self):
        self._theta0 = 0.0
        self._thetai = 0.0

    def get_velocity(self, theta):
        theta = self._target_theta - theta
        dtheta = (theta - self._theta0) / self._sampling_time
        self._theta0 = theta
        self._thetai += theta * self._sampling_time
        
        if self._thetai > 10000:
            self._thetai = 10000
        elif self._thetai < -10000:
            self._thetai = -10000
        
        duty_ratio = theta * self._kp + self._thetai * self._ki + dtheta * self._kd
        
        if duty_ratio > 1:
            duty_ratio = 1.0
        elif duty_ratio < -1:
            duty_ratio = -1.0
        
        return -duty_ratio

if __name__ == '__main__':
    pid = PID()

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
        res, theta = vrep.simxGetJointPosition(clientID, Potentiometer, vrep.simx_opmode_oneshot)
        
        linearVelocity = pid.get_velocity(theta)
        
        vrep.simxSetJointTargetVelocity(clientID, JointDynamic, linearVelocity, vrep.simx_opmode_oneshot)
        
        #Set pendulum Position
        if math.fabs(theta) >= math.radians(90.0):
            print "Reset positions"
            pid.init_status()
            vrep.simxStopSimulation(clientID, vrep.simx_opmode_oneshot_wait)
            time.sleep(1)
            wait_until_simulator_started(clientID)
        
        time.sleep(0.01)

