#include "accounts.h"
#include <stddef.h>

static const Account accounts[] = {
    {0, "username1", "pass1"},
    {1, "username2", "pass2"},
    {2, "username3", "pass3"},
    {3, "username4", "pass4"},
    {4, "username5", "pass5"},
    {5, "username6", "pass6"},
};

#define ACCOUNT_COUNT (sizeof(accounts) / sizeof(accounts[0]))

uint8_t AccountsCount(void){
    return ACCOUNT_COUNT;
}

/**
 * @brief Returns the pointer of the account at the requested index.
 * 
 * @param index The "position"/index at which to return the pointer to.
 * @return const Account* The pointer for the acount.
 */
const Account* AccountsGetIndex(uint8_t index){
    if (index < ACCOUNT_COUNT){
        return &accounts[index];
    }
    return NULL;
}