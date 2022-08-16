#include "rodos.h"
#include <support-libs.h>
#include "connection.h"

/**
 * Connection represents a connection to another uart device. It has a HAL_UART connection and logic to receive
 * and send messages over said connection
 * @param uartIdx of HAL_UART
 * @param txPin of HAL_UART
 * @param rxPin of HAL_UART
 * @param id of connection, relevant for topic assignment of messages received over this connection
 * @param messages pointer to allocatable space for new messages
 * @param newMessages pointer to new messages list
 */
Connection::Connection(UART_IDX uartIdx, GPIO_PIN txPin, GPIO_PIN rxPin, uint8_t id,
                       RODOS::AllocableObjects<Message, 40> *messages, Fifo<Message *, 40> *newMessages,
                       int *send) : halUart(
        uartIdx, txPin, rxPin) {
    this->id = id;
    this->send = send;
    rxBuf[0] = {0};
    this->messages = messages;
    this->newMessages = newMessages;
    halUart.init();
}

uint8_t *Connection::requestBufferToRead(int32_t &msgLen) {
    msgLen = static_cast<int32_t>(this->lengthOut);
    return this->bufferOut;
}

// performance evaluation
int received = 0;
int bad = 0;
int trashed = 0;
int64_t start = 0;
bool printed = false;

void Connection::endOfMessage(int32_t len) {
    Message *msg = messages->alloc();
    if (msg == nullptr) { // no free space in message bufferIn!  we have a problem!
        PRINTF("ERROR bufferIn full!\n");
        trashed++;
        return;
    }
    msg->len = static_cast<uint32_t>(len);
    msg->channelX = this->id;
    for (int i = 0; i < len; i++) {
        msg->data[i] = bufferIn[i];
    }

    if (this->newMessages->isFull()) {
        PRINTF("ERROR queue is full!\n");
    } else {
        received++;
        this->newMessages->put(msg);
    }
    if (len != MAX_NETWORK_MESSAGE_LENGTH) bad++;
//    // performance evaluation
//    if (NOW() - start > 120000000000 && !printed) {
//        PRINTF("sent:%d received:%d bad:%d trashed:%d\n", *this->send, received, bad, trashed);
//        printed = true;
//    }
}

uint8_t *Connection::requestBufferToWrite(int32_t &maxlen) {
    maxlen = MAX_NETWORK_MESSAGE_LENGTH;
    return bufferIn;
}

/**
 * Thread which runs indefinitely, checks for new messages receivals over uart and saves them into the new messages list
 */
[[noreturn]] void Connection::run() {
    suspendCallerUntil(NOW() + 1500 * MILLISECONDS);
    start = NOW();
    while (true) {
        halUart.suspendUntilDataReady();
        halUart.read(rxBuf, 1);
//        PRINTF("%c", rxBuf[0]);
        this->putByte(rxBuf[0]);
    }
}