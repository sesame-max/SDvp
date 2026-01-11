#include "sRingBuffer.h"
#include "string.h"

void sRing_init(sRingTypeDef *ringBuffer, uint8_t *buffer, uint32_t bufferSize)
{
    ringBuffer->buffer = buffer;
    ringBuffer->bufferSize = bufferSize;
    ringBuffer->head = 0;
    ringBuffer->tail = 0;
}

sRingStatus sRing_push(sRingTypeDef *ringBuffer, uint8_t *data, uint32_t dataLen)
{
    uint32_t i = 0;
    uint32_t len = dataLen;
    uint32_t emptySize = 0;
    uint32_t tailPos = ringBuffer->tail;
    uint32_t headPos = ringBuffer->head;
    if (headPos <= tailPos)
    {
        emptySize = headPos + ringBuffer->bufferSize - tailPos;
    }
    else
    {
        emptySize = headPos - tailPos;
    }

    if (emptySize <= dataLen + sizeof(dataLen))
    {
        return SRING_FULL;
    }

    for (i = 0; i < sizeof(dataLen); i++)
    {
        ringBuffer->buffer[(tailPos + i) % ringBuffer->bufferSize] = *((uint8_t *)&len + i);
    }

    for (; i < dataLen + sizeof(dataLen); i++)
    {
        ringBuffer->buffer[(tailPos + i) % ringBuffer->bufferSize] = data[i - sizeof(dataLen)];
    }

    ringBuffer->tail = (tailPos + len + sizeof(dataLen)) % ringBuffer->bufferSize;

    return SRING_SUCCESS;
}

uint32_t sRing_pop(sRingTypeDef *ringBuffer, uint8_t *data, uint32_t maxDataLen)
{
    uint32_t dataLen = 0;
    uint32_t i = 0;
    uint32_t nextHead = 0;
    uint32_t tailPos = ringBuffer->tail;
    uint32_t headPos = ringBuffer->head;

    if (headPos == tailPos)
    {
        return 0;
    }

    for (i = 0; i < sizeof(maxDataLen); i++)
    {
        *((uint8_t *)&dataLen + i) = ringBuffer->buffer[(headPos + i) % ringBuffer->bufferSize];
    }
    dataLen = dataLen > maxDataLen ? maxDataLen : dataLen;

    nextHead = (headPos + dataLen + sizeof(maxDataLen)) % ringBuffer->bufferSize;

    for (; i < dataLen + sizeof(maxDataLen); i++)
    {
        data[i - sizeof(maxDataLen)] = ringBuffer->buffer[(headPos + i) % ringBuffer->bufferSize];
    }

    ringBuffer->head = nextHead;

    return dataLen;
}

uint32_t sRing_get_used(sRingTypeDef *ringBuffer)
{
    uint32_t used = 0;
    uint32_t tailPos = ringBuffer->tail;
    uint32_t headPos = ringBuffer->head;
    if (headPos <= tailPos)
    {
        used = tailPos - headPos;
    }
    else
    {
        used = headPos + ringBuffer->bufferSize - tailPos;
    }
    return used;
}

uint32_t sRing_get_empty(sRingTypeDef *ringBuffer)
{
    uint32_t emptySize = 0;
    uint32_t tailPos = ringBuffer->tail;
    uint32_t headPos = ringBuffer->head;
    if (headPos <= tailPos)
    {
        emptySize = headPos + ringBuffer->bufferSize - tailPos;
    }
    else
    {
        emptySize = headPos - tailPos;
    }
    return emptySize;
}
