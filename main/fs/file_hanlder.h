//
// Interface for File handling
//

#ifndef LOGME_INTERFACE_FILE_HANLDER_H
#define LOGME_INTERFACE_FILE_HANLDER_H

#include <esp_err.h>

esp_err_t file_handler_read_file(char *path, char *buffer, size_t buffer_size);

esp_err_t file_handler_write_file(char *path, cJSON *json_object);


#endif //LOGME_INTERFACE_FILE_HANLDER_H
