/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
#include "mbed.h"
#include "BLEDevice.h"
#include "UARTService.h"
 

#define LOG(args...)        printf(args)

 
BLEDevice  ble;
DigitalOut led1(LED1);
Serial pc(USBTX, USBRX);
 
UARTService *uartServicePtr;
 
void disconnectionCallback(Gap::Handle_t handle, Gap::DisconnectionReason_t reason)
{
    LOG("Disconnected!\n");
    LOG("Restarting the advertising process\n");
    ble.startAdvertising();
}
 
void onDataWritten(const GattCharacteristicWriteCBParams *params)
{
    if ((uartServicePtr != NULL) && (params->charHandle == uartServicePtr->getTXCharacteristicHandle())) {
        uint16_t bytesRead = params->len;
        LOG("received %u bytes\n", bytesRead);
        ble.updateCharacteristicValue(uartServicePtr->getRXCharacteristicHandle(), params->data, bytesRead);
    }
}

void advertisementCallback(const Gap::address_t      peerAddr,
                           int8_t                    rssi,
                           bool                      isScanResponse,
                           Gap::AdvertisementType_t  type,
                           uint8_t                   advertisingDataLen,
                           const uint8_t            *advertisingData) {
    LOG("adv peerAddr[%02x %02x %02x %02x %02x %02x] rssi %d, isScanResponse %u, AdvertisementType %u\n",
           peerAddr[0], peerAddr[1], peerAddr[2], peerAddr[3], peerAddr[4], peerAddr[5], rssi, isScanResponse, type);
    LOG("len = %u\n", advertisingDataLen);
    unsigned index = 0;
    for (; index < advertisingDataLen; index++) {
        LOG("%02x ", advertisingData[index]);
    }
    LOG("\n");
}
 
void periodicCallback(void)
{
    led1 = !led1;
}
 
int main(void)
{
    led1 = 1;
    Ticker ticker;
    ticker.attach(periodicCallback, 1);
 
    LOG("Initialising the nRF51822\n");
    wait(1);
    ble.init();
    
    ble.onDisconnection(disconnectionCallback);
    ble.onDataWritten(onDataWritten);
 
    /* setup advertising */
    ble.accumulateAdvertisingPayload(GapAdvertisingData::BREDR_NOT_SUPPORTED);
    ble.setAdvertisingType(GapAdvertisingParams::ADV_CONNECTABLE_UNDIRECTED);
    ble.accumulateAdvertisingPayload(GapAdvertisingData::SHORTENED_LOCAL_NAME,
                                     (const uint8_t *)"BLE UART", sizeof("BLE UART") - 1);
    ble.accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LIST_128BIT_SERVICE_IDS,
                                     (const uint8_t *)UARTServiceUUID_reversed, sizeof(UARTServiceUUID_reversed));
 
    ble.setAdvertisingInterval(Gap::MSEC_TO_ADVERTISEMENT_DURATION_UNITS(1000));
    ble.startAdvertising();
 
    UARTService uartService(ble);
    uartServicePtr = &uartService;

    ble.startScan(advertisementCallback);

    while (true) {
        ble.waitForEvent();
    }
}