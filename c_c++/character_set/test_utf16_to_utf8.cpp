#include <iostream>

int toUTF8(const unsigned short *utf16, unsigned char *utf8)
{
    unsigned short ucs4[2];
    unsigned int *u = (unsigned int *)ucs4;
    int w;

    if(utf16[0] >= 0xd800 && utf16[0] <= 0xdfff)
    {
        if(utf16[0] < 0xdc00)
        {
            if(utf16[1] >= 0xdc00 && utf16[1] <= 0xdfff)
            {
                ucs4[1] = (utf16[0] & 0x3ff);
                ucs4[0] = (utf16[1] & 0x3ff);
                ucs4[0] = ((ucs4[1] << 10) | ucs4[0]);
                ucs4[1] = ((ucs4[1] >> 6) | 1);
            }
            else
            {
                return -1;
            }
        }
        else
        {
            return -1;
        }
    }
    else
    {
        ucs4[0] = utf16[0];
        ucs4[1] = 0x00;
    }

    w = *u;

    if(w <= 0x0000007f)
    {
        /*U-00000000 - U-0000007F:  0xxxxxxx*/
        utf8[0] = (w & 0x7f);

        return 1;
    }
    else if(w >= 0x00000080 && w <= 0x000007ff)
    {
        /*U-00000080 - U-000007FF:  110xxxxx 10xxxxxx*/
        utf8[1] = (w & 0x3f) | 0x80;
        utf8[0] = ((w >> 6) & 0x1f) | 0xc0;

        return 2;
    }
    else if(w >= 0x00000800 && w <= 0x0000ffff)
    {
        /*U-00000800 - U-0000FFFF:  1110xxxx 10xxxxxx 10xxxxxx*/
        utf8[2] = (w & 0x3f) | 0x80;
        utf8[1] = ((w >> 6) & 0x3f) | 0x80;
        utf8[0] = ((w >> 12) & 0x0f) | 0xe0;

        return 3;
    }
    else if(w >= 0x00010000 && w <= 0x001fffff)
    {
        /*U-00010000 - U-001FFFFF:  11110xxx 10xxxxxx 10xxxxxx 10xxxxxx*/
        utf8[3] = (w & 0x3f) | 0x80;
        utf8[2] = ((w >> 6) & 0x3f) | 0x80;
        utf8[1] = ((w >> 12) & 0x3f) | 0x80;
        utf8[0] = ((w >> 18) & 0x07) | 0xf0;

        return 4;
    }
    else if(w >= 0x00200000 && w <= 0x03ffffff)
    {
        /*U-00200000 - U-03FFFFFF:  111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx*/
        utf8[4] = (w & 0x3f) | 0x80;
        utf8[3] = ((w >> 6) & 0x3f) | 0x80;
        utf8[2] = ((w >> 12) & 0x3f) | 0x80;
        utf8[1] = ((w >> 18) & 0x3f) | 0x80;
        utf8[0] = ((w >> 24) & 0x03) | 0xf8;

        return 5;
    }
    else if(w >= 0x04000000 && w <= 0x7fffffff)
    {
        /*U-04000000 - U-7FFFFFFF:  1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx*/
        utf8[5] = (w & 0x3f) | 0x80;
        utf8[4] = ((w >> 6) & 0x3f) | 0x80;
        utf8[3] = ((w >> 12) & 0x3f) | 0x80;
        utf8[2] = ((w >> 18) & 0x3f) | 0x80;
        utf8[1] = ((w >> 24) & 0x03) | 0xf8;
        utf8[0] = ((w >> 30) & 0x01) | 0xfc;

        return 6;
    }

    return 0;
}

void toUTF8(const unsigned short* s, size_t len, unsigned char* utf8) {
    int offset = 0;
    for (int i = 0; i < len; ++i) {
        int l = toUTF8(&s[i], utf8 + offset);
        offset += l;
    }
}


int main() {
    unsigned short* s = new unsigned short[2];
    s[0] = 0x4e2d;
    s[1] = 0x56fd;
    unsigned char buf[256] = {0};
    toUTF8(s, 2, buf);
    std::cout << buf;
    return 0;
}
