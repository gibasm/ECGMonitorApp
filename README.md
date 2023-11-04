# ECG Monitor App

[![GNU/Linux CI badge](https://github.com/mbedded-mike/ECGMonitorApp/actions/workflows/ci_linux.yml/badge.svg)](https://github.com/mbedded-mike/ECGMonitorApp/actions/workflows/ci_linux.yml)

desktop app for my ECGMonitor hardware

![Rough, layered architecture depiction of the app](media/layers.png)

# Building

## Dependencies

- GNU/Linux machine

- compiler with C++20 support (gcc/clang)

- CMake 3.20

- GNU Make or Ninja

- SDL2

## Steps

1.
```
mkdir build && cd build
```

2.
```
cmake ..
```

3.
```
make -j`nproc`
```

4.
```
./ecgm_app
```

## Choosing a service

A service (in some cases called a daemon, because optionally you can use it like so in your system) is essentially a backend for the app,
it acts as a bridge between the OS/device drivers and the frontend. You might choose one based on your hardware & software setup.

### Demo service 

This service is great for software only check if app actually works as as a whole on your system.

```
./services/demo/ecgmd_demo --file <text file path>
```

The signal should be encoded as floating points in plain text as such:

```
-0.1
0.25
-0.35
...
```

This service does not respond to anything other than *START* / *STOP*,
and sampling frequency of 360 *Hz* is assumed.


### ecgmd

(work in progress)

## Running all of it 

1. Run the desired service (see: [Choosing a service](#choosing-a-service))
2. Then run the *ecgm_app*  

