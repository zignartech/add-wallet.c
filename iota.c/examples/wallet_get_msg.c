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

#include "client/api/v1/find_message.h"
#include "core/utils/byte_buffer.h"
#include "wallet/wallet.h"

#define NODE_HOST "api.lb-0.testnet.chrysalis2.com"
#define NODE_HOST_PORT 443
#define NODE_USE_TLS true

char  my_seed[65] = {0};
char const *const account_path = "m/44'/4218'/0'/0'";

int wallet_get_msg_by_index(iota_wallet_t* w,  char* index){
  char hex_addr[IOTA_ADDRESS_HEX_BYTES + 1] = {};
  res_find_msg_t* msg_res = NULL;

  if ((msg_res = res_find_msg_new()) == NULL) {
    printf("[%s:%d] Err: OOM\n", __func__, __LINE__);
    return -1;
  }

  if (find_message_by_index(&w->endpoint, index, msg_res) != 0) {
    printf("[%s:%d] Err: get msg API failed\n", __func__, __LINE__);
    res_find_msg_free (msg_res);
    return -1;
  }

  if (msg_res->is_error) {
    printf("Err response: %s\n", msg_res->u.error->msg);
  } else {

size_t count = res_find_msg_get_id_len(msg_res);
for (size_t i = 0; i < count; i++) {
  printf("%s\n", res_find_msg_get_id(msg_res, i));
}
//printf("message ID count %zu\n", count);

  }

  res_find_msg_free(msg_res);
  return 0;
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
 

    if (wallet_get_msg_by_index(wallet, "Zignar Technologies-DOUT") != 0) {
      printf("wallet get txo by [%" PRIu32 "]failed\n", 2);
      goto failed_destroy;
    }

  done:
  wallet_destroy(wallet);
  return 0;
failed_destroy:
  wallet_destroy(wallet);
failed_nodestroy:
  return 1;
}
