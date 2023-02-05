//
//
//

typedef enum mbus_status_e {
    MBUS_DEFAULT = 0,
    MBUS_ERROR = 1,
    MBUS_IDLE = 2,
    MBUS_READING = 3,
} mbus_status_e;


#ifndef LOGME_INTERFACE_MBUS_SENSOR_T_H
#define LOGME_INTERFACE_MBUS_SENSOR_T_H
typedef struct mbus_device_t {
    int id;
    char description[50];
    int pin_rx;
    int pin_tx;
    int baudrate;
    int primary_address;
    int secondary_address;
    char name[20];
    char *data;
    mbus_status_e status;
    int dashboard_config_ids[2][10];
} mbus_device_t;

#endif //LOGME_INTERFACE_MBUS_SENSOR_T_H
