#ifndef SETTINGMENU_H
#define SETTINGMENU_H

static const QString FUNC_TYPE_CHANGE_PASSWORD = "Change Password";
static const QString FUNC_TYPE_CHANGE_UNLOCK_PASSWORD = "Change Unlock Password";

static const QString FUNC_TYPE_APPLICATION_LIST = "Application List";
static const QString FUNC_TYPE_APPLICATION_MANAGE = "Application Manage";

static const QString FUNC_TYPE_WIFI_SETTING = "WIFI Setting";
static const QString FUNC_TYPE_ETHERNET_SETTING = "Ethernet Setting";
static const QString FUNC_TYPE_BT_SETTING = "BT Setting";
static const QString FUNC_TYPE_3G_4G_SETTING = "3G/4G Setting";
static const QString FUNC_TYPE_DATE_TIME_SETTING = "Date/Time Setting";
static const QString FUNC_TYPE_LANGUAGE_SETTING = "Language Setting";
static const QString FUNC_TYPE_VPN_SETTING = "VPN Setting";
static const QString FUNC_TYPE_BRIGHT_SETTING = "Bright Setting";
static const QString FUNC_TYPE_VOICE_SETTING = "Voice Setting";
static const QString FUNC_TYPE_SCAN_MODE_SETTING = "Scanner Mode Setting";
static const QString FUNC_TYPE_BATTERY_SETTING = "Battery Setting";
static const QString FUNC_TYPE_RECOVERY_DEFAULT = "Recovery Default";

static const QString FUNC_TYPE_CA_DOWNLOAD = "CA Download";
static const QString FUNC_TYPE_SIGN_KEY_DOWNLOAD = "Firmware Sign Key Download";
static const QString FUNC_TYPE_APP_KEYS_DOWNLOAD = "Acquire CA, App Keys Download";
static const QString FUNC_TYPE_PAYMENT_KEY_DOWNLOAD = "Payment Key Download";
static const QString FUNC_TYPE_KEY_CHECKSUM= "Key Checksum";

static const QString FUNC_TYPE_SDCARD_SYSTEM_UPDATE = "SD Card System Update";
static const QString FUNC_TYPE_SDCARD_APP_UPDATE = "SD Card Application Update";

static const QString FUNC_TYPE_REMOTE_SYSTEM_UPDATE = "Remote System Update";
static const QString FUNC_TYPE_REMOTE_APP_UPDATE = "Remote Application Update";

static const QString FUNC_TYPE_TEST_TOOL = "Production Test Tool";

static const QString FUNC_TYPE_OP_TEST = "OP Test";

static const QString FUNC_TYPE_EMMC_USAGE = "EMMC Usage";
static const QString FUNC_TYPE_MEMORY_USAGE = "Memory Usage";
static const QString FUNC_TYPE_SYSTEM_INFO = "System Info";
static const QString FUNC_TYPE_VERSION_INFO = "Version Info";
static const QString FUNC_TYPE_HARDWARE_INFO= "Hardware Info";
static const QString FUNC_TYPE_ABOUT= "About";


static const QString FUNC_TYPE_UPDATE_BOOT = "Update Boot";
static const QString FUNC_TYPE_UPDATE_KERNEL = "Update Kernel";
static const QString FUNC_TYPE_UPDATE_32550_OS = "Update 32550 Os";
static const QString FUNC_TYPE_UPDATE_SYSTEM_LIB = "Update System Lib";
static const QString FUNC_TYPE_UPDATE_MASTER_APP = "Update Master App";

static const QString FUNC_TYPE_ADMIN_A_CHANGE_PWD = "Admin A Change Pwd";
static const QString FUNC_TYPE_ADMIN_B_CHANGE_PWD = "Admin B Change Pwd";

