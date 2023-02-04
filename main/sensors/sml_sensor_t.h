//
// Created by yannick on 25.01.23.
//

#ifndef LOGME_INTERFACE_SML_SENSOR_T_H
#define LOGME_INTERFACE_SML_SENSOR_T_H

typedef enum mbus_status_e {
    MBUS_DEFAULT = 0,
    MBUS_ERROR = 1,
    MBUS_IDLE = 2,
    MBUS_READING = 3,
} mbus_status_e;


typedef struct sml_smart_meter_history_t {
    uint32_t day_24_kw[24];
    uint32_t week_7_kw[7];
    uint32_t month_30_kw[30];
} sml_smart_meter_history_t;

typedef struct sml_smart_meter_sensor_t {
    uint16_t id;
    char name[20];
    uint32_t count;
    uint32_t power;
    sml_smart_meter_history_t history;
} sml_smart_meter_sensor_t;


typedef struct sml_smart_meter_ws_t {
    uint16_t id;
    int32_t timestamp;
    uint32_t value;
} sml_smart_meter_ws_t;

typedef struct mbus_device_t {
    int pin_rx;
    int pin_tx;
    char name[20];
    char *data;
    mbus_status_e status;
} mbus_device_t;


#endif //LOGME_INTERFACE_SML_SENSOR_T_H
