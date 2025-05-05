#include "videodata.hpp"

Cache::Cache(int c) : cap(c) {}

void Cache:: addBlock(const DataBlock& block) {
    if (blocks.size() >= cap) blocks.pop_front();
    blocks.push_back(block);
}

bool Cache:: hasConsecutives(int start, int count) {
    int fount = 0;
    for (const auto& blk : blocks) {
        if (blk.seqNum == start + found) ++found;
        if (found == count) return true;
    }
    return false;
}
