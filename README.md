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
