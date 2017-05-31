#include "World.hpp"

void World::ParseChunkData(Packet packet) {
    int chunkX = packet.GetField(0).GetInt();
    int chunkZ = packet.GetField(1).GetInt();
    bool isGroundContinuous = packet.GetField(2).GetBool();
    std::bitset<16> bitmask(packet.GetField(3).GetVarInt());
    int entities = packet.GetField(5).GetVarInt();

    size_t dataLen = packet.GetField(5).GetLength();
    byte *content = new byte[dataLen];
    byte *contentOrigPtr = content;
    packet.GetField(5).CopyToBuff(content);

    if (isGroundContinuous)
        dataLen -= 256;

    byte *biomes = content + packet.GetField(5).GetLength() - 256;
    for (int i = 0; i < 16; i++) {
        if (bitmask[i]) {
            size_t len = 0;
            Vector chunkPosition = Vector(chunkX, i, chunkZ);
            if (!m_sections.insert(std::make_pair(chunkPosition, ParseSection(content, len))).second)
                LOG(ERROR) << "Chunk not created: " << chunkPosition;
            auto sectionIter = m_sections.find(chunkPosition);
            if (sectionIter == m_sections.end())
                LOG(ERROR)<< "Created chunk not found: " << chunkPosition;
            else
                sectionIter->second.Parse();
            content += len;
        }
    }
    delete[] contentOrigPtr;
}

Section World::ParseSection(byte *data, size_t &dataLen) {
    dataLen = 0;

    Field fBitsPerBlock = FieldParser::Parse(UnsignedByte, data);
    byte bitsPerBlock = fBitsPerBlock.GetUByte();
    data += fBitsPerBlock.GetLength();
    dataLen += fBitsPerBlock.GetLength();

    Field fPaletteLength = FieldParser::Parse(VarIntType, data);
    int paletteLength = fPaletteLength.GetVarInt();
    data += fPaletteLength.GetLength();
    dataLen += fPaletteLength.GetLength();

    std::vector<unsigned short> palette;
    if (paletteLength > 0) {
        for (unsigned char i = 0; i < paletteLength; i++) {
            endswap(&i);
            Field f = FieldParser::Parse(VarIntType, data);
            data += f.GetLength();
            dataLen += f.GetLength();
            palette.push_back(f.GetVarInt());
            endswap(&i);
        }
    }

    Field fDataLength = FieldParser::Parse(VarIntType, data);
    data += fDataLength.GetLength();
    dataLen += fDataLength.GetLength();

    int dataLength = fDataLength.GetVarInt();
    size_t dataSize = dataLength * 8;
    dataLen += dataSize;
    byte *dataBlocks = data;

    data += 2048;
    dataLen += 2048;
    byte *dataLight = data;

    byte *dataSky = nullptr;
    if (m_dimension == 0) {
        data += 2048;
        dataLen += 2048;
        dataSky = data;
    }

    return Section(dataBlocks, dataSize, dataLight, dataSky, bitsPerBlock, palette);
}

World::~World() {
}

World::World() {

}

