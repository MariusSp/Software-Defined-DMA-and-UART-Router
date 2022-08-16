#include "rodos.h"
#include "clientConnection.h"
#include "../../message.h"

class Client : StaticThread<5000> {
    Connection connection;
public:
    Client(UART_IDX uartIdx, GPIO_PIN txPin, GPIO_PIN rxPin);

    [[noreturn]] void run() override;
};