cmake_minimum_required(VERSION 3.19)
project(nanoservices2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

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
# THIRD-PARTY LIBRARIES
#
# Do not comment these lines.
############################
# Readable stack traces (used in NsException)
list(APPEND SRC thirdparty/backward/backward.cpp)

############################
# UTILITY DEFINITIONS
#
# Do not comment these lines.
############################
list(APPEND SRC util/stringutils/stringutils.cpp)

############################
# CONFIGURATION ENGINE
#
# Uncomment one of the following options to use one of the configuration engines. Exactly one option should be used for each service.
############################

# Configuration via command line arguments
list(APPEND SRC plugins/configuration/argc_argv/configuration_argc_argv.cpp)

# Configuration via environment variables
#list(APPEND SRC plugins/required/configuration/environment_variables/configuration_environment_variables.cpp)

# Configuration via JSON files. Two files, /etc/nanoservices2/globalconfig.json and /etc/nanoservices2/{serviceExecutableName}/serviceconfig.json, will be used as the configuration source. serviceconfig.json takes precedence. See the plugin documentation for more info
#list(APPEND SRC plugins/required/configuration/json_file/configuration_json_file.cpp)

# Configuration via Spring Cloud Config Server. To use this option, uncomment both of these lines. See the plugin documentation for more info
#ADD_DEFINITIONS(-DNANOSERVICES_CONF_PLUGIN_IMPLEMENTS_GETPROPERTY_STRING)
#list(APPEND SRC plugins/required/configuration/sccs/configuration_sccs.cpp)

# Configuration via ZooKeeper server. To use this option, uncomment both of these lines. See the plugin documentation for more info
#ADD_DEFINITIONS(-DNANOSERVICES_CONF_PLUGIN_IMPLEMENTS_GETPROPERTY_STRING)
#list(APPEND SRC plugins/required/configuration/zookeeper/configuration_zookeeper.cpp)

############################
# LOGGING ENGINE
#
# Uncomment one of the following options to use one of the logging engines. Exactly one option should be used for each service.
############################

# Log to STDOUT
list(APPEND SRC plugins/logging/stdout/logging_stdout.cpp)

# Log using the syslog.
#list(APPEND SRC plugins/logging/syslog/logging_syslog.cpp)

# Log using log4cxx. See the plugin documentation for more info
#list(APPEND SRC plugins/logging/log4cxx/logging_log4cxx.cpp)

# Log using the ELK (ElasticSearch, Logstash, Kibana) stack. See the plugin documentation for more info
#list(APPEND SRC plugins/logging/elk/logging_elk.cpp)

############################
# MONITORING ENGINE
#
# Uncomment one of the following options to use one of the monitoring engines. Exactly one option should be used for each service.
############################

# No monitoring
list(APPEND SRC plugins/monitoring/monitoring_none.cpp)

# Monitoring via Zipkin. See the plugin documentation for more info
#list(APPEND SRC plugins/optional/monitoring/zipkin/monitoring_zipkin.cpp)

add_executable(nanoservices2 ${SRC})