#!/bin/bash

CORES=$(nproc)

while true; do
    STATUS=$(cpustatus $CORES ' ' '.' ':' "|" <<< "$PREV_STATUS")
    PREV_STATUS=$(</proc/stat)
    echo -ne "\r$STATUS"
    sleep 1
done
