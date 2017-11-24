//
// Created by dima on 13/10/17.
//

#include "BufferPool.h"
BufferPool* BufferPool::create(uint32_t total, uint32_t bufferLen){
    return new BufferPool(total, bufferLen);
}
BufferPool* BufferPool::bufferPool = BufferPool::create(1024, 65535);