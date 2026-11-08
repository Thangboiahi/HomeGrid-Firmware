/*
 * HomeGrid board profile metadata for FW-01C preparation.
 *
 * Reference-only: this header is not included by app_main.c and defines no
 * GPIO assignments. Keep application logic independent of board-specific
 * pin mapping until a later reviewed hardware-integration task.
 */
#ifndef HOMEGRID_BOARD_CONFIG_H
#define HOMEGRID_BOARD_CONFIG_H

#define HOMEGRID_BOARD_PROFILE_NAME "Generic ESP32 development board"
#define HOMEGRID_BOARD_SOC_FAMILY "Original ESP32"
#define HOMEGRID_BOARD_IDF_TARGET "esp32" /* Department approved; FW-01D build passed. */
#define HOMEGRID_BOARD_MODULE_FAMILY "ESP-WROOM-32"
#define HOMEGRID_BOARD_USB_UART_FAMILY "CH9102"

#define HOMEGRID_BOARD_WIFI_24GHZ_CAPABLE 1
#define HOMEGRID_BOARD_BLE_CAPABLE 1

/* Unknown capacities use zero only as an explicit unknown sentinel. */
#define HOMEGRID_BOARD_FLASH_CAPACITY_BYTES 0U
#define HOMEGRID_BOARD_PSRAM_CAPACITY_BYTES 0U
#define HOMEGRID_BOARD_FLASH_CAPACITY_VERIFIED 0
#define HOMEGRID_BOARD_PSRAM_CAPACITY_VERIFIED 0

/* No application GPIO or relay pin has been assigned. */
#define HOMEGRID_BOARD_GPIO_MAPPING_CONFIRMED 0

#endif /* HOMEGRID_BOARD_CONFIG_H */
