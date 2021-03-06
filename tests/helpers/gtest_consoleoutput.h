/*
  Copyright (c) 2015, 2018, Oracle and/or its affiliates. All rights reserved.

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License, version 2.0,
  as published by the Free Software Foundation.

  This program is also distributed with certain software (including
  but not limited to OpenSSL) that is licensed under separate terms,
  as designated in a particular file or component or in included license
  documentation.  The authors of MySQL hereby grant you an additional
  permission to link the program and your derivative works with the
  separately licensed software that they have included with MySQL.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef MYSQLROUTER_GTEST_ROUTER_EXETEST_INCLUDED
#define MYSQLROUTER_GTEST_ROUTER_EXETEST_INCLUDED

#include "mysql/harness/filesystem.h"
#include "mysql/harness/logging/registry.h"

#include <memory>

#include "gmock/gmock.h"

using mysql_harness::Path;
using std::string;

class ConsoleOutputTest : public ::testing::Test {
protected:
  virtual void SetUp() {
    using mysql_harness::Path;

    char *stage_dir_c = std::getenv("STAGE_DIR");
    if (stage_dir_c == nullptr) {
      stage_dir.reset(new Path("./stage"));
    } else {
      stage_dir.reset(new Path(stage_dir_c));
    }
#ifdef CMAKE_INTDIR
    if (origin_dir) {
      *stage_dir = stage_dir->join(origin_dir->basename());
    } else {
      *stage_dir = stage_dir->join("RelWithDebInfo");
    }
#endif
    plugin_dir.reset(new Path(*stage_dir));
    plugin_dir->append("lib");
#ifndef _WIN32
    plugin_dir->append("mysqlrouter");
#endif
    app_mysqlrouter.reset(new Path(*stage_dir));
    app_mysqlrouter->append("bin");
#ifdef _WIN32
    app_mysqlrouter->append("mysqlrouter.exe");
#else
    app_mysqlrouter->append("mysqlrouter");
#endif
    mysql_server_mock.reset(new Path(*stage_dir));
    mysql_server_mock->append("bin");
#ifdef _WIN32
    mysql_server_mock->append("mysql_server_mock.exe");
#else
    mysql_server_mock->append("mysql_server_mock");
#endif
    orig_cerr_ = std::cerr.rdbuf();
    std::cerr.rdbuf(ssout.rdbuf());

    std::ostream *log_stream = mysql_harness::logging::get_default_logger_stream();
    if (log_stream != &std::cerr) {
      orig_log_ = log_stream->rdbuf();
      log_stream->rdbuf(ssout_log.rdbuf());
    }
  }

  virtual void TearDown() {
    if (orig_cerr_) {
      std::cerr.rdbuf(orig_cerr_);
    }

    if (orig_log_) {
      std::ostream *log_stream = mysql_harness::logging::get_default_logger_stream();
      log_stream->rdbuf(orig_log_);
    }
  }

  void reset_ssout() {
    ssout.str("");
    ssout.clear();
    ssout_log.str("");
    ssout_log.clear();
  }

  void set_origin(const Path &origin) {
    origin_dir.reset(new Path(origin));
  }

  std::stringstream& get_log_stream() {
    if (orig_log_) {
      // if logger stream differs from cerr
      return ssout_log;
    }

    return ssout;
  }

  std::unique_ptr<Path> stage_dir;
  std::unique_ptr<Path> plugin_dir;
  std::unique_ptr<Path> app_mysqlrouter;
  std::unique_ptr<Path> origin_dir;
  std::unique_ptr<Path> mysql_server_mock;

  std::stringstream ssout;
  std::streambuf *orig_cerr_{nullptr};
  std::streambuf *orig_log_{nullptr};

 private:
  std::stringstream ssout_log;
};

#endif // MYSQLROUTER_GTEST_ROUTER_EXETEST_INCLUDED
