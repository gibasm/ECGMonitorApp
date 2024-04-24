#!/bin/bash
# Run ecgmd service and start the frontend

DEV=\
'/dev/serial/by-id/usb-STMicroelectronics_STM32_STLink_066DFF383638534B43224635-if02'

if [[ $? -eq 2 ]] then
    DEV=$1 
fi

echo "Using device path: $DEV"

./build/services/ecgmd/ecgmd --wired $DEV &2> ecgmd.log &
sleep 1
./build/ecgm_app $@ &2> /dev/null & 
wait
