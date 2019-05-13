// -*- C++ -*-
//===========================================================================
/* Time-stamp: <2016-06-02 11:42:01 by jiangtao> */

/* @file Helper.cpp
 * $Id: Helper.cpp 2019-03-29 06:45:26 jiangtao Exp $
 *
 * @author:jiangtao<jiangtao@Jiangtao>
 */
//===========================================================================


#include "Helper.h"
#include <glog/logging.h>

#include "restless.hpp"
using Http = asoni::Handle;

#define MY_LOG  LOG(INFO)

Helper::Helper(const std::string& url)
  : url_ (url)
{
  MY_LOG << url_;
}

int Helper::transfer(const std::string& from, const std::string& to,
                     const std::string& quantity, const std::string& memo)
{
  try{
    Json data;
    data["code"] = "eosio.token";
    data["action"] = "transfer";
    Json args;
    args["from"] = from;
    args["to"] = to;
    args["quantity"] = quantity;
    args["memo"] = memo;
    data["args"] = args;
    int retval = -1;
    Json js_binargs = this->abi_json_to_bin (data);
    MY_LOG << "data:" << data.dump(2);
    MY_LOG << "binargs:" << js_binargs.dump(2);
    Json js_info = this->get_info();
    MY_LOG << "js_info:" << js_info.dump(2);
    std::string chain_id =  js_info["chain_id"];
    MY_LOG << "chain_id:" <<  chain_id;
    Json js_block = this->get_block(js_info);
    MY_LOG << "js_block:" << js_block.dump(2);

    retval = this->unlock_wallet();
    MY_LOG << "unlock_wallet: retval =" << retval << std::endl;
#if 0
    if (retval != 0)
    {
      retval = this->open_wallet();
    }

    MY_LOG << "open wallet: retval =" << retval << std::endl;
#endif
    std::string name =  data["action"];
    std::string data_arg = js_binargs["binargs"];
    std::string expiration = Utils::timestamp_offset(js_block["timestamp"],"1m");
    long ref_block_num = js_block["block_num"];
    long ref_block_prefix =  js_block["ref_block_prefix"];

    Json js_keys = this->get_requied_keys(from,
                                          name,
                                          data_arg,
                                          expiration,
                                          ref_block_num,
                                          ref_block_prefix);
    MY_LOG << "js_keys:" << js_keys.dump(2);

    //  chain_id = "038f4b0fc8ff18a4f0842a8f0564611f6e96e8535901dd45e43ac8691a1c4dca";
    Json js_sign_tx = this->sign_transaction(from,
                                             name,
                                             data_arg,
                                             expiration,
                                             ref_block_num,
                                             ref_block_prefix,
                                             js_keys,
                                             chain_id);

    MY_LOG << "js_sign_tx:" << js_sign_tx.dump(2);



    Json push_transaction_js = this->push_transaction(from,
                                                      name,
                                                      data_arg,
                                                      expiration,
                                                      ref_block_num,
                                                      ref_block_prefix,
                                                      js_sign_tx);


    MY_LOG << "push_transaction_js:" <<  push_transaction_js.dump(2);


  }
  catch (const std::exception& ex){
    MY_LOG << "exception:" <<  ex.what();
  }
  return 0;
}

Json Helper::abi_json_to_bin (const Json& data)
{

  std::string m = "abi_json_to_bin";
  auto result = Http().post(url_ + m)
    .content("application/json",data.dump())
    .exec();

  auto response = Json::parse(result.body);
  return response;
}

Json Helper::get_info()
{
  std::string m = "get_info";
  auto result = Http().get(url_ + m)
    .exec();
  auto response = Json::parse(result.body);
  return response;
}

Json Helper::get_block(const Json& js_info)
{
  Json data;
  data["block_num_or_id"] = js_info["head_block_num"];
  std::string m = "get_block";
  auto result = Http().post(url_ + m)
    .content("application/json",data.dump())
    .exec();
  auto response = Json::parse(result.body);
  return response;
}

int Helper::lock_all()
{
  std::string url2 ="http://hk.happyfight.net:9876/v1/wallet/lock_all";
  std::string d = "";

  auto result = Http().post(url2,d)
    . content("plain/text",d)
    .exec();
  std::cout << "loack_all" << std::endl;
  std::cout << result.body << std::endl;
  return 0;

  return 0;
}
int Helper::create_wallet()
{
  std::string url2 ="http://hk.happyfight.net:9876/v1/wallet/create";
  std::string d = "\"default\"";

  auto result = Http().post(url2,d)
    . content("plain/text",d)
    .exec();
  std::cout << "create_wallet" << std::endl;
  std::cout << result.body << std::endl;
  return 0;
}
int Helper::open_wallet()
{
  std::string url2 ="http://hk.happyfight.net:9876/v1/wallet/unlock";
  std::string name = "default";
  std::string pwd = "PW5JrqaN34jbrg3cWEz5rAqfgDQNy6YMV5HpwruGK6UGFRVGMKsqP";
  Json d;
  d.push_back(name);
  d.push_back(pwd);
  //PW5JduAm4YEmwZEEgoqdRa3JaW5dgkfUZHFKVbqBARKEXCQ78xn5Q
  auto result = Http().post(url2)
    . content("application/json",d.dump())
    .exec();
  MY_LOG<< "open wallet" <<  result.body << std::endl;
  if (result.body == "{}")
  {
    return 0;
  }
  return -1;
}

int Helper::import()
{
  std::string url2 ="http://hk.happyfight.net:9876/v1/wallet/import_key";
  std::string name = "default";
  std::string key = "5Hqn5TvE2GDhWvUnUFxA3f5aTrY18XTzNvNxA4gGbe2vmgP47HT";
  Json d;
  d.push_back(name);
  d.push_back(key);
  //PW5JduAm4YEmwZEEgoqdRa3JaW5dgkfUZHFKVbqBARKEXCQ78xn5Q
  auto result = Http().post(url2)
    . content("application/json",d.dump())
    .exec();
  MY_LOG<< "wallet import" <<  result.body << std::endl;

  return 0;
}

int Helper::list_keys()
{
  std::string url2 ="http://hk.happyfight.net:9876/v1/wallet/list_keys";
  std::string name = "default";
  Json d;
  d.push_back(name);
 
  //PW5JduAm4YEmwZEEgoqdRa3JaW5dgkfUZHFKVbqBARKEXCQ78xn5Q
  auto result = Http().post(url2)
    . content("application/json",d.dump())
    .exec();
  MY_LOG<< "wallet list keys" <<  result.body << std::endl;

  return 0;
}
int Helper::unlock_wallet()
{
  std::string url2 ="http://hk.happyfight.net:9876/v1/wallet/unlock";
  std::string name = "default";
  std::string pwd = "PW5JrqaN34jbrg3cWEz5rAqfgDQNy6YMV5HpwruGK6UGFRVGMKsqP";
  Json d;
  d.push_back(name);
  d.push_back(pwd);
  auto result = Http().post(url2)
    . content("application/json",d.dump())
    .exec();
  MY_LOG << "unlock wallet:" <<  result.body << std::endl;
  if (result.body == "{}")
  {
    return 0;
  }
  return -1;
}

Json Helper::get_requied_keys(const std::string& from,
                              const std::string& name,
                              const std::string& data_arg,
                              const std::string& expiration,
                              long ref_block_num,
                              long ref_block_prefix)
{
  std::string m = "get_required_keys";
  Json data;
  Json available_keys = Json::array();
  available_keys.push_back("EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV");
  available_keys.push_back("EOS6eGUZU3oHauPXPRkzjxTYcP8NDzpGzMvUhxHDy2Vk7ekJBooBU");
  available_keys.push_back("EOS8E9ZERpZeHN5VbmjFvHqS1vBenAiScRBCo2ehk1E6KbK9ZNfau");
  //EOS6nrDPaPEu3UaSiVMpuXZfCrn2K1QoUeXgdk5AenQDpWcybhR7w
  //EOS67idGgoYu3HTyyd6dQBabESCQUJubGcA8hVVs3ZZVdkBd6Xgdx

  data["available_keys"] = available_keys;

  Json actions;

  Json action_item;
  action_item["account"] = "eosio.token";
  Json authorization;
  Json a_item;
  a_item["actor"] = from;
  a_item["permission"] = "active";

  authorization.push_back(a_item);
  action_item["authorization"] = authorization;
  action_item["data"] = data_arg;
  action_item["name"] = name;
  
  actions.push_back (action_item);

  Json transaction;
  transaction["actions"] = actions;

  Json context_free_actions = Json::array();
  transaction["context_free_actions"] = context_free_actions;

  Json context_free_data = Json::array();
  transaction["context_free_data"] = context_free_data;
  transaction["delay_sec"] = 0;
  transaction["expiration"] = expiration;
  transaction["max_kcpu_usage"] = 0;
  transaction["max_net_usage_words"] = 0;
  transaction["ref_block_num"] = ref_block_num;
  transaction["ref_block_prefix"] = ref_block_prefix;
  Json signatures = Json::array();
  transaction["signatures"] = signatures;

  data["transaction"] = transaction;
  
  auto result = Http().post(url_ + m)
    .content("application/json",data.dump())
    .exec();
  auto response = Json::parse(result.body);
  return response;
 

}

