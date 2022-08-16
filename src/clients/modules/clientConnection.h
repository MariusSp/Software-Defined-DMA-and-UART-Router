#include <s3p-asynchronous-interface.h>
#include <allocableobjects.h>
#include "../../message.h"

class Connection : public S3pReceiverAsyncrhonous, public S3pMsgSenderAsyncrhonous, public StaticThread<1000> {
public:
    uint8_t *bufferOut{};
    int *send{};
    uint32_t lengthOut{};
private:
    uint8_t rxBuf[1]{};
    uint8_t bufferIn[MAX_NETWORK_MESSAGE_LENGTH]{};

public:
    Connection(UART_IDX uartIdx, GPIO_PIN txPin, GPIO_PIN rxPin, int *send);

    void endOfMessage(int32_t len) override;

    uint8_t *requestBufferToWrite(int32_t &maxlen) override;

    uint8_t *requestBufferToRead(int32_t &msgLen) override;

    HAL_UART halUart;

    [[noreturn]] void run() override;
};