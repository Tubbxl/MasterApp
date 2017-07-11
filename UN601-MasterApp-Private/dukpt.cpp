#include "dukpt.h"
#include<string.h>

Dukpt::Dukpt()
{
    byAppNo = 0;
    byKeyIndex = 0;
    byBDKLen = 0;
    byKSNLen = 0;

    memset(byBDKData, 0x00, sizeof(byBDKData));
    memset(byKSNData, 0x00, sizeof(byKSNData));
}

Dukpt::~Dukpt()
{

}

