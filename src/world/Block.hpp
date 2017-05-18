#pragma once

struct Block {
    Block(unsigned short idAndState, unsigned char light);

    Block(unsigned short id, unsigned char state, unsigned char light);

    Block();

    ~Block();

    unsigned short id:13;
    unsigned char state:4;
    unsigned char light:4;
};