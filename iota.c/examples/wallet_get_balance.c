// Copyright 2020 IOTA Stiftung
// SPDX-License-Identifier: Apache-2.0

/**
 * @brief A simple example for checking balances.
 *
 */

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core/utils/byte_buffer.h"
#include "wallet/wallet.h"

#define NODE_HOST "api.lb-0.testnet.chrysalis2.com"
#define NODE_HOST_PORT 443
#define NODE_USE_TLS true

char  my_seed[65] = {0};
char const *const account_path = "m/44'/4218'/0'/0'";

void show_address(iota_wallet_t *w, uint32_t index) {
  byte_t addr_wit_version[IOTA_ADDRESS_BYTES] = {};
  char tmp_bech32_addr[100] = {};

  addr_wit_version[0] = ADDRESS_VER_ED25519;
  wallet_address_by_index(w, index, addr_wit_version + 1);
  address_2_bech32(addr_wit_version, "atoi", tmp_bech32_addr);
  //printf("Addr[%" PRIu32 "]\n", index);
  // print ed25519 address without version filed.
  //printf("\t");
  //dump_hex_str(addr_wit_version + 1, ED25519_ADDRESS_BYTES);
  // print out
  //printf("\t%s\n", tmp_bech32_addr);
}

int main(int argc, char *argv[]) {

 if( strlen(argv[1])<=0 ){
	printf("invalid seed introduced\n");
	return -1;
}

 //printf("my_seed:%s\n", argv[1]);
 //printf("my_path:%s\n", argv[2]);
 //printf("account_path:%s\n", account_path);

 sprintf(my_seed,"%s",argv[1]);	 
 

 byte_t seed[IOTA_SEED_BYTES] = {};
  // address with a version byte
  byte_t recv[IOTA_ADDRESS_BYTES] = {};
  iota_wallet_t *wallet = NULL;

  if (strlen(my_seed) != 64) {
    printf("invalid seed string, it should be a 64-character-string..\n");
    return -1;
  }

  // convert seed from hex string to binary
  if (hex_2_bin(my_seed, strlen(my_seed), seed, sizeof(seed)) != 0) {
    printf("convert seed failed\n");
    goto failed_destroy;
  }

  if ((wallet = wallet_create(seed, account_path)) == NULL) {
    printf("create wallet failed\n");
	goto failed_nodestroy;
  }

  // set connected node
  wallet_set_endpoint(wallet, NODE_HOST, NODE_HOST_PORT, NODE_USE_TLS);
 
  uint64_t total_balance = 0;
  for (uint32_t idx = 0; idx < 5; idx++) {
    uint64_t value = 0;
    if (wallet_balance_by_index(wallet, idx, &value) != 0) {
      printf("wallet get balance [%" PRIu32 "]failed\n", idx);
      goto failed_destroy;
    }
    show_address(wallet, idx);
    total_balance += value;
//  printf("balance: %" PRIu64 "\n", value);
  }
   //printf("total balance: %" PRIu64 "\n", total_balance);  
	printf("%" PRIu64 , total_balance);
done:
  wallet_destroy(wallet);
  return 0;
failed_destroy:
  wallet_destroy(wallet);
failed_nodestroy:
  return 1;
}
