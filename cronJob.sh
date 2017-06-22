#!/bin/bash

args="-c -i 30 -a 10 -p -n"

if ps -ef | grep -v grep | grep SumpMonitor ; then
    exit 0
else
    echo "Sump Monitor restarted at " $(date)
    /home/pi/sumpMonitor/SumpMonitor/SumpMonitor $(args) >> sumpLog.log &
fi
