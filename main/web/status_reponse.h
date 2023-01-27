//
// Created by yannick on 27.01.23.
//

#ifndef LOGME_INTERFACE_STATUS_REPONSE_H
#define LOGME_INTERFACE_STATUS_REPONSE_H

#include "cJSON.h"

typedef enum status_response_t {
    STATUS_OK = 0,
    STATUS_ERROR = 1,
    STATUS_WARNING = 2,
} status_response_t;

static void json_status_response_create(cJSON *json_response, status_response_t status, char *message) {


    switch (status) {
        case STATUS_OK:
            cJSON_AddNumberToObject(json_response, "code", status);
            cJSON_AddStringToObject(json_response, "status", "STATUS_OK");
            break;
        case STATUS_ERROR:
            cJSON_AddNumberToObject(json_response, "code", status);
            cJSON_AddStringToObject(json_response, "status", "STATUS_ERROR");
            break;
        case STATUS_WARNING:
            cJSON_AddNumberToObject(json_response, "code", status);
            cJSON_AddStringToObject(json_response, "status", "STATUS_WARNING");
            break;
        default:
            break;
    }
    cJSON_AddStringToObject(json_response, "message", message);

}

#endif //LOGME_INTERFACE_STATUS_REPONSE_H
