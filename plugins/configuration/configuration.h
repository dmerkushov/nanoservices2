//
// Created by dmerkushov on 3/13/21.
//

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <map>
#include <memory>
#include <string>

namespace nanoservices {

/**
 * @brief The nanoservice configuration class.
 */
class configuration {
private:
    /**
     * @brief The map containing the configuration _properties after being read by
     * configuration::initialize(int,char**).
     * @details By default, the configuration is being read at startup. But, if the configuration engine plugin
     * implements its own configuration::get_property(std::string&), it may override this default behaviour.
     */
    static std::map<std::string, std::shared_ptr<std::string>, std::less<>> _properties;

public:
    /**
     * @brief The maximum allowed length of a configuration property name when using const char pointers to access it:
     * 256 bytes, excluding the terminating '\0'
     * @details The limit is introduced to prevent breaking the char-pointer-processing functions by an erroneous
     * pointer.
     */
    static const size_t MAX_PROPERTYNAME_LEN_CONSTCHARPTR = 256;

    /**
     * @brief Initialize the configuration engine
     * @details This method must be implemented by the configuration engine plugin.
     * @param argc argc from main() (command line argument count, including the executable name)
     * @param argv argv from main() (command line arguments, including the executable name at argv[0])
     */
    static void initialize(int argc, char **argv) noexcept;

    /**
     * @brief Finalize the configuration engine
     * @details This method must be implemented by the configuration engine plugin.
     */
    static void finalize() noexcept;

    /**
     * @brief Get a value of a configuration property
     * @details The default implementation is provided in configuration.cpp.
     *
     * For a configuration engine plugin to implement it, NANOSERVICES_CONF_PLUGIN_IMPLEMENTS_GETPROPERTY_STRING must be
     * defined (to do it, add a corresponding line to ${NSPROJECT_ROOT}/CMakeLists.txt - see
     * https://cmake.org/cmake/help/v3.6/command/add_definitions.html ). Otherwise, a duplicate definition error will be
     * raised by the linker.
     * @param property_name name of the property
     * @return The value of the named property. If the property is absent, returns an empty shared pointer.
     */
    static std::shared_ptr<std::string> get_property(const std::string &property_name) noexcept;

    /**
     * @brief Get a value of a configuration property
     * @details The implementation is provided in configuration.cpp, that makes use of
     * nanoservices::configuration::get_property(const std::string&). configuration engine plugins are not intended to
     * implement this method.
     * @param property_name name of the property
     * @return The value of the named property. If the property is absent, or its name is too long (see
     * nanoservices::configuration::MAX_PROPERTYNAME_LEN_CONSTCHARPTR), returns an empty shared pointer.
     */
    static std::shared_ptr<std::string> get_property(const char *property_name) noexcept;
};
} // namespace nanoservices

#endif // CONFIGURATION_H
