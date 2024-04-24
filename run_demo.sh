#!/bin/bash
# Run the demo service and start the frontend

./build/services/demo/ecgmd_demo --file ./demo.txt > ecgmd.log &
sleep 1
./build/ecgm_app > app.log &
wait
