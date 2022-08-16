#pragma once

#include "rodos.h"

//__________________________________ Pointer to messages to be processed
struct Message {
    uint32_t len{}; // <= max network message len (ca 1300 Bytes)
    uint8_t data[MAX_NETWORK_MESSAGE_LENGTH] = {};
    uint32_t channelX{}; // Index of channel of origin. Used only to assign topics

};