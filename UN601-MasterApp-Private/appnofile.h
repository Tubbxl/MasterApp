#ifndef APPNOFILE_H
#define APPNOFILE_H

#include <QString>

class AppNoFile
{
public:
    AppNoFile();
    ~AppNoFile();

      int writeCurrAppNo(unsigned char byAppNo);
      int readCurrAppNo(unsigned char *byAppNo);

private:
       char mCurrAppNoFilePath[256];

};

#endif // APPNOFILE_H
