//
// Created by dmerkushov on 3/13/21.
//

#ifndef MONITORING_H
#define MONITORING_H

#include <map>
#include <memory>
#include <string>

namespace nanoservices {

/**
 * @brief Initialize the monitoring engine. Called after the configuration is loaded, so monitoring may be configured
 * via the configuration engine.
 */
void mon_initialize() noexcept;

/**
 * @brief Finalize the monitoring engine.
 */
void mon_finalize() noexcept;

/**
 * @brief Is the monitoring active? By default, no, so the user may skip building the monitoring data.
 * @return If monitoring is active
 */
bool mon_active() noexcept;

/**
 * @brief Send an event to the monitoring engine. By default, does nothing.
 * @param eventId An event id according
 * @param eventData
 */
void mon_event(uint32_t eventId, std::shared_ptr<std::map<std::string, std::string>> eventData = nullptr) noexcept;

} // namespace nanoservices

#endif // MONITORING_H
