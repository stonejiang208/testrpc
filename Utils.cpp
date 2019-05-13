// -*- C++ -*-
//===========================================================================
/* Time-stamp: <2018-06-11 15:35:33 by root> */

/* @file Utils.cpp
 * $Id: Utils.cpp 2017-12-29 14:34:53 jiangtao Exp $
 *
 * @author:jiangtao<jiangtao@Jiangtao>
 */
//===========================================================================


#include "Utils.h"
#include <sstream>
#include "ace/UUID.h"
#include "ace/OS_NS_time.h"
#include "ace/OS_NS_sys_time.h"
#include <iostream>
#include <cstdlib>
#include <regex>
#include <fstream>
#include <iostream>
#include <ctime>
#include <iomanip>
#include <ratio>
#include <ctype.h>
#include <algorithm>
#include <random>

int Utils::get_int_value( const Json &obj, const std::string &k, int d )
{
  int v = d;
  std::string sz;
  auto it = obj.find(k);
  if (it != obj.end())
  {
    if (it->is_string())
    {
      sz = it.value();
      v = std::atoi(sz.c_str());
    }
    else if (it->is_number())
    {
      v = it.value();
    }
  }
  else
  {
    std::cout << "no key " << k << "\n";
  }
  return v;
}

ACE_UINT32 Utils::get_uint32_value( const Json &obj,
                                    const std::string &k,
                                    ACE_UINT32 d )
{
  ACE_UINT32 v = d;
  std::string sz;
  auto it = obj.find(k);
  if (it != obj.end())
  {
    if (it->is_string())
    {
      sz = it.value();
      v = std::atoi(sz.c_str());
    }
    else if (it->is_number())
    {
      v = it.value();
    }
  }
  else
  {
    std::cout << "no key " << k << "\n";
  }
  return v;
}

ACE_INT32 Utils::get_int32_value( const Json &obj,
                                    const std::string &k,
                                    ACE_INT32 d )
{
  ACE_INT32 v = d;
  std::string sz;
  auto it = obj.find(k);
  if (it != obj.end())
  {

    if (it->is_string())
    {
      sz = it.value();
      v = std::atoi(sz.c_str());
    }
    else if (it->is_number())
    {
      v = it.value();
    }
  }
  else
  {
    std::cout << "no key " << k << "\n";
  }
  return v;
}

ACE_UINT64 Utils::get_uint64_value( const Json &obj,
                                    const std::string &k,
                                    ACE_UINT64 d )
{
  ACE_UINT64 v = d;
  std::string sz;
  auto it = obj.find(k);
  if (it != obj.end())
  {
    if (it->is_string())
    {
      sz = it.value();
      v = std::atol(sz.c_str());
    }
    else if (it->is_number())
    {
      v = it.value();
    }
  }
  else
  {
    std::cout << "no key " << k << "\n";
  }
  return v;
}

ACE_INT64 Utils::get_int64_value( const Json &obj,
                                  const std::string &k,
                                  ACE_INT64 d )
{
  ACE_INT64 v = d;
  std::string sz;
  auto it = obj.find(k);
  if (it != obj.end())
  {
    if (it->is_string())
    {
      sz = it.value();
      v = std::atol(sz.c_str());
    }
    else if (it->is_number())
    {
      v = it.value();
    }
  }
  else
  {
    std::cout << "no key " << k << "\n";
  }
  return v;
}

double Utils::get_double_value( const Json &obj,
                                const std::string &k,
                                double d )
{
  double v = d;
  std::string sz;
  auto it = obj.find(k);
  if (it != obj.end())
  {
    if (it->is_string())
    {
      sz = it.value();
      v  = ::strtod(sz.c_str(), 0);
    }
    else if (it->is_number())
    {
      v = it.value();
    }
  }
  else
  {
    std::cout << "no key " << k << "\n";
  }
  return v;
}

float Utils::get_float_value( const Json &obj,
                                const std::string &k,
                                float d )
{
  return (float) Utils::get_double_value (obj,k,d);
}

std::string Utils::get_string_value( const Json &obj,
                                     const std::string &k,
                                     const std::string& d )
{
  std::string v = d;
  std::string sz;
  auto it = obj.find(k);
  if (it != obj.end())
  {
    if (it->is_string())
    {
      sz = it.value();
      v = sz;
    }
    else if (it->is_number())
    {
      if (it->is_number_integer())
      {
        int x = it.value();
        v = std::to_string(x);
      }
      else if (it->is_number_float())
      {
        float x = it.value();
        v = std::to_string(x);
      }
    }

  }
  else
  {
    std::cout << "no key " << k << "\n";
  }
  return v;
}

Json Utils::get_value (const Json& obj,
                       const std::string& k)
{
  Json js;
  auto it = obj.find(k);
  if (it != obj.end())
  {
    js = it.value();
  }
  return js;
}


std::string Utils::time_to_iso8601(const std::time_t& t)
{

  char buf[64]= {0};
  strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%S.50", gmtime(&t));

 return buf;
}

std::time_t Utils::iso8601_to_time(const std::string& t)
{
  std::tm tm = {};
  std::istringstream ss (t);
  ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%SZ");
  std::time_t tt = std::mktime(&tm);
  return tt;
}

std::string Utils::timestamp_offset(const std::string& t_str,
                                    const std::string& offset)
{
  //2018-05-24T15:30:32.000
  std::cout << "input:" << t_str << std::endl;
  using std::chrono::system_clock;
 
  std::time_t tt = Utils::iso8601_to_time (t_str);

  typedef std::chrono::duration<int, std::ratio<60> > one_min;
  system_clock::time_point tp = system_clock::from_time_t (tt);
  one_min delta (1 + 60 * 8);
  system_clock::time_point tp2 = tp + delta;

  std::time_t t2 = system_clock::to_time_t ( tp2 );

  std::string str = Utils::time_to_iso8601(t2);

  std::cout << "output:" << str << std::endl;
  return str;
}

