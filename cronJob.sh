#!/bin/bash

if ps -ef | grep -v grep | grep -v cronJob.sh  | grep SumpMonitor ; then
    exit 0
else
    echo "Sump Monitor restarted at " $(date)
    /home/pi/sumpMonitor/SumpMonitor/SumpMonitor -c -i 30 -p -n >> sumpLog.log &
fi
