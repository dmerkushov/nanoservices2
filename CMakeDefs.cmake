############################
# CONFIGURATION SOURCE
#
# One of the following service configuration options may be chosen:
# "argc_argv"               - command line arguments
# "environment_variables"   - environment variables
# "json_file"               - JSON files. Two files, /etc/nanoservices2/globalconfig.json and /etc/nanoservices2/{serviceExecutableName}/serviceconfig.json, will be used as the configuration source. serviceconfig.json takes precedence. See the plugin documentation for more info
# "sccs"                    - Spring Cloud Config Server. To use this option, uncomment both of these lines. See the plugin documentation for more info
# "zookeeper"               - ZooKeeper server. To use this option, uncomment both of these lines. See the plugin documentation for more info
############################
set(NANOSERVICES2_CONFIGURATION_SOURCE "argc_argv")


############################
# LOGGING DESTINATION
#
# One of the following service logging options may be chosen:
# "stdout"      - Log to STDOUT
# "syslog"      - Log using the syslog
# "elk"         - Log using the ELK (ElasticSearch, Logstash, Kibana) stack. See the plugin documentation for more info
# "fluentd"     - Log using Fluentd. See the plugin documentation for more info
############################
set(NANOSERVICES2_LOGGING_DESTINATION "stdout")


############################
# MONITORING DESTINATION
#
# One of the following service monitoring options may be chosen:
# "none"    - Monitoring is disabled
# "zipkin"  - Distributed system monitoring via Zipkin. See the plugin documentation for more info
############################
set(NANOSERVICES2_MONITORING_DESTINATION "none")