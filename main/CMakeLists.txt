

idf_component_register(SRCS "main.c"
        "fs/file_hanlder.c"
        "fs/config.c"
        "wifi/wifi_manager.c"
        "wifi/dns_server.c"
        "wifi/json.c"
        "wifi/nvs_sync.c"
        "web/rest_server.c"
        "system/system_info.c"
        "sensors/sensor_manager.c"
        "sensors/sensor_helper.c"
        "timer/timer_manager.c"
        "mqtt/mqtt_manager.c"
        "fs/config.c"
        "web/rest_server_helper.c"
        "web/handler_get/rest_get_handler.c"
        "web/handler_post/rest_post_handler.c"
        INCLUDE_DIRS "."
        )

if (CONFIG_EXAMPLE_WEB_DEPLOY_SF)
    set(WEB_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../web-ui")
    if (EXISTS ${WEB_SRC_DIR}/dist)
        spiffs_create_partition_image(www ${WEB_SRC_DIR}/dist FLASH_IN_PROJECT)
    else ()
        message(FATAL_ERROR "${WEB_SRC_DIR}/dist doesn't exit. Please run 'npm run build' in ${WEB_SRC_DIR}")
    endif ()
endif ()

## Link to mount storage_db
#set(DB_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../db")
#if (EXISTS ${DB_SRC_DIR})
#    spiffs_create_partition_image(db ${DB_SRC_DIR} FLASH_IN_PROJECT)
#else ()
#    message(FATAL_ERROR "${DB_SRC_DIR} doesn't exit. Please run 'npm run build' in ${DB_SRC_DIR}")
#endif ()

if (DEFINED ENV{INSIGHTS_PATH})
    set(INSIGHTS_PATH $ENV{INSIGHTS_PATH})
else ()
    set(INSIGHTS_PATH ${CMAKE_CURRENT_LIST_DIR}/../..)
endif (DEFINED ENV{INSIGHTS_PATH})

execute_process(COMMAND "git tag --sort=-v:refname --list " v[0-9]*" | head -n 1 | cut -c 2-" APPVERSION file_contents)
set(LOGME_VERSION 0.2.1)
set(CONFIG_LOGME_VERSION 0.2.1)

# Add RainMaker components


set(EXTRA_COMPONENT_DIRS
        ${INSIGHTS_PATH}/components
        ${INSIGHTS_PATH}/examples/common
        $ENV{IDF_PATH}/components/esp_cron
        $ENV{IDF_PATH}/components/esp_libmbus
        )

