# cpustatus
A small tool generating a text-based status line for CPU core usage bars based on ```/proc/stat``` data.

## Installation
```
sudo make install
```
## Usage
You can specify the amount of CPU cores to watch and bar-glyphs to use. The provided bar-glyphs will be used to display core usage ranges: if two glyphs are specified, the first one is used for the range ```[0, 0.5]``` and the second for the range ```(0.5, 1]```.
```
OLD_PROC_STAT | cpustatus CORECOUNT [GLYPH0 GLYPH1 ...]

```

## Examples
### Shell
```cpustatus``` is meant to be used inside a shell script, as it needs previous data from ```/proc/stat``` piped into ```stdin```
```
#!/bin/bash

CORES=$(nproc)

while true; do
    STATUS=$(cpustatus $CORES ' ' '.' ':' "|" <<< "$PREV_STATUS")
    PREV_STATUS=$(</proc/stat)
    echo -ne "\r$STATUS"
    sleep 1
done

```
Output running a moderate CPU stresstest (16 cores, glyphs: ```' ', '.', ':', '|'```):
```
  . : : | : | | | | | . | | | |
```
### dwm status
![dwm status screenshot](dwm_status.png)
```
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
```
