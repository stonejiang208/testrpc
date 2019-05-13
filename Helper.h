// -*- C++ -*-
//===========================================================================
/* Time-stamp: <2014-03-22 10:04:22 by jiangtao> */

/* @file Helper.h
 * $Id: Helper.h 2019-03-29 06:45:18 jiangtao 
 *
 * @author:jiangtao<jiangtao@Jiangtao>
 */
//===========================================================================

#ifndef HELPER_H
#define HELPER_H
#include <string>
#include "Utils.h"

class Helper
{
public:
  Helper(const std::string& url);

  int transfer (const std::string& from, const std::string& to,
                const std::string& quantity, const std::string& memo);

public:
  Json abi_json_to_bin(const Json& data);
  Json get_info();
  Json get_block(const Json& js_info);
  Json get_account(const std::string& account_name);
  Json get_currency_balance(const std::string& code,
                            const std::string& account,
                            const std::string& symbol);
  Json get_currency_stats(const std::string& code,
                          const std::string& symbol);
  int create_wallet();
  int  lock_all();
  int  unlock_wallet();
  int  open_wallet();
  int import();
  int list_keys();

  Json get_requied_keys(const std::string& from,
                        const std::string& name,
                        const std::string& data_arg,
                        const std::string& expiration,
                        long ref_block_num,
                        long ref_block_prefix
    );
  Json sign_transaction(const std::string& from,
                        const std::string& name,
                        const std::string& data_arg,
                        const std::string& expiration,
                        long ref_block_num,
                        long ref_block_prefix,
                        const Json& js_keys,
                        const std::string& chain_id);

  Json push_transaction(const std::string& from,
                        const std::string& name,
                        const std::string& data_arg,
                        const std::string& expiration,
                        long ref_block_num,
                        long ref_block_prefix,
                        const Json& signatures_js);
private: 
  std::string url_;
};

#endif /* HELPER_H */