static const QString FUNC_TYPE_DISCHARGE_TEST="Discharge";
static const QString FUNC_TYPE_BACKLIGHT_TEST="Backlight Test";
static const QString FUNC_TYPE_KEYPAD_TEST="Switch / Key pad Test";
static const QString FUNC_TYPE_DDR_TEST="DDR Test";
static const QString FUNC_TYPE_SD_CARD_TEST="SD Card Test";
static const QString FUNC_TYPE_TOUCH_TEST="Touch Test";
static const QString FUNC_TYPE_LCD_TEST="LCD (RGB) Test";
static const QString FUNC_TYPE_W_SN_TEST="Write SN Test";
static const QString FUNC_TYPE_W_WIFI_MAC_TEST="Write WIFI MAC";
static const QString FUNC_TYPE_BATTERY_TEST="Battery Gauge(Battery Test)";
static const QString FUNC_TYPE_LED_TEST="Led Test";
static const QString FUNC_TYPE_BAR_CODE_TEST="Barcode (1D 2D)Test";
static const QString FUNC_TYPE_MSR_TEST="MSR Test ";
static const QString FUNC_TYPE_NFC_TEST="PICC Test";
static const QString FUNC_TYPE_FISCAL_TEST="Fiscal Test";
static const QString FUNC_TYPE_GPRS_TEST="2G/3G/4G Test ";
static const QString FUNC_TYPE_ETH_TEST="Ethernet Test";
static const QString FUNC_TYPE_WIFI_TEST="WIFI Test";
static const QString FUNC_TYPE_MODEM_TEST="Modem Test";
static const QString FUNC_TYPE_SAM_CARD_TEST="SAM Card Test";
static const QString FUNC_TYPE_USB_OTG_TEST="USB OTG Test";
static const QString FUNC_TYPE_USB_HOST_TEST="USB Host Test";
static const QString FUNC_TYPE_PRINTER_TEST="Printer Test";
static const QString FUNC_TYPE_IC_CARD_TEST="IC Card Test";
static const QString FUNC_TYPE_DATE_TEST="Date/Time Test";
static const QString FUNC_TYPE_UART_TEST="UART Test (UART1 ~ 3)";
static const QString FUNC_TYPE_FIRMWARE_VERSION="Firmware version";
static const QString FUNC_TYPE_BEEP_TEST="Beep Test";
static const QString FUNC_PIN_INPUT__TEST="Keypad Test (Pin Input Test)";
static const QString FUNC_TYPE_PCI_TEST="PCI Test";


static const QString  gb_Factory_UI[][18] = {
                                                        {"Setting", "11",
                                                         FUNC_TYPE_WIFI_SETTING,
                                                         FUNC_TYPE_BT_SETTING,
                                                         FUNC_TYPE_3G_4G_SETTING,
                                                         FUNC_TYPE_DATE_TIME_SETTING,
                                                         FUNC_TYPE_LANGUAGE_SETTING,
                                                         FUNC_TYPE_VPN_SETTING,
                                                         FUNC_TYPE_BRIGHT_SETTING,
                                                         FUNC_TYPE_VOICE_SETTING,
                                                         FUNC_TYPE_SCAN_MODE_SETTING,
                                                         FUNC_TYPE_BATTERY_SETTING,
                                                         FUNC_TYPE_RECOVERY_DEFAULT},

                                                        {"Key Management", "2",
                                                         FUNC_TYPE_CA_DOWNLOAD,
                                                         FUNC_TYPE_SIGN_KEY_DOWNLOAD},

                                                       {"SD Card Update", "1",
                                                        FUNC_TYPE_SDCARD_SYSTEM_UPDATE },

                                                      {"Production Test Tool", "1",
                                                       FUNC_TYPE_TEST_TOOL},

                                                        {"System Info", "6",
                                                         FUNC_TYPE_EMMC_USAGE,
                                                         FUNC_TYPE_MEMORY_USAGE,
                                                         FUNC_TYPE_SYSTEM_INFO,
                                                         FUNC_TYPE_VERSION_INFO,
                                                         FUNC_TYPE_HARDWARE_INFO,
                                                         FUNC_TYPE_ABOUT}
};

