cmake_minimum_required(VERSION 3.6)
project(nanoservices2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_VERBOSE_MAKEFILE ON)

############################
# CORE DEFINITIONS
#
# Do not comment these lines.
############################
list(APPEND SRC main.cpp)
#list(APPEND SRC core/exception/NsException.cpp)

############################
# BASIC PLUGIN DEFINITIONS
#
# Do not comment these lines.
############################
list(APPEND SRC plugins/configuration/configuration.cpp)
list(APPEND SRC plugins/logging/logging.cpp)

############################
# THIRD-PARTY LIBRARIES DEFINITIONS
#
# Do not comment these lines.
############################
list(APPEND SRC thirdparty/backward/backward.cpp)   # Readable stack traces

############################
# UTILITY DEFINITIONS
#
# Do not comment these lines.
############################
list(APPEND SRC util/stringutils/stringutils.cpp)

############################
# CONFIGURATION ENGINE
#
# Uncomment one of the following lines to use one of the configuration engines. Exactly one should be used.
#
# argc_argv - command line arguments will be used as the configuration source
# environment_variables - environment vars will be used as configuration settings
# json_file - two json files, /etc/nanoservices2/globalconfig.json and /etc/nanoservices2/{serviceExecutableName}/serviceconfig.json, will be used as the configuration source. serviceconfig.json takes precedence. See the CPP file for more info
# sccs - a Spring Cloud Config Server instance will be used as the configuration source. See the CPP file for more info
# zookeeper - a zookeeper server will be used as the configuration source. See the CPP file for more info
############################
list(APPEND SRC plugins/configuration/argc_argv/configuration_argc_argv.cpp)
#list(APPEND SRC plugins/required/configuration/environment_variables/configuration_environment_variables.cpp)
#list(APPEND SRC plugins/required/configuration/json_file/configuration_json_file.cpp)
#list(APPEND SRC plugins/required/configuration/sccs/configuration_sccs.cpp)
#list(APPEND SRC plugins/required/configuration/zookeeper/configuration_zookeeper.cpp)

############################
# LOGGING ENGINE
#
# Uncomment one of the following lines to use one of the logging engines. Exactly one should be used.
#
# stdout - log to STDOUT
# elk - log using the ELK (ElasticSearch, Logstash, Kibana) stack. See the CPP file for more info
# log4cxx - log using the log4cxx engine
# syslog - log to syslog
############################
list(APPEND SRC plugins/logging/stdout/logging_stdout.cpp)
#list(APPEND SRC plugins/logging/elk/logging_elk.cpp)
#list(APPEND SRC plugins/logging/log4cxx/logging_log4cxx.cpp)
#list(APPEND SRC plugins/logging/syslog/logging_syslog.cpp)

############################
# MONITORING ENGINE
#
# Uncomment one of the following lines to use one of the monitoring engines. Exactly one should be used.
############################
list(APPEND SRC plugins/monitoring/monitoring_none.cpp)
#list(APPEND SRC plugins/optional/monitoring/zipkin/monitoring_zipkin.cpp)

add_executable(nanoservices2 ${SRC})