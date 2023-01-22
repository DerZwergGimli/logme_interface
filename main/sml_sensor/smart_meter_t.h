#ifndef SMART_METER_T_H
#define SMART_METER_T_H

typedef struct smart_meter_history
{
    int16_t day_24_kw[24];
    int16_t week_7_kw[7];
    int16_t month_30_kw[30];
} smart_meter_history_t;

typedef struct smart_meter_sensor
{
    int16_t id;
    char name[20];
    int16_t count;
    int16_t power;
    smart_meter_history_t history;
} smart_meter_sensor_t;

typedef struct smart_meter_ws
{
    int16_t id;
    int32_t timestamp;
    int16_t value;
} smart_meter_ws_t;

#endif