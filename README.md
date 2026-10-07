# cadena-frio-aduino
Arduino code for cold chain Monitoring.

## Requirements

```

arduino-cli

```


## Compiling

```

arduino-cli compile --fqbn esp8266:esp8266:nodemcuv2 .

```

## Upload
```

arduino-clupload -p /dev/ttyUSB0 --fqbn esp8266:esp8266:nodemcuv2 .

```


## Monitoring

```

arduino-clmonitor -p /dev/ttyUSB0 --config baudrate=115200

```
