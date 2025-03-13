#include <fstream>
#include <iostream>
#include "player_generated.h"

void encode() {
    flatbuffers::FlatBufferBuilder builder;
    auto item1 = CreateItem(builder, 1, builder.CreateString("Sword"));
    auto item2 = CreateItem(builder, 2, builder.CreateString("Shield"));
    auto inventory = builder.CreateVector({item1, item2});
    auto player = CreatePlayer(
        builder, 1001, builder.CreateString("Hero"), 150.0f, inventory
    );
    builder.Finish(player);
    
    uint8_t* buffer = builder.GetBufferPointer();
    auto size = builder.GetSize();

    std::ofstream out("player.bin", std::ios::binary);
    out.write(reinterpret_cast<char*>(buffer), size);
    out.close();
}

void decode() {
    std::ifstream in("player.bin", std::ios::binary);
    in.seekg(0, std::ios::end);
    auto size = in.tellg();
    in.seekg(0, std::ios::beg);
    uint8_t* out_buf = new uint8_t[size];
    in.read(reinterpret_cast<char*>(out_buf), size);
    auto player = GetPlayer(out_buf);
    std::cout << "HP: " << player->hp() << ","
              << "ID: " << player->id() << ","
              << " Name: " << player->name()->c_str() << ","
              << std::endl;
    for (auto i : *(player->inventory())) {
        std::cout << "Item: " << i->name()->c_str() << std::endl;
    }
}
int main() {
    decode();
    return 0;
}