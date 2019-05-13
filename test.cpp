// -*- C++ -*-
//===========================================================================
/* Time-stamp: <2016-06-02 11:42:01 by jiangtao> */

/* @file test.cpp
 * $Id: test.cpp 2017-12-29 03:06:44 jiangtao Exp $
 *
 * @author:jiangtao<jiangtao@jiangtaodeMacBook-Pro.local>
 */
//===========================================================================


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include "ace/Log_Msg.h"
#include "ace/OS_main.h"
#include "ace/Arg_Shifter.h"
#include <glog/logging.h>
#include "restless.hpp"
#include "Utils.h"

#include "Helper.h"

using Http = asoni::Handle;

#define MY_LOG  LOG(INFO)

int ACE_TMAIN (int argc, ACE_TCHAR* argv[])
{
  ACE_UNUSED_ARG(argc);
  ACE_DEBUG ((LM_DEBUG,ACE_TEXT("(%P|%T) %s:%d start here \n"),__FILE__,__LINE__));
  google::InitGoogleLogging(argv[0]);
  google::SetLogDestination(google::INFO, ".log/INFO");

  Helper helper ("http://hk.happyfight.net:8888/v1/chain/");
  int result = -1;
  #if 1
  Json js = helper.get_account("jiangtao");
  MY_LOG << "get_account:" << js.dump(2);

  js = helper.get_currency_balance("eosio.token","jiangtao","EOS");
  MY_LOG << "get_currency_balance:" << js.dump(2);

  js = helper.get_currency_stats("eosio.token","EOS");
  MY_LOG << "get_currency_stats:" << js.dump(2);

  result = helper.transfer ("alice","bob","1.0000 EOS", "mygame");

  MY_LOG << "result =" << result ;
  #else
  // int result = helper.lock_all();
//  int result = helper.open_wallet();
//  MY_LOG << "result =" << result ;

  result = helper.import();

  MY_LOG << "result =" << result ;

  #endif



  #if 0
#if 1
  LOG(INFO) << "info test " << 13 << "abc";

  LOG(INFO) << "info test2";

  std::string url = "http://hk.happyfight.net:8888/v1/chain/";
  std::string data;
  {
    std::string m = "abi_json_to_bin";
    Json d;
    d["code"] = "eosio.token";
    d["action"] = "transfer";
    d["args"]["from"] = "alice";
    d["args"]["to"] = "bob";
    d["args"]["quantity"] = "1.0000 SYS";
    d["args"]["memo"] = "transfer by rpc";
    auto result = Http().post(url + m)
      .content("application/json",d.dump())
      .exec();
    std::cout << m << std::endl;

    auto response = Json::parse(result.body);
    std::cout << response.dump(2) << std::endl;
    data = Utils::get_string_value(response,"binargs");
    std::cout << "binargs: " << data << std::endl;
  }
  {
    std::string m = "get_info";
    auto result = Http().get(url + m)
      .exec();
    auto response = Json::parse(result.body);
    std::cout << response.dump(2) << std::endl;
    int head_block_num = response["head_block_num"];
    std::cout << "head_block_num = " << head_block_num << std::endl;

    m = "get_block";
    Json d;
    d["block_num_or_id"] = head_block_num;
    auto result2 = Http().post(url + m)
      .content("application/json",d.dump())
      .exec();
    auto response2 = Json::parse(result2.body);
    std::cout << "get block \n";
    std::cout << response2.dump(2) << std::endl;


  }
  #if 0
  {
    // 创建钱包
    std::string url2 ="http://hk.happyfight.net:9876/v1/wallet/create";
    std::string d = "\"dynamic\"";

      auto result = Http().post(url2,d)
      . content("plain/text",d)
      .exec();
    std::cout << "wallet open\n";
    std::cout << result.body << std::endl;

  }
 #endif
  {
    // 打开钱包
    std::string url2 ="http://hk.happyfight.net:9876/v1/wallet/open";
    std::string d = "\"dynamic\"";
    //PW5JduAm4YEmwZEEgoqdRa3JaW5dgkfUZHFKVbqBARKEXCQ78xn5Q
    auto result = Http().post(url2,d)
      . content("plain/text",d)
      .exec();
    std::cout << "wallet open\n";
    std::cout << result.body << std::endl;

  }
  {
    //解锁
    std::string url2 ="http://hk.happyfight.net:9876/v1/wallet/unlock";
    std::string name = "dynamic";
    std::string pwd = "PW5JduAm4YEmwZEEgoqdRa3JaW5dgkfUZHFKVbqBARKEXCQ78xn5Q";
    Json d;
    d.push_back(name);
    d.push_back(pwd);
    //PW5JduAm4YEmwZEEgoqdRa3JaW5dgkfUZHFKVbqBARKEXCQ78xn5Q
    auto result = Http().post(url2)
      . content("application/json",d.dump())
      .exec();
    std::cout << "unlock\n";
    std::cout << result.body << std::endl;

  }

  {
    //PW5JduAm4YEmwZEEgoqdRa3JaW5dgkfUZHFKVbqBARKEXCQ78xn5Q

    //private key: 5KQwrPbwdL6PhXujxW37FSSQZ1JiwsST4cqQzDeyXtP79zkvFD3

  }


  #if 0
  {
    std::string m = "get_required_keys";
    Json d;
    auto result = Http().post(url + m)
      .content("application/json",d.dump())
      .exec();
    auto response = Json::parse(result.body);
    std::cout <<  m << std::endl;
    std::cout << response.dump(2) << std::endl;

  }
  #endif

  {
    Json d;
    d["account_name"] = "alice";
    std::string m = "get_account";
    auto result = Http().post(url + m)
      .content("application/json",d.dump())
      .exec();
    auto response = Json::parse(result.body);
    MY_LOG << response.dump(2);

    Json d1;
    d1["code"] = "eosio.token";
    d1["account"] = "alice";
    d1["symbol"] = "SYS";

    m = "get_currency_balance";
    auto result1 =  Http().post(url + m)
      .content("application/json",d1.dump())
      .exec();
    auto response1 = Json::parse(result1.body);
    MY_LOG << response1.dump(2);
    std::cout << response1.dump(2) << std::endl;
  
  }
#endif
#if 0
  {
    std::string url = "https://open-api.eos.blockdog.com/v1/chain/";
    std::string m = "get_info";
    auto result = Http().get(url + m)
      .header({
          {"accept","*/*"},
          {"apikey","5b4added-e80c-41fb-b5a9-16269d2de79b"}
            })
      .exec();

    std::cout << result.body << std::endl;
    auto result2 = Http().post(url + m)
      .content("application/json",d.dump())
      .exec();
    auto response2 = Json::parse(result2.body);


  }
  #endif
 #endif

  google::ShutdownGoogleLogging();
  ACE_DEBUG ((LM_DEBUG,ACE_TEXT("(%P|%T) %s:%d end here \n"),__FILE__,__LINE__));
  return 0;
}
