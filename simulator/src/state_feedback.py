from simulator import *
import time
import math

class StateFeedback(object):
    def __init__(self):
        self._k1 = 0.0
        self._k2 = 0.0
        self._k3 = 0.0
        self._k4 = 0.0
        
        self._target_theta = 0.0
        self._sampling_time = 0.01
        
        self._x0 = 0.0
        self._theta0 = 0.0

    def init_status(self):
        self._x0 = 0.0
        self._theta0 = 0.0

    def get_velocity(self, theta, x):
        dx = (x - self._x0) / self._sampling_time
        self._x0 = x
        
        theta = self._target_theta - theta
        dtheta = (theta - self._theta0) / self._sampling_time
        self._theta0 = theta
        
        duty_ratio = x * self._k1 + dx * self._k2 + theta * self._k3 + dtheta * self._k4
        
        if duty_ratio > 100.0:
            duty_ratio = 100.0
        elif duty_ratio < -100.0:
            duty_ratio = -100.0
        
        return -duty_ratio

if __name__ == '__main__':
    sf = StateFeedback()

    clientID = start_simulator()

    res, objs = vrep.simxGetObjects(clientID, vrep.sim_handle_all, vrep.simx_opmode_oneshot_wait)
    error_check(clientID, res)

    time.sleep(1)

    res, JointDynamic = vrep.simxGetObjectHandle(clientID, "joint" , vrep.simx_opmode_oneshot_wait)
    error_check(clientID, res)
    res, Potentiometer = vrep.simxGetObjectHandle(clientID, "Potentiometer", vrep.simx_opmode_oneshot_wait)
    error_check(clientID, res)

    vrep.simxSetJointForce(clientID, JointDynamic, 0.384, vrep.simx_opmode_oneshot)

    while True:
        res, x = vrep.simxGetJointPosition(clientID, JointDynamic, vrep.simx_opmode_oneshot)
        res, theta = vrep.simxGetJointPosition(clientID, Potentiometer, vrep.simx_opmode_oneshot)
        
        linearVelocity = sf.get_velocity(theta, x)
        
        vrep.simxSetJointTargetVelocity(clientID, JointDynamic, linearVelocity, vrep.simx_opmode_oneshot)
        
        if math.fabs(theta) >= math.radians(90.0):
            print "Reset positions"
            sf.init_status()
            vrep.simxStopSimulation(clientID, vrep.simx_opmode_oneshot_wait)
            time.sleep(1)
            wait_until_simulator_started(clientID)
        
        time.sleep(0.01)