Json Helper::sign_transaction(const std::string& from,
                              const std::string& name,
                              const std::string& data_arg,
                              const std::string& expiration,
                              long ref_block_num,
                              long ref_block_prefix,
                              const Json& js_keys,
                              const std::string& chain_id)
{
  Json data = Json::array();
  Json item;
  item["ref_block_number"] = ref_block_num;
  item["ref_block_prefix"] = ref_block_prefix;
  item["expiration"] = expiration;

  Json actions;

  Json action_item;
  action_item["account"] = "eosio.token";
  Json authorization;
  Json a_item;
  a_item["actor"] = from;
  a_item["permission"] = "active";

  authorization.push_back(a_item);
  action_item["authorization"] = authorization;
  action_item["data"] = data_arg;
  action_item["name"] = name;
  
  actions.push_back (action_item);
  item["actions"] = actions;
  item["signatures"] = Json::array();
  data.push_back(item);

  Json keys = Utils::get_value(js_keys,"required_keys");
  data.push_back(keys);
 
  data.push_back(chain_id);


  MY_LOG << "sign_transaction args:" << data.dump(2);
  std::string m = "sign_transaction";
  std::string url2 ="http://hk.happyfight.net:9876/v1/wallet/";
  auto result = Http().post(url2 + m)
    .content("application/json",data.dump())
    .exec();
  auto response = Json::parse(result.body);
  return response;

}

Json Helper::push_transaction(const std::string& from,
                              const std::string& name,
                              const std::string& data_arg,
                              const std::string& expiration,
                              long ref_block_num,
                              long ref_block_prefix,
                              const Json& signatures_js)
{
  Json data;
  data["compression"] = "none";
  data["signatures"] = signatures_js["signatures"];
  Json actions;
  Json action_item;
  action_item["account"] = "eosio.token";
  Json authorization;
  Json a_item;
  a_item["actor"] = from;
  a_item["permission"] = "active";

  authorization.push_back(a_item);
  action_item["authorization"] = authorization;
  action_item["data"] = data_arg;
  action_item["name"] = name;
  actions.push_back (action_item);

  Json transaction;
  transaction["actions"] = actions;
  transaction["context_free_actions"] = Json::array();
  transaction["context_free_data"] = Json::array();
  transaction["expiration"] = expiration;
  transaction["ref_block_num"] = ref_block_num;
  transaction["ref_block_prefix"] = ref_block_prefix;

  transaction["transaction_extensions"] = Json::array();
  data["transaction"] = transaction;
  MY_LOG << "push_transaction args:" << data.dump(2);

  std::string m = "push_transaction";
  auto result = Http().post(url_ + m)
    .content("application/json",data.dump())
    .exec();
  MY_LOG << m << "\t" << result.body;
  auto response = Json::parse(result.body);
  return response;
}

Json Helper::get_account(const std::string& account_name)
{
  Json data;
  data["account_name"] = account_name;

  std::string m = "get_account";
  auto result = Http().post(url_ + m)
    .content("application/json",data.dump())
    .header({{"accept","application/json"},
             {"content-type","application/json"}})
    .exec();
  auto response = Json::parse(result.body);
  return response;
}

Json Helper::get_currency_balance(const std::string& code,
                          const std::string& account,
                          const std::string& symbol)
{
  Json data;
  data["account"] = account;
  data["code"] = code;
  data["symbol"] = symbol;

  std::string m = "get_currency_balance";
  auto result = Http().post(url_ + m)
    .content("application/json",data.dump())
    .exec();
  auto response = Json::parse(result.body);
  return response;
}

Json Helper::get_currency_stats(const std::string& code,
                                const std::string& symbol)
{
  Json data;
  data["code"] = code;
  data["symbol"] = symbol;

  std::string m = "get_currency_stats";
  auto result = Http().post(url_ + m)
    .content("application/json",data.dump())
    .exec();
  auto response = Json::parse(result.body);
  return response;
}
