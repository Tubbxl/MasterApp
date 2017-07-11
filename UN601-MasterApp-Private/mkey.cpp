#include "mkey.h"
#include<string.h>

MKey::MKey()
{
    byAppNo = 0;
    byKeyTpe = 0;
    byKeyIndex = 0;
    byKeyLen = 0;
    memset(byKeyData, 0x00, sizeof(byKeyData));
}

MKey::~MKey()
{

}

