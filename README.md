mbed BLE
========

To test lastest [BLE_API](https://github.com/mbedmicro/BLE_API) and [nRF51822](https://github.com/mbedmicro/nRF51822) with [mbed SDK](https://github.com/mbedmicro/mbed), change the path of these libraries in CMakeLists.txt.

```
# define some more paths to projects we depend on
set (MBED_SRC_PATH     ${BLUE_SOURCE_DIR}/../mbed/libraries/mbed)
set (BLE_API_SRC_PATH  ${BLUE_SOURCE_DIR}/../BLE_API)
set (NRF51822_SRC_PATH ${BLUE_SOURCE_DIR}/../nRF51822)
```

