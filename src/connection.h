#include <s3p-asynchronous-interface.h>
#include <allocableobjects.h>
#include "message.h"

class Connection : public S3pReceiverAsyncrhonous, public S3pMsgSenderAsyncrhonous, public StaticThread<750> {
public:
    uint8_t *bufferOut{};
    uint32_t lengthOut{};
private:
    uint8_t id;
    int *send{};
    uint8_t rxBuf[1]{};
    RODOS::AllocableObjects<Message, 40> *messages{};
    Fifo<Message *, 40> *newMessages{};
    uint8_t bufferIn[MAX_NETWORK_MESSAGE_LENGTH]{};

public:
    Connection(UART_IDX uartIdx, GPIO_PIN txPin, GPIO_PIN rxPin, uint8_t id,
               RODOS::AllocableObjects<Message, 40> *messages, Fifo<Message *, 40> *newMessages, int *send);

    void endOfMessage(int32_t len) override;

    uint8_t *requestBufferToWrite(int32_t &maxlen) override;

    uint8_t *requestBufferToRead(int32_t &msgLen) override;

    HAL_UART halUart;

    [[noreturn]] void run() override;
};