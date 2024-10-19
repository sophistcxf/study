#include <iostream>
#include <fstream>
#include <string>
 std::string decompressString(const std::vector<unsigned char>& compressedData, uLongf               originalSize) {
     std::vector<unsigned char> decompressedData(originalSize);

     int rlt = uncompress(decompressedData.data(), &originalSize, compressedData.data(),             compressedData.size());
     if (rlt != Z_OK) {
         std::cout << rlt << std::endl;
         throw std::runtime_error("Failed to decompress data");
     }

     return std::string(decompressedData.begin(), decompressedData.end());
 }

int main() {
    std::ifstream istrm("hello.zip", std::ios::in);
    if (!istrm.is_open()) {
        return;
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
