#!/bin/bash
n=$(ps -ef | grep -v grep | grep -v cronJob.sh  | grep SumpMonitor | wc -l)
if [ $n == 1 ] ; then
    exit 0
else
    echo "Sump Monitor restarted at " $(date)
    /home/pi/sumpMonitor/SumpMonitor/SumpMonitor -q -c -i 300 -p -n -t 8 >> /home/pi/sumpMonitor/sumpLog.log 2>>/home/pi/sumpMonitor/sumpLog.log &
fi
