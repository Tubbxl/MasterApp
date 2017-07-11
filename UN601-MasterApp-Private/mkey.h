#ifndef MKEY_H
#define MKEY_H


class MKey
{
public:
    MKey();
    ~MKey();

    unsigned char  byAppNo;
    unsigned char  byKeyTpe;
    unsigned char  byKeyIndex;
    unsigned char  byKeyLen;
    unsigned char  byKeyData[24];

};

#endif // MKEY_H
