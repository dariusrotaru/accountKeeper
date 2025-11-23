#ifndef ACCOUNTS_H
#define ACCOUNTS_H

#include <stdint.h>

#define MAX_CREDENTIAL_LENGTH 16

typedef struct {
    uint8_t id;
    char username[MAX_CREDENTIAL_LENGTH];
    char password[MAX_CREDENTIAL_LENGTH];
} Account;

/**
 * @brief Returns the number of accounts.
 * 
 * @return uint8_t Number of accounts.
 */
uint8_t AccountsCount(void);

/**
 * @brief 
 * 
 * @param index 
 * @return const Account* 
 */
const Account* AccountsGetIndex(uint8_t index);

#endif