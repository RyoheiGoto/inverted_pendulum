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

def wait_until_simulator_started(clientID):
    while True:
        try:
            if vrep.simxStartSimulation(clientID, vrep.simx_opmode_oneshot_wait) == vrep.simx_return_ok:
                break
        except KeyboardInterrupt:
            sys.exit('Program Ended')

