![logme-header](images/logme_header.png)
[![Open Source Love](https://badges.frapsoft.com/os/v1/open-source.svg?v=103)](https://github.com/ellerbrock/open-source-badges/)
[![test](https://github.com/DerZwergGimli/logme_interface/actions/workflows/buildtest.yml/badge.svg)](https://github.com/DerZwergGimli/logme_interface/actions/workflows/buildtest.yml)
[![deploy](https://github.com/DerZwergGimli/logme_interface/actions/workflows/createrelease.yml/badge.svg)](https://github.com/DerZwergGimli/logme_interface/actions/workflows/createrelease.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

# LogME (LogMeter)

[...] unleash the power of your smart-meters!

# About

This project is written with ESP-IDFv5.0 using an ESP-32WROOM

This project is for reading data using Optical-Interfaces from the following devices:

- [Smart meter]('https://en.wikipedia.org/wiki/Smart_meter') via SmartMeterLanguage(SML) - Serial
- [Heat meter]('https://en.wikipedia.org/wiki/Heat_meter') [under-construction]

## Features

- WebUI
- Restful-API
- Create a default fallback AP
- WiFi-Setup can be configured via WEB-UI
- HomeAssist Plugin (planned)

---

## Webserver

The WebUI is written with Vite.js (Vue3.js) and can be found under `./web-ui/`

### Endpoints (REST)

LogMe supports some basic REST-Requests for providing data back and forth from the WebUI.
(more: [logme_endpoints.json](web-ui/json-server/logme_endpoints.json))

#### GET

- `/wifi`
- `/system`
- `/sensors`
- `/sensorstore.json`

#### PUT

- `/restart`
- `/wifireset`
- `/wificonfig`
- `/sensorsave`
- `/sensorhistoryclear`
- `/sensor_edit_name/#`
- `/sensor_edit_description/#`

#### Websocket

- `/websocket`

## Flash Project

[for Linux]

1. `mkdir /home/esp && cd /home/$USER/esp`
2. `git clone https://github.com/espressif/esp-idf.git`
3. Clone this repo...
    1. Go back into your project dir (this repo)
    2. `/home/$USER/esp/esp-idf/install.sh`
    3. `. /home/$USER/esp/esp-idf/export.sh`
4. You should now be able to run:
    1. `idf.py build`
    2. `idf.py flash`
    3. `idf.py monitor`

## Credits

Many thanks to the following repos:

- [libmbus](https://github.com/rscada/libmbus)
- [SoftwarSerial](https://github.com/plerup/espsoftwareserial)

## Useful Links

[M-Bus datafram decoder](https://dev-lab.github.io/tmbus/tmbus.htm)

GET-GIT-Vserion

git tag --sort=-v:refname --list "v[0-9]*" | head -n 1 | cut -c 2-
