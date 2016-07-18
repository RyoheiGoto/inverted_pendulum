#!/bin/sh
sudo chgrp root /dev/ttyACM0
sudo cu -l /dev/ttyACM0 -s 9600
$@

