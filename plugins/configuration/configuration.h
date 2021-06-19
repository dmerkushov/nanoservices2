//
// Created by dmerkushov on 3/13/21.
//

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <memory>
#include <string>

namespace nanoservices {

/**
 * @brief Initialize the configuration engine
 * @param argc argc from main() (command line argument count, including the executable name)
 * @param argv argv from main() (command line arguments, including the executable name at argv[0])
 */
    void conf_initialize(int argc, char **argv) noexcept;

/**
 * @brief Finalize the configuration engine
 */
    void conf_finalize() noexcept;

/**
 * @brief Get a value of a configuration property. To be implemented by the plugin
 * @param propertyName name of the property
 * @return The value of the named property. If the property is absent, must return an empty shared pointer.
 */
    std::shared_ptr<std::string> conf_getProperty(const char *propertyName) noexcept;

/**
 * @brief Get the value of a configuration property. Implemented in the default <code>configuration.cpp</code>
 * @param propertyName name of the property
 * @return
 */
    std::shared_ptr<std::string> conf_getProperty(std::shared_ptr<std::string> propertyName) noexcept;
}

#endif //CONFIGURATION_H
