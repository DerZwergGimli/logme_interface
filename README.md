![logme-header](images/logme_header.png)

# LogME (LogMeter)

[...] unleash the power of your smart-meters!

# About

This project is written with ESP-IDFv5.0 using an ESP-32WROOM

This project is for reading data using OptialInterfaces from the following devices:

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

#### PUT

- `/wificonfig`

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