static const QString  gb_Release_UI[][18] = {
                                                        {"Administrator Management", "2",
                                                         FUNC_TYPE_CHANGE_PASSWORD,
                                                        FUNC_TYPE_CHANGE_UNLOCK_PASSWORD },

                                                           {"Application Management", "2",
                                                            FUNC_TYPE_APPLICATION_LIST,
                                                            FUNC_TYPE_APPLICATION_MANAGE },

                                                        {"Setting", "11",
                                                         FUNC_TYPE_WIFI_SETTING,
                                                         FUNC_TYPE_BT_SETTING,
                                                         FUNC_TYPE_3G_4G_SETTING,
                                                         FUNC_TYPE_DATE_TIME_SETTING,
                                                         FUNC_TYPE_LANGUAGE_SETTING,
                                                         FUNC_TYPE_VPN_SETTING,
                                                         FUNC_TYPE_BRIGHT_SETTING,
                                                         FUNC_TYPE_VOICE_SETTING,
                                                         FUNC_TYPE_SCAN_MODE_SETTING,
                                                         FUNC_TYPE_BATTERY_SETTING,
                                                         FUNC_TYPE_RECOVERY_DEFAULT},

                                                        {"Key Management", "3",
                                                         FUNC_TYPE_APP_KEYS_DOWNLOAD,
                                                         FUNC_TYPE_PAYMENT_KEY_DOWNLOAD,
                                                        FUNC_TYPE_KEY_CHECKSUM},

                                                       {"SD Card Update", "2",
                                                        FUNC_TYPE_SDCARD_SYSTEM_UPDATE,
                                                        FUNC_TYPE_SDCARD_APP_UPDATE},

                                                        {"Remote Update", "2",
                                                         FUNC_TYPE_REMOTE_SYSTEM_UPDATE,
                                                         FUNC_TYPE_REMOTE_APP_UPDATE},

                                                        {"System Info", "6",
                                                         FUNC_TYPE_EMMC_USAGE,
                                                         FUNC_TYPE_MEMORY_USAGE,
                                                         FUNC_TYPE_SYSTEM_INFO,
                                                         FUNC_TYPE_VERSION_INFO,
                                                         FUNC_TYPE_HARDWARE_INFO,
                                                         FUNC_TYPE_ABOUT}
};

static const QString  gb_Factory_Pci_UI[][18] = {
                                                        {"Setting", "4",
                                                         FUNC_TYPE_WIFI_SETTING,
//                                                         FUNC_TYPE_BT_SETTING,
                                                         FUNC_TYPE_ETHERNET_SETTING,
                                                         FUNC_TYPE_3G_4G_SETTING,
                                                         FUNC_TYPE_DATE_TIME_SETTING},

                                                        {"Key Management", "3",
                                                         FUNC_TYPE_CA_DOWNLOAD,
                                                         FUNC_TYPE_SIGN_KEY_DOWNLOAD,
                                                         " "},

                                                       {"SD Card Update", "1",
                                                        FUNC_TYPE_SDCARD_SYSTEM_UPDATE },

                                                        {"System Info", "1",
//                                                         FUNC_TYPE_SYSTEM_INFO,
                                                         FUNC_TYPE_VERSION_INFO
 //                                                        FUNC_TYPE_HARDWARE_INFO,
//                                                         FUNC_TYPE_ABOUT
                                                         }
};

