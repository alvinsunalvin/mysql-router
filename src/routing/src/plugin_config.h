/*
  Copyright (c) 2015, Oracle and/or its affiliates. All rights reserved.

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; version 2 of the License.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef PLUGIN_CONFIG_ROUTING_INCLUDED
#define PLUGIN_CONFIG_ROUTING_INCLUDED

#include "utils.h"
#include "mysqlrouter/datatypes.h"
#include "mysqlrouter/utils.h"
#include <mysqlrouter/routing.h>

#include <map>
#include <string>

#include "mysqlrouter/plugin_config.h"
#include "plugin.h"

using std::map;
using std::string;
using mysqlrouter::to_string;
using mysqlrouter::TCPAddress;

/** @brief Timeout for idling clients (in seconds)
 *
 * Constant defining how long (in seconds) a client can keep the connection idling. This is similar to the
 * wait_timeout variable in the MySQL Server.
 */
const int kDefaultWaitTimeout = 300;

/** @brief Max number of active routes for this routing instance */
const uint16_t kDefaultMaxConnections = 512;

/** @brief Timeout connecting to destination (in seconds)
 *
 * Constant defining how long we wait to establish connection with the server before we give up.
 */
const int kDefaultDestinationConnectionTimeout = 1;


class RoutingPluginConfig final : public mysqlrouter::BasePluginConfig {
public:
  /** @brief Constructor
   *
   * @param section from configuration file provided as ConfigSection
   */
  RoutingPluginConfig(const ConfigSection *section)
      : BasePluginConfig(section),
        destination(get_option_string(section, "destination")),
        bind_address(get_option_tcp_address(section, "bind_address", true)),
        connect_timeout(get_uint_option<uint16_t>(section, "connect_timeout", 1)),
        wait_timeout(get_uint_option<uint16_t>(section, "wait_timeout", 1)),
        mode(get_option_mode(section, "mode")),
        max_connections(get_uint_option<uint16_t>(section, "max_connections", 1)) { }

  string get_default(const string &option);
  bool is_required(const string &option);

  /** @brief `destination` option read from configuration section */
  const string destination;
  /** @brief `bind_address` option read from configuration section */
  const TCPAddress bind_address;
  /** @brief `connect_timeout` option read from configuration section */
  const int connect_timeout;
  /** @brief `wait_timeout` option read from configuration section */
  const int wait_timeout;
  /** @brief `mode` option read from configuration section */
  const routing::AccessMode mode;
  /** @brief `max_connections` option read from configuration section */
  const int max_connections;

protected:

private:
  routing::AccessMode get_option_mode(const ConfigSection *section, const string &option);

};

#endif // PLUGIN_CONFIG_ROUTING_INCLUDED
