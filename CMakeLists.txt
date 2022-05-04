cmake_minimum_required(VERSION 3.19)
project(nanoservices2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules)

include(CMakeDefs.cmake)

if (UNIX)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(CMAKE_CXX_COMPILER g++-11)
        set(CMAKE_CXX_COMPILER_VERSION 11.1.0)
    endif ()
endif ()

############################
# CORE DEFINITIONS
#
# Do not comment these lines.
############################
list(APPEND SRC src/core/exception/ns_exception.cpp)
list(APPEND SRC src/core/serialization/serialization.cpp)

# Testing of the core components
list(APPEND TEST_SRC src/core/exception/_test_exception.cpp)

include_directories(BEFORE SYSTEM src/thirdparty/gabime)

############################
# BASIC PLUGIN DEFINITIONS
#
# Do not comment these lines.
############################
list(APPEND SRC src/plugins/configuration/configuration.cpp)
list(APPEND SRC src/plugins/logging/logging.cpp)

############################
# THIRD-PARTY LIBRARIES
#
# Do not comment these lines.
############################

############################
# UTILITY DEFINITIONS
#
# Do not comment these lines.
############################
list(APPEND SRC src/util/stringutils/stringutils.cpp)
list(APPEND SRC src/util/stringutils/_test_stringutils.cpp)
list(APPEND SRC src/util/testing/testing.cpp)

############################
# CONFIGURATION SOURCE
#
# Choose one of the options below by changing the value of NANOSERVICES2_CONFIGURATION_SOURCE in CMakeDefs.cmake
############################
message("==== Nanoservices2 configuration source: ${NANOSERVICES2_CONFIGURATION_SOURCE}")

if (NANOSERVICES2_CONFIGURATION_SOURCE STREQUAL "argc_argv")
    # configuration via command line arguments
    list(APPEND SRC src/plugins/configuration/argc_argv/configuration_argc_argv.cpp)

elseif (NANOSERVICES2_CONFIGURATION_SOURCE STREQUAL "environment_variables")
    # configuration via environment variables
    list(APPEND SRC plugins/required/configuration/environment_variables/configuration_environment_variables.cpp)

elseif (NANOSERVICES2_CONFIGURATION_SOURCE STREQUAL "json_file")
    # configuration via JSON files. Two files, /etc/nanoservices2/globalconfig.json and /etc/nanoservices2/{serviceExecutableName}/serviceconfig.json, will be used as the configuration source. serviceconfig.json takes precedence. See the plugin documentation for more info
    list(APPEND SRC plugins/required/configuration/json_file/configuration_json_file.cpp)

elseif (NANOSERVICES2_CONFIGURATION_SOURCE STREQUAL "sccs")
    # configuration via Spring Cloud Config Server. To use this option, uncomment both of these lines. See the plugin documentation for more info
    ADD_DEFINITIONS(-DNANOSERVICES_CONF_PLUGIN_IMPLEMENTS_GETPROPERTY_STRING)
    list(APPEND SRC plugins/required/configuration/sccs/configuration_sccs.cpp)

elseif (NANOSERVICES2_CONFIGURATION_SOURCE STREQUAL "zookeeper")
    # configuration via ZooKeeper server. To use this option, uncomment both of these lines. See the plugin documentation for more info
    ADD_DEFINITIONS(-DNANOSERVICES_CONF_PLUGIN_IMPLEMENTS_GETPROPERTY_STRING)
    list(APPEND SRC plugins/required/configuration/zookeeper/configuration_zookeeper.cpp)

endif ()

############################
# LOGGING DESTINATION
#
# Choose one of the options below by changing the value of NANOSERVICES2_CONFIGURATION_SOURCE in CMakeDefs.cmake
############################
message("==== Nanoservices2 logging destination: ${NANOSERVICES2_LOGGING_DESTINATION}")

if (NANOSERVICES2_LOGGING_DESTINATION STREQUAL "stdout")
    # Log to STDOUT
    list(APPEND SRC src/plugins/logging/stdout/logging_stdout.cpp)

elseif (NANOSERVICES2_LOGGING_DESTINATION STREQUAL "syslog")
    # Log using the syslog.
    list(APPEND SRC plugins/logging/syslog/logging_syslog.cpp)

elseif (NANOSERVICES2_LOGGING_DESTINATION STREQUAL "elk")
    # Log using the ELK (ElasticSearch, Logstash, Kibana) stack. See the plugin documentation for more info
    list(APPEND SRC plugins/logging/elk/logging_elk.cpp)

elseif (NANOSERVICES2_LOGGING_DESTINATION STREQUAL "fluentd")
    # Log using Fluentd. See the plugin documentation for more info
    list(APPEND STD plugins/logging/fluentd/logging_fluentd.cpp)

endif ()

############################
# MONITORING DESTINATION
#
# Uncomment one of the following options to use one of the monitoring engines. Exactly one option should be used for each service.
############################
message("==== Nanoservices2 monitoring destination: ${NANOSERVICES2_MONITORING_DESTINATION}")

if (NANOSERVICES2_MONITORING_DESTINATION STREQUAL "none")
    # No monitoring
    list(APPEND SRC src/plugins/monitoring/monitoring_none.cpp)

elseif (NANOSERVICES2_MONITORING_DESTINATION STREQUAL "zipkin")
    # Distributed system monitoring via Zipkin. See the plugin documentation for more info
    list(APPEND SRC plugins/optional/monitoring/zipkin/monitoring_zipkin.cpp)

endif ()

############################
# main.cpp
#
# Append all regular sources to the test sources, then the regular main.cpp to the regular sources, and the testing main.cpp to the test sources
# Do not comment these lines.
############################
list(APPEND TEST_SRC ${SRC})
list(APPEND SRC src/main.cpp)
list(APPEND TEST_SRC src/_test_main.cpp)

############################
# MAIN EXECUTABLE
#
# Do not comment these lines.
############################
add_executable(nanoservices2 ${SRC})

############################
# TESTING/CODE COVERAGE EXECUTABLE
#
# You may comment these lines if you don't need testing or code coverage.
############################
add_executable(nanoservices2_testing ${TEST_SRC})
target_compile_options(nanoservices2_testing PRIVATE -O0 -fprofile-arcs -ftest-coverage)
target_compile_features(nanoservices2_testing PRIVATE cxx_std_20)
target_link_options(nanoservices2_testing PUBLIC -fprofile-arcs -lgcov)

# Machine-readable coverage reports
add_custom_target(gcov
        DEPENDS nanoservices2_testing
        COMMAND bash ${PROJECT_SOURCE_DIR}/cmake/scripts/run_gcov.sh "${PROJECT_SOURCE_DIR}" "${CMAKE_BINARY_DIR}"
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/
        )

# Human-readable (HTML) coverage reports
add_custom_target(lcov
        DEPENDS nanoservices2_testing
        COMMAND bash ${PROJECT_SOURCE_DIR}/cmake/scripts/run_lcov.sh "${PROJECT_SOURCE_DIR}" "${CMAKE_BINARY_DIR}"
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/
        )