# cadena-frio-aduino
Arduino code for cold chain Monitoring.

## Requirements

* arduino-cli 

## Compiling

``` bash

arduino-cli compile --fqbn esp8266:esp8266:nodemcuv2 .

```

## Upload
``` bash

arduino-cli upload -p <USB-PORT-HERE> --fqbn esp8266:esp8266:nodemcuv2 .

```


## Monitoring

``` bash

arduino-cli monitor -p <USB-PORT-HERE> --config baudrate=115200

```


### getting arduino's USB connected port

``` bash
// this will output all devices connected 
// usually something like: '/dev/ttyUSB0'
arduino-cli board list


```
