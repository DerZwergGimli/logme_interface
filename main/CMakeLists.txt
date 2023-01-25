idf_component_register(SRCS "main.c"
        "wifi/wifi_manager.c"
        "wifi/dns_server.c"
        "wifi/json.c"
        "wifi/nvs_sync.c"
        "web/rest_server.c"
        "system/system_info.c"
        INCLUDE_DIRS ".")

if (CONFIG_EXAMPLE_WEB_DEPLOY_SF)
    set(WEB_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../web-ui")
    if (EXISTS ${WEB_SRC_DIR}/dist)
        spiffs_create_partition_image(www ${WEB_SRC_DIR}/dist FLASH_IN_PROJECT)
    else ()
        message(FATAL_ERROR "${WEB_SRC_DIR}/dist doesn't exit. Please run 'npm run build' in ${WEB_SRC_DIR}")
    endif ()
endif ()

if (DEFINED ENV{INSIGHTS_PATH})
    set(INSIGHTS_PATH $ENV{INSIGHTS_PATH})
else ()
    set(INSIGHTS_PATH ${CMAKE_CURRENT_LIST_DIR}/../..)
endif (DEFINED ENV{INSIGHTS_PATH})

# Add RainMaker components
set(EXTRA_COMPONENT_DIRS ${INSIGHTS_PATH}/components ${INSIGHTS_PATH}/examples/common)

