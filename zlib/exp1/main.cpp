#include <iostream>
#include <fstream>
#include <string>
#include <zlib.h>
#include <vector>

void createZipFile() {
    FILE* fp = fopen("numbers.txt", "w");
    for (int i = 0; i < 1024 * 1024; ++i) {
        fprintf(fp, "%d\n", i);
    }
    fclose(fp);

    system("zip -r compress.zip numbers.txt");
}

 std::string decompressString(const std::vector<unsigned char>& compressedData, uLongf originalSize) {
     std::vector<unsigned char> decompressedData(originalSize);

     int rlt = uncompress(decompressedData.data(), &originalSize, compressedData.data(), compressedData.size());
     if (rlt != Z_OK) {
         std::cout << rlt << std::endl;
         throw std::runtime_error("Failed to decompress data");
     }

     return std::string(decompressedData.begin(), decompressedData.end());
 }

int main() {
    createZipFile();

    std::ifstream istrm("compress.zip", std::ios::in);
    if (!istrm.is_open()) {
        return -1;
    }
    istrm.seekg(0, std::ios::end);
    std::streamsize size = istrm.tellg();
    istrm.seekg(0, std::ios::beg);
    char* buf = new char[size];
    istrm.read(buf, size);
    istrm.close();

    std::vector<unsigned char> v;
    v.assign(buf, buf+size);

    std::cout << decompressString(v, size) << std::endl;
}
