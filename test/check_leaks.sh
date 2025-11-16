#!/bin/bash
./test/test_memory &
PID=$!
sleep 0.8
leaks $PID 2>&1
wait $PID
