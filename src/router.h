#pragma once

#include "rodos.h"
#include "message.h"
#include "allocableobjects.h"
#include "fifo.h"

using namespace RODOS;

namespace uartRouter {

    static const int NUM_OF_UARTS = 16;
    extern Fifo<Message *, 40> newMessages; // From all input channels to the router
    extern AllocableObjects<Message, 40> messages; // All pending messages to be forwarded

    extern TopicListReport subscribers[NUM_OF_UARTS];  // Which channel/UART is expecting which topics
}