/**
 * 流式解压gzip
 */

#include <zlib.h>
#include <stdio.h>
#include <assert.h>

void createGzipFile() {
    FILE* fp = fopen("numbers.txt", "w");
    for (int i = 0; i < 1024 * 1024; ++i) {
        fprintf(fp, "%d\n", i);
    }
    fclose(fp);

    system("gzip -c numbers.txt > compress.gz");
}

int32_t decompress() {
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;

    // inflateInit不能用来解gzip压缩
    //int32_t ret = inflateInit(&strm);
    
    // gzip压缩，要用 inflateInit2
    // zlib和gzip的结构不同，他们的header不一样，zlib使用RFP1950，gzip使用RFC1952
    // 他们其他的部分是一样的，都使用 RFC 1951
    int32_t ret = inflateInit2(&strm, 16 + MAX_WBITS);
    if (ret != Z_OK) {
        return ret; 
    }

    const int32_t kChunk = 16384;
    unsigned char* in = new unsigned char[kChunk];
    unsigned char* out = new unsigned char[kChunk];

    FILE* source = fopen("compress.gz", "rb");
    FILE* dest = fopen("uncompress", "wb");

    if (ret != Z_OK)
        return ret;
    do {
        strm.avail_in = fread(in, 1, kChunk, source);
        if (ferror(source)) {
            (void)inflateEnd(&strm);
            return Z_ERRNO;
        }
        if (strm.avail_in == 0)
            break;
        strm.next_in = in;
        do {
            strm.avail_out = kChunk;
            strm.next_out = out;            
            ret = inflate(&strm, Z_NO_FLUSH);
            assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
            switch (ret) {
            case Z_NEED_DICT:
                ret = Z_DATA_ERROR;     /* and fall through */
            case Z_DATA_ERROR:
            case Z_MEM_ERROR:
                (void)inflateEnd(&strm);
                return ret;
            }
            int32_t have = kChunk - strm.avail_out;
            if (fwrite(out, 1, have, dest) != have || ferror(dest)) {
                (void)inflateEnd(&strm);
                return Z_ERRNO;
            }
        } while (strm.avail_out == 0);
    } while (ret != Z_STREAM_END);
        /* clean up and return */
    (void)inflateEnd(&strm);

    fclose(source);
    fclose(dest);

    return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
}

int main() {
    createGzipFile();
    decompress();
}
