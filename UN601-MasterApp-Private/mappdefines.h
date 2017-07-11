#ifndef MAPPDEFINES
#define MAPPDEFINES

#include <QString>

static const unsigned char PWD_TYPE_ADMIN_A = 0x00;
static const unsigned char PWD_TYPE_ADMIN_B = 0x01;
static const unsigned char FUNC_TYPE_VERIFY_PWD = 0x00;
static const unsigned char FUNC_TYPE_CHANGE_PWD = 0x01;
static const unsigned char ADMINS_VERIFY_PWD = 0x00;
static const unsigned char ADMINS_CHANGE_PWD = 0x01;



static const unsigned char SCHIP_HAVE_KEY = 0x01;
static const unsigned char  SCHIP_HAVE_APP = (0x01 << 1);
static const unsigned char  SCHIP_CHANGED_ADMINA_PWD = (0x01 << 2);
static const unsigned char  SCHIP_CHANGED_ADMINB_PWD = (0x01 << 3);
static const unsigned char  SCHIP_UNLOCK_PWD = (0x01 << 4);

static const int FACTORY_MODE = 0x01;
static const int RELEASE_MODE = 0x02;

static const QString SD_CARD_FILE_PATH  = "/media/media/mmcblk0p1/";
//static const QString SD_CARD_FILE_PATH  = "/media/media/sda4/";

static const QString FILE_TYPE_32550_OS_SUFFIX  = "bin";
static const QString FILE_TYPE_FIRMWARE_SUFFIX  = "imx";
static const QString FILE_TYPE_SYSTEM_LIB_SUFFIX = "so";
static const QString FILE_TYPE_KERNEL_PREFIX  = "zImage";
static const QString FILE_TYPE_BOOT_PREFIX  = "u-boot";
static const QString FILE_TYPE_MASTER_APP_PREFIX  = "MasterApp";
static const QString FILE_TYPE_API_SERVER_PREFIX  = "ApiServer";

static const int FILE_TYPE_BOOT = 1;
static const int FILE_TYPE_KERNEL = 2;
static const int FILE_TYPE_32550_OS = 3;
static const int FILE_TYPE_SYSTEM_LIB = 4;
static const int FILE_TYPE_MASTER_APP = 5;
static const int FILE_TYPE_SUB_APP = 6;
static const int FILE_TYPE_API_SERVER = 7;

static const int SUB_APP_MAX_NUMS = 24;
static const int SUB_APP_NAME_LEN = 32;

 static const int TERM_TYPE_MF2352 = 0x00;
 static const int TERM_TYPE_TRSM = 0x01;

 static const int CA_TYPE_VENDOR = 0x00;
 static const int CA_TYPE_ACQUIRE = 0x01;

  static const int FUNC_TYPE_SIGN_FIRMWARE = 0x00;
  static const int FUNC_TYPE_SIGN_APP = 0x01;
  static const int FUNC_TYPE_TEST_OP = 0x02;
  static const int FUNC_TYPE_DOWNLOAD_MK = 0x03;
  static const int FUNC_TYPE_DOWNLOAD_ACQUIRECA = 0x04;

   static const int DOWNLOAD_KEYS_TIME_OUT = 2*60*1000;
   static const int UPDATE_APP_FM_TIME_OUT = 3*60*1000;
    static const int UI_TIME_OUT = 60*1000;
    static const int WAIT_TIME_OUT = 100;

   static const int APP_PERM_FILE_LEN = 1024*6;
  static const int APP_PERM_ITEM_LEN = 37;
   static const int APP_PERM_APP_NAME_LEN = 32;
   static const int APP_PERM_ITEM_FLAG_POS = 32;
   static const unsigned char  APP_PERM_ITEM_DELETE_FLAG = 0x00;
  static const unsigned char  APP_PERM_ITEM_NORMAL_FLAG = 0x01;

    static const int APP_PERM_BYTE_LEN = 4;
     static const int VER_DATA_BYTE_LEN = 3;
    static const int SIGN_DATA_BYTE_LEN = 288;
     static const int APP_SIGN_DATA_LEN = 292;

     static const unsigned char MODULE_TYPE_ICC = 0x00;
     static const unsigned char MODULE_TYPE_MCR = 0x01;
     static const unsigned char MODULE_TYPE_PICC = 0x02;
     static const unsigned char MODULE_TYPE_PRINTER = 0x03;
     static const unsigned char MODULE_TYPE_SCAN = 0x04;
     static const unsigned char MODULE_TYPE_LCM = 0x05;
     static const unsigned char MODULE_TYPE_FINGERPRINT = 0x06;
     static const unsigned char MODULE_TYPE_PCI = 0x07;
     static const unsigned char MODULE_TYPE_SYSTEM = 0x08;
     static const unsigned char MODULE_TYPE_EXTERNALSERIAL = 0x09;


     static const QString UBOOT_VER  = "1.0.0";
     static const QString MASTERAPP_VER  = "1.0.1";

     extern int gb_RunMode;
     extern  bool gb_RefreshMainUI ;

#endif // MAPPDEFINES

