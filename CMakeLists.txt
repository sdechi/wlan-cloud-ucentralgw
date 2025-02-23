cmake_minimum_required(VERSION 3.13)
project(ucentralgw VERSION 0.4.0)

set(CMAKE_CXX_STANDARD 17)

if(UNIX AND APPLE)
    set(OPENSSL_ROOT_DIR /usr/local/opt/openssl)
    set(MYSQL_ROOT_DIR /usr/local/opt/mysql-client)
    list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
endif()

if(UNIX AND NOT APPLE)
    set(PostgreSQL_TYPE_INCLUDE_DIR /usr/include/postgresql)
    list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
endif()

if(SMALL_BUILD)
    add_definitions(-DSMALL_BUILD)
endif()

# Auto build increment. You must define BUILD_INCREMENT with cmake -DBUILD_INCREMENT=1
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/build)
    file(READ build BUILD_NUM)
    if(BUILD_INCREMENT)
        MATH(EXPR BUILD_NUM "${BUILD_NUM}+1")
        file(WRITE build ${BUILD_NUM})
    endif()
else()
    set(BUILD_NUM 1)
    file(WRITE build ${BUILD_NUM})
endif()
add_definitions(-DAPP_VERSION="${CMAKE_PROJECT_VERSION}" -DBUILD_NUMBER="${BUILD_NUM}")

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost REQUIRED system)
find_package(OpenSSL REQUIRED)
find_package(ZLIB REQUIRED)
find_package(Lua REQUIRED)

if(SMALL_BUILD)
    find_package(Poco REQUIRED COMPONENTS Crypto JWT Net Util NetSSL Data DataSQLite)
else()
    find_package(CppKafka REQUIRED)
    find_package(PostgreSQL REQUIRED)
    find_package(MySQL REQUIRED)
    find_package(ODBC REQUIRED)
    find_package(Poco REQUIRED COMPONENTS Crypto JWT Net Util NetSSL Data DataSQLite DataPostgreSQL DataMySQL DataODBC)
endif()

include_directories(/usr/local/include  /usr/local/opt/openssl/include src include/kafka)

add_executable( ucentralgw
                build
                src/uCentral.cpp src/uCentral.h src/RESTAPI_server.cpp src/RESTAPI_server.h
                src/uCentralWebSocketServer.cpp src/uCentralWebSocketServer.h src/uSubSystemServer.cpp src/uSubSystemServer.h
                src/uStorageService.cpp src/uStorageService.h src/uDeviceRegistry.cpp src/uDeviceRegistry.h
                src/RESTAPI_oauth2Handler.cpp src/RESTAPI_oauth2Handler.h src/RESTAPI_devices_handler.cpp src/RESTAPI_devices_handler.h
                src/RESTAPI_device_handler.cpp src/RESTAPI_device_handler.h src/RESTAPI_unknownRequestHandler.cpp src/RESTAPI_unknownRequestHandler.h
                src/RESTAPI_handler.cpp src/RESTAPI_handler.h src/RESTAPI_device_commandHandler.cpp src/RESTAPI_device_commandHandler.h
                src/RESTAPI_objects.h src/RESTAPI_objects.cpp src/uAuthService.cpp src/uAuthService.h src/uCentralConfig.cpp src/uCentralConfig.h
                src/RESTAPI_default_configuration.cpp
                src/RESTAPI_default_configuration.h src/RESTAPI_default_configurations.cpp src/RESTAPI_default_configurations.h src/RESTAPI_commands.cpp
                src/RESTAPI_commands.h src/uCommandManager.cpp src/uCommandManager.h src/RESTAPI_command.cpp src/RESTAPI_command.h
                src/uFileUploader.cpp src/uFileUploader.h src/RESTAPI_file.cpp src/RESTAPI_file.h src/uCommandChannel.cpp src/uCommandChannel.h
                src/RESTAPI_system_command.cpp src/RESTAPI_system_command.h src/RESTAPI_BlackList.cpp src/RESTAPI_BlackList.h src/uUtils.h src/uUtils.cpp src/storage_blacklist.cpp src/storage_identity.cpp
                src/storage_command.cpp src/storage_healthcheck.cpp src/storage_statistics.cpp src/storage_logs.cpp
                src/storage_device.cpp src/storage_capabilities.cpp src/storage_defconfig.cpp src/storage_sqlite.cpp
                src/storage_mysql.cpp src/storage_pgql.cpp src/storage_odbc.cpp src/storage_tables.cpp src/RESTAPI_callback.cpp
                src/RESTAPI_callback.h src/uCallbackManager.cpp src/uCallbackManager.h src/uFirmwareManager.cpp
                src/uFirmwareManager.h src/storage_firmware_updates.cpp src/uStateProcessor.cpp src/uStateProcessor.h
                src/storage_lifetime_stats.cpp src/uCentralProtocol.cpp src/uCentralProtocol.h src/RESTAPI_protocol.h
                src/ALBHealthCheckServer.h src/kafka_topics.h)

if(NOT SMALL_BUILD)
    target_sources(ucentralgw PUBLIC src/kafka_service.cpp src/kafka_service.h)
endif()

INSTALL(TARGETS ucentralgw
        RUNTIME DESTINATION /usr/bin
)

target_link_libraries(ucentralgw PUBLIC
                        ${Poco_LIBRARIES} ${Boost_LIBRARIES} ${ZLIB_LIBRARIES})
if(NOT SMALL_BUILD)
    target_link_libraries(ucentralgw PUBLIC
            ${MySQL_LIBRARIES} ${ODBC_LIBRARIES} ${ZLIB_LIBRARIES} ${LUA_LIBRARIES}
            CppKafka::cppkafka
             )
endif()