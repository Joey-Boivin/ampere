#!/bin/bash

Xephyr -br -ac -noreset -screen 800x800 :1 &
#Because Xephyr takes a bit of time to setup, we must sleep before launching the program
sleep 1

export DISPLAY=:1
build/ampere &