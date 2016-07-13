# -*- coding: utf-8 -*-

try:
    import vrep
except:
    print ('--------------------------------------------------------------')
    print ('"vrep.py" could not be imported. This means very probably that')
    print ('either "vrep.py" or the remoteApi library could not be found.')
    print ('Make sure both are in the same folder as this file,')
    print ('or appropriately adjust the file "vrep.py"')
    print ('--------------------------------------------------------------')

import time
import sys
import ctypes

print 'Program started'
vrep.simxFinish(-1)
clientID = vrep.simxStart('127.0.0.1', 19999, True, True, 5000, 5)

if clientID != -1:
    print 'Connected to remote API server'
else:
    print 'Failed connecting to remote API server'
    sys.exit('Program Ended')
    
nominalLinearVelocity = 0.3
    
res, objs = vrep.simxGetObjects(clientID, vrep.sim_handle_all,vrep.simx_opmode_blocking)
if res == vrep.simx_return_ok:
    print 'Number of objects in the scene: %d' % len(objs)
else:
    print 'Remote API function call returned with error code: %d' % res

time.sleep(2)

startTime = time.time()
vrep.simxGetIntegerParameter(clientID,vrep.sim_intparam_mouse_x,vrep.simx_opmode_streaming)

#res, display = vrep.simxGetUIHandle(clientID, "sensorDisplay", vrep.simx_opmode_blocking)

res, JointDynamic = vrep.simxGetObjectHandle(clientID, "joint" , vrep.simx_opmode_blocking)
res, JointPotentiometer = vrep.simxGetObjectHandle(clientID, "Potentiometer", vrep.simx_opmode_blocking)

if res != vrep.simx_return_ok:
    print 'Failed to get sensor Handler'
    vrep.simxFinish(clientID)
    sys.exit('Program ended')
	
    #Get Potentiometer
    pos = vrep.simxGetJointPosition(clientID, Potentiometer, vrep.simx_opmode_oneshot)
   
    #Decide about joint velocities:
    s = 1.0
    linearVelocity  = nominalLinearVelocity * s
    
    vrep.simxSetJointTargetVelocity(clientID, JointDynamic, linearVelocity, vrep.simx_opmode_oneshot)
    
    #Set pendulum Position
    if pos <= 50:
        vrep.simxSetJointPosition(clineID,Potentiometer, 0, vrep.simx_opmode_oneshot)
    
    time.sleep(0.005)

