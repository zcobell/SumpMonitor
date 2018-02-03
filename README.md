# SumpMonitor

## Repository Status
[![Build Status](https://travis-ci.org/zcobell/SumpMonitor.svg?branch=master)](https://travis-ci.org/zcobell/SumpMonitor)
[![Coverity](https://scan.coverity.com/projects/9088/badge.svg)](https://scan.coverity.com/projects/zcobell-sumpmonitor)

## Disclaimer
I'm certain portions of this code can be written more elegantly and more safely. I'm happy to accept pull requests from anyone who'd like to contribute.

## Background
The SumpMonitor was devised after coming to the realization that the most critical piece of hardware in my house happened to be the sump pump. It only needed to fail me once before I waited for it to fail me a second time. After the second time, I decided I needed a solution that would keep me advised on its status and give me ample alert time when I was not home or traveling.

## Capabilities
The SumpMonitor has two important goals. The first is to track the water level data over time visually and, more importantly, the second is to alert the user via a push alert when a critical threshold is reached. The code allows the user to turn on and off capabilities individually so that the operating environment can be customized.

### Visual Water Level Tracking
Using a web based server, in my case, my personal public facing web server, a SQL database recieves data from the SumpMonitor at specified intervals. The code for the web page backend (written in php and javascript using [HighCharts](http://www.highcharts.com)) is posted in the `web` folder. The graphic below shows an example of the monitor working normally through the web interface.

![SumpWebInterface](https://github.com/zcobell/SumpMonitor/blob/master/img/sumpMonitorWeb.png)

### Cell Phone Push Alerts
Push alerts are delivered using the PushOver service. PushOver runs on IOS and Andoid based devices and gives someone like me without the infrastructure to generate an app that can receive a push alert the ability to use a simple API to send data to my cell phone. The code uses a single HTTP POST to send the messages to the user via PushOver. PushOver is a paid service and costs $5 per user per application. To me this was a no-brainer.

## Hardware
The SumpMonitor uses a few pieces of hardware to keep track of water levels. They are listed below.
1. Raspberry Pi - [Amazon](http://a.co/8r6OwYx)
2. Float Sensor - [Amazon](http://a.co/iENk9YJ)
3. eTape Water Level Sensor - [MiloneTech](https://milonetech.com/products/chemical-etape-assembly)
    * Note that I use the chemical version in hopes that it will last longer in the nasty water in the sump pit
4. Ultrasonic Sensor - [Amazon](http://a.co/eqKbz1X)
    * The code is set up to handle this sensor, however, I do not use it because the echo doesn't work well in my particular sump pit because there are too many obstacles. It is possible someone else may not have the issue so I've left this as an option.
5. MCP3008 Analog to Digital Converter - [Amazon](http://a.co/gazDuli)
6. Various small resistors, cables, etc that would be common to any basic RaspberryPi kit

## System Requirements
SumpMonitor requires your system running a version of Qt5, Qt5SQL-MySQL, and WiringPi.

## Operation
The code is set up so that it runs in a Linux environment on the RaspberryPi. My personal use case is to run a cron job every 10 minutes that checks if the code is still running. If the code is not running in the background, i.e. it crashed for some reason, the cron job restarts the code and a restart alert is sent via PushOver. The code will also send a confirmation-of-operation once per day to ensure that if you don't receive your morning alert, you know something is wrong. By default, this alert is set at 8am local time, however, the user can edit this. I also restart my RaspberryPi once per day anyway, so I coincide the cron restart job with the confirmation-of-operation alert.

### Command Line Switches
| Switch               | Function                                                    |
|----------------------|-------------------------------------------------------------|
| `-h`                 | displays a help dialog
| `-v`                 | displays the current version of the code
| `-q`                 | does not display verbose messages as screen output
| `--single`           | runs the monitor once and exits the program
| `--continuous`       | runs the monitor continuously at a user specified interval
| `--interval <s>`     | time in seconds between each monitoring data point
| `--push`             | use the PushOver alert service
| `--sql`              | post the data to a SQL database
|`--samples <n>`       | attempts to smooth the sensor response if the data is noisy
| `--float`            | enables the float sensor
| `--ultrasonic`       | enables the ultrasonic sensor
| `--etape`            | enables the eTape sensor
| `--time`             | hour (0-23) to send the daily status confirmation
| `--netcdf <file>`    | Write the data to a netcdf-4 formatted file

## Compiling

### WiringPi
This code uses the excellent [WiringPi](http://wiringpi.com/) library to communicate with the sensors. It is included as a submodule to this repository and can be downloaded by running:
```
git submodule update --init
```
With WiringPi installed, you're ready to compile the SumpMonitor.

### netCDF
NetCDF is currently a dependency of the code. Most Unix platforms make this readily available via a package manager. For Ubuntu/Debian type distributions, the following packages should be installed:
*  `netcdf-bin`
*  `libnetcdf-dev`

### Compiling SumpMonitor
Before compiling, you'll need to define information about your system. Two files will need to be altered.

#### Tokens
The `tokens.h` file in the `src` directory defines the passwords that SumpMonitor will use to access PushOver and your SQL server depending on which options you ultimately select. The structure of the SQL table that SumpMonitor will write to should have the format:

| Field Name  | Field Data Type |     Field Attributes        |
|-------------|-----------------|-----------------------------|
| id          | bigint          | Primary Key, Auto Increment |
| time        | datetime        | &nbsp;                      |
| waterlevel  | double          | &nbsp;                      |
| floatstatus | tinyint         | &nbsp;                      |

You do not need to define the values in `tokens.h` if you are not using them. You can simply leave them as their default values.

#### Pins
The `pins.h` file in the `src` directory defines the pins that SumpMonitor should expect to use for various GPIO devices.

### Sensor Settings
The `sensorlevel.h` file defines a number of variables related to the critical levels for the ultrasonic sensor and eTape sensor. It also is used to set the function used to llinearly interpolate from the raw sensor data provided by the eTape to water levels.

### CMake
The code is compiled using CMake. A Qt .pro file is also available, but is provided only as a development convenience for working with QtCreator.  

## Wiring Diagram
Coming Soon! Unfortunately, I need to return to my notes to see exactly what all I did, however, it is fairly straight forward to connect the MCP3008 to the eTape and float sensor to the specified GPIO pin.
