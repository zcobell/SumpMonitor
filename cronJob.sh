#!/bin/bash
if ps -ef | grep -v grep | grep SumpMonitor ; then
    exit 0
else
    echo "Sump Monitor restarted at " $(date)
    /home/pi/sumpMonitor/SumpMonitor/SumpMonitor >> sumpLog.log &
fi
