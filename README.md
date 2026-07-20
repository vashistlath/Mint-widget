# Mint widget

A Linux `(linux mint specifically)`desktop system widget written in `C` with a planned  `raylib` frontend.

## Current backend features

* RAM usage using the file(`/proc/meminfo`)
* CPU counters using the file (`/proc/stat`)
* Uptime using the file (`/proc/uptime`)
* Load average using the file (`/proc/loadavg`)
* CPU temperature using the file (`/sys/class/thermal/thermal_zone0/temp`)
* Network RX/TX counters using the file (`/proc/net/dev`)

## Planned features

* Real-time CPU usage graph
* Network graph
* UI with raylib
* Frameless desktop widget
* Modular backend 

## Status

Built a working Linux system dashboard backend in C that shows live RAM, CPU, uptime, load average, and temperature data . Next step is adding a live refreshing terminal UI with colors, progress bars, and alert indicators that refreshes every 15 mins or so.
