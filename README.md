# LogME

This project is for reading data from:

- [Smart meter]('https://en.wikipedia.org/wiki/Smart_meter')
- [Heat meter]('https://en.wikipedia.org/wiki/Heat_meter')

---

## Webserver

The WebUI is written with Vite.js (Vue3.js) and can be found under `./web-ui/`

### Endpoints (REST)

LogMe supports some basic REST-Requests for providing data back and forth from the WebUI.

#### GET

- `/wifi`
- `/system`
- `/sensors`

#### PUT

- `/wifiConfig`

#### Websocket

- `/websocket`

## Development

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
