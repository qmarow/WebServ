#include <stdlib.h>
#include <string.h>

char  Base64Table[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

unsigned char*decode_b64(const unsigned char *buftodec, int bufsize,
                         unsigned char *decbuf)
{
    // выделяем память под временный буфер
    unsigned char *buftemp =(unsigned char *)malloc(sizeof(unsigned char) * bufsize);
    memset(buftemp, '\0', bufsize);
    memcpy(buftemp, buftodec, bufsize);

    int i = 0;
    int cpos[5];
    unsigned char binbyte[4];

    while (i < bufsize)
    {
        if(buftemp[i] == '=')
            cpos[0] = 0;
        else
            cpos[0] = strchr(Base64Table, buftemp[i]) - Base64Table;

        if(buftemp[i + 1] == '=')
            cpos[1] = 0;
        else
            cpos[1] = strchr(Base64Table, buftemp[i + 1]) - Base64Table;

        if(buftemp[i + 2] == '=')
            cpos[2] = 0;
        else
            cpos[2] = strchr(Base64Table, buftemp[i + 2]) - Base64Table;

        if(buftemp[i + 3] == '=')
            cpos[3] = 0;
        else
            cpos[3] = strchr(Base64Table,buftemp[i + 3]) - Base64Table;

        binbyte[0] = ((cpos[0] << 2) | (cpos[1] >> 4));
        binbyte[1] = ((cpos[1] << 4) | (cpos[2] >> 2));
        binbyte[2] = (((cpos[2] & 0x03 )<< 6) | (cpos[3] & 0x3f));
        decbuf[i - (i / 4)] = binbyte[0];
        decbuf[i - (i / 4) + 1] = binbyte[1];
        decbuf[i - (i / 4) + 2] = binbyte[2];
        i += 4;
    }
    free(buftemp);
    return decbuf;
}