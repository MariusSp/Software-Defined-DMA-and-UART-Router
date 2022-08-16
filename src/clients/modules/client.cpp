#include "client.h"

int send = 0;

Client::Client(UART_IDX uartIdx, GPIO_PIN txPin, GPIO_PIN rxPin)
        : connection(uartIdx, txPin, rxPin, &send) {
}

/**
 * sends periodically messages over its uart connection
 */
void Client::run() {
    PRINTF("initializing hardware...\n");
    uint32_t len = MAX_NETWORK_MESSAGE_LENGTH;
    int loopTime = 150;
    char msg[MAX_NETWORK_MESSAGE_LENGTH] = "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Duis autem vel eum iriure dolor in hendrerit in vulputate velit esse molestie consequat, vel illum dolore eu feugiat nulla facilisis at vero eros et accumsan et iusto odio dignissim qui blandit praesent luptatum zzril delenit augue duis dolore te feugait nulla facilisi. Lorem ipsum dolor sit amet, consectetuer adipiscing elit, sed diam nonummy nibh euismod tincidunt ut laoreet dolore magna aliquam erat volus";
    suspendCallerUntil(NOW() + 2000 * MILLISECONDS);

    PRINTF("beginning with data transmission every %dms...\n", loopTime);
    while (true) {
        connection.bufferOut = reinterpret_cast<uint8_t *>(msg);
        connection.lengthOut = len;
        uint16_t dualByte;
        uint16_t last = 0;
        while (true) {
            dualByte = connection.getByteOrNODATA();
            connection.halUart.write(&dualByte, 1);
            while (!connection.halUart.isWriteFinished()) {
                suspendCallerUntil(NOW() + 500 * NANOSECONDS);
            }
            if (last == 254 && dualByte == 3) {
                send++;
                break;
            }
            last = dualByte;
        }
        suspendCallerUntil(NOW() + loopTime * MILLISECONDS);
    }
}