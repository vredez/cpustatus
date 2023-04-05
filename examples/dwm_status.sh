#!/bin/bash

CORES=$(nproc)

while true; do
    SEP="~"
    CPU=$(cpustatus $CORES ' ' '.' ':' "|" <<< "$PREV_STAT")
    MEM=$(used_mem)
    CLOCK=$(date +"%a@%V, %d.%m.%Y $SEP %H:%M:%S")

    xsetroot -name " $CPU $MEM $SEP $CLOCK "

    PREV_STAT=$(</proc/stat)
    sleep 1
done