static const QString  gb_Release_Pci_UI[][18] = {
                                                        {"Administrator Management", "1",
                                                         FUNC_TYPE_CHANGE_PASSWORD
 //                                                       FUNC_TYPE_CHANGE_UNLOCK_PASSWORD
    },

                                                           {"Application Management", "3",
                                                            FUNC_TYPE_APPLICATION_LIST,
                                                            FUNC_TYPE_APPLICATION_MANAGE,
                                                            " "},

                                                        {"Setting", "1",
//                                                         FUNC_TYPE_WIFI_SETTING,
//                                                         FUNC_TYPE_BT_SETTING,
//                                                         FUNC_TYPE_3G_4G_SETTING,
                                                         FUNC_TYPE_DATE_TIME_SETTING},

                                                        {"Key Management", "3",
                                                         FUNC_TYPE_APP_KEYS_DOWNLOAD,
                                                         FUNC_TYPE_PAYMENT_KEY_DOWNLOAD,
                                                         " "
//                                                        FUNC_TYPE_KEY_CHECKSUM
    }
                                                         ,

                                                       {"SD Card Update", "3",
                                                        FUNC_TYPE_SDCARD_SYSTEM_UPDATE,
                                                        FUNC_TYPE_SDCARD_APP_UPDATE,
                                                        " "},

                                                        {"OP Test", "1",
                                                         FUNC_TYPE_OP_TEST},

                                                        {"System Info", "1",
 //                                                        FUNC_TYPE_SYSTEM_INFO,
                                                         FUNC_TYPE_VERSION_INFO
//                                                         FUNC_TYPE_HARDWARE_INFO,
//                                                         FUNC_TYPE_ABOUT
                                                         }
};

static const QString  gb_Update_System_UI[]= {
                                                        "System Update",
                                                         FUNC_TYPE_UPDATE_BOOT,
                                                         FUNC_TYPE_UPDATE_KERNEL,
                                                         FUNC_TYPE_UPDATE_32550_OS,
 //                                                        FUNC_TYPE_UPDATE_SYSTEM_LIB,
                                                         FUNC_TYPE_UPDATE_MASTER_APP
};

static const QString  gb_Admins_Manage_UI[]= {
                                                        "Admins Manage",
                                                         FUNC_TYPE_ADMIN_A_CHANGE_PWD,
                                                         FUNC_TYPE_ADMIN_B_CHANGE_PWD
};

static const QString  gb_ProductionTest_UI[][35] = {
                                                        {"ProductionTest", "30",

                                                          FUNC_TYPE_BACKLIGHT_TEST,
                                                          FUNC_TYPE_KEYPAD_TEST,
                                                          FUNC_TYPE_DDR_TEST,
                                                         FUNC_TYPE_TOUCH_TEST,
                                                          FUNC_TYPE_SD_CARD_TEST,
                                                          FUNC_TYPE_LCD_TEST,
                                                          FUNC_TYPE_W_SN_TEST,
                                                          FUNC_TYPE_LED_TEST,
                                                          FUNC_TYPE_BAR_CODE_TEST,
                                                          FUNC_TYPE_MSR_TEST,
                                                          FUNC_TYPE_NFC_TEST,
                                                          FUNC_TYPE_GPRS_TEST,
                                                          FUNC_TYPE_ETH_TEST,
                                                          FUNC_TYPE_USB_OTG_TEST,
                                                          FUNC_TYPE_USB_HOST_TEST,
                                                          FUNC_TYPE_WIFI_TEST,
                                                          FUNC_TYPE_PRINTER_TEST,
                                                          FUNC_TYPE_IC_CARD_TEST,
                                                          FUNC_TYPE_DATE_TEST,
                                                          FUNC_TYPE_FIRMWARE_VERSION,
                                                          FUNC_TYPE_BEEP_TEST,
                                                          FUNC_TYPE_PCI_TEST,
                                                         FUNC_TYPE_UART_TEST,

                                                         //Not do
                                                         FUNC_PIN_INPUT__TEST,
                                                         FUNC_TYPE_DISCHARGE_TEST,
                                                         FUNC_TYPE_SAM_CARD_TEST,
                                                         FUNC_TYPE_W_WIFI_MAC_TEST,
                                                         FUNC_TYPE_BATTERY_TEST,
                                                         FUNC_TYPE_FISCAL_TEST,
                                                         FUNC_TYPE_MODEM_TEST,
                                                        "" ,


                                                        },

};


#endif // SETTINGMENU_H

