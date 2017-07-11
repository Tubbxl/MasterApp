#ifndef DUKPT_H
#define DUKPT_H


class Dukpt
{
public:
    Dukpt();
    ~Dukpt();

    unsigned char byAppNo;
    unsigned char byKeyIndex;
    unsigned char byBDKLen;
    unsigned char byBDKData[24];
    unsigned char byKSNLen;
    unsigned char byKSNData[16];


};

#endif // DUKPT_H
