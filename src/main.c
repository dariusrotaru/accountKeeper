#include <avr/io.h>
#include "timer.h"
#include "lcd.h"
#include "mfrc522.h"
#include "accounts.h"
#include "button.h"

static const uint8_t VALID_UID[4] ={0xE3, 0x0F, 0x68, 0x3B};


int main(void)
{

    uint8_t uid[10];
    uint8_t uidSize;
    uint8_t currentIndex = 0;
    const Account* acc = AccountsGetIndex(currentIndex);

    TimerInit();
    LCDInit();
    LCDClear();
    BtnInit();
    LCDSetCursor(0,0);
    MFRC522Init();


    while (1) {
        if(!BtnRGetState()){
            if(currentIndex == 0){
                currentIndex = AccountsCount() - 1;
            } else {
                currentIndex--;
            }
        }
        if(!BtnLGetState()){
            currentIndex++;
            if(currentIndex >= AccountsCount()) currentIndex = 0;
        }
        acc = AccountsGetIndex(currentIndex);
        uint8_t status = MFRC522GetUID(uid, &uidSize);

        if (status == MFRC522_STATUS_OK && uidSize == 4) {
            uint8_t match = 1;

            for(uint8_t i = 0; i < 4; i++){
                if(uid[i] != VALID_UID[i]){
                    match = 0;
                    break;
                }
            }
            LCDClear();
            LCDSetCursor(0, 0);

            if(match){
                LCDSetCursor(0, 0);
                LCDPrintString(acc->username);
                LCDSetCursor(0,1);
                LCDPrintString(acc->password);
                DelayMs(2000);
            } else {
                LCDPrintString("Unknown key.");
            }

            DelayMs(200);
        } else {
            LCDClear();
            LCDSetCursor(0,0);
            LCDPrintString(acc->username);
            LCDSetCursor(0,1);
            LCDPrintString("*********");
            DelayMs(200);
        }
    }

    return 0;
}
