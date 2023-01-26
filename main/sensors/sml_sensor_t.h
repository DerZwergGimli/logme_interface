//
// Created by yannick on 25.01.23.
//

#ifndef LOGME_INTERFACE_SML_SENSOR_T_H
#define LOGME_INTERFACE_SML_SENSOR_T_H


typedef struct sml_smart_meter_history_t {
    int16_t day_24_kw[24];
    int16_t week_7_kw[7];
    int16_t month_30_kw[30];
} sml_smart_meter_history_t;

typedef struct sml_smart_meter_sensor_t {
    int16_t id;
    char name[20];
    int32_t count;
    int16_t power;
    sml_smart_meter_history_t history;
} sml_smart_meter_sensor_t;


typedef struct sml_smart_meter_ws_t {
    int16_t id;
    int32_t timestamp;
    int16_t value;
} sml_smart_meter_ws_t;


#endif //LOGME_INTERFACE_SML_SENSOR_T_H
