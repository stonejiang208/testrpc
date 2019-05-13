// -*- C++ -*-
//===========================================================================
/* Time-stamp: <2014-03-22 10:04:22 by jiangtao> */

/* @file Utils.h
 * $Id: Utils.h 2017-12-29 14:34:33 jiangtao 
 *
 * @author:jiangtao<jiangtao@Jiangtao>
 */
//===========================================================================

#ifndef UTILS_H
#define UTILS_H

#include "ace/Basic_Types.h"
#include "ace/Time_Value.h"
#include <functional>
#include <set>
#include <map>
#include <sstream>
#include <string>
#include "json.hpp"


using Json = nlohmann::json;


class  Utils
{
 public:
  static int get_int_value (const Json& obj,const std::string& k, int d = 0);
  static ACE_UINT32 get_uint32_value (
    const Json& obj,const std::string& k, ACE_UINT32 d = 0);
  static ACE_INT32 get_int32_value (
    const Json& obj,const std::string& k, ACE_INT32 d = 0);

  static ACE_UINT64 get_uint64_value (
    const Json& obj,const std::string& k, ACE_UINT64 d = 0);
  static ACE_INT64 get_int64_value (
    const Json& obj,const std::string& k, ACE_INT64 d = 0);

  static double get_double_value (
    const Json& obj,const std::string& k, double d = 0.0);

  static float get_float_value (
    const Json& obj,const std::string& k, float d = 0.0f);

  static std::string get_string_value (
    const Json& obj,const std::string& k, const std::string& d = "");

  static Json get_value (const Json& obj, const std::string& k);

  static std::string time_to_iso8601(const std::time_t& t);
  static std::time_t iso8601_to_time(const std::string& t);
  static std::string timestamp_offset(const std::string& input,
                                      const std::string& offset); 
};


#endif /* UTILS_H */
