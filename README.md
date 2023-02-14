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

- `/system`

```json 
{
   "info": {
      "target": "esp32",
      "cores": 2,
      "major_rev": 3,
      "minor_rev": 0,
      "flash_size_MB": 8,
      "total_heap": 272232,
      "free_heap": 50612,
      "uptime_ms": 435915,
      "time": "Tue Feb 14 21:38:04",
      "bin_version": "0.0.4"
}
```

- `/sensors`
  Response for one sensor

```json
[
  {
    "name": "Sensor_1",
    "id": 0,
    "description": "best sensor ever",
    "status": 2,
    "pin_rx": 0,
    "pin_tx": 0,
    "baudrate": 0,
    "primary_address": 0,
    "secondary_address": 0,
    "dashboard_config_ids": [
      [
        0,
        2,
        -1,
        -1,
        -1,
        -1,
        -1,
        -1,
        -1,
        -1
      ],
      [
        0,
        1,
        2,
        3,
        4,
        5,
        6,
        7,
        -1,
        -1
      ]
    ],
    "data": {
      "salve_info": {
        "id": 22975096,
        "manufacturer": "SEN",
        "version": 29,
        "product_name": "",
        "medium": "Heat: Inlet",
        "access_number": 136,
        "status": "00",
        "signature": "0000"
      },
      "slave_data": [
        {
          "id": 0,
          "function": "Instantaneous value",
          "storage_number": 0,
          "unit": "Wh",
          "quantity": "Energy",
          "value": 0
        },
        {
          "id": 1,
          "function": "Instantaneous value",
          "storage_number": 0,
          "unit": "m^3",
          "quantity": "Volume",
          "value": 0.013
        }
      ]
    }
  }
]
```

- `/default_sensor_config.json`

```json
[
  {
    "id": "001",
    "name": "Sensor_1",
    "description": "best sensor ever",
    "pin_rx": 4,
    "pin_tx": 5,
    "baudrate": 2400,
    "primary_address": 1,
    "secondary_address": 0,
    "dashboard_config_ids": [
      [
        0,
        2
      ],
      [
        0,
        1,
        2,
        3,
        4,
        5,
        6,
        7
      ]
    ]
  }
]
```

#### PUT

- `/restart`
- `/wifireset`
- `/wificonfig`
- `/sensorsave`
- `/sensorhistoryclear`
- `/sensor_edit_name/#`
- `/sensor_edit_description/#`

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
