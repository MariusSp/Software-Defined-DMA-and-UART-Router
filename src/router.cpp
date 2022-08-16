#include "router.h"
#include "connection.h"


namespace uartRouter {

    static Application uartRouter("Software Defined DMA and uartRouter");

    Fifo<Message *, 40> newMessages;                  // From all input channels to the router
    AllocableObjects<Message, 40> messages;           // All pending messages to be forwarded
    TopicListReport subscribers[NUM_OF_UARTS];        // Which channel/UART is expecting which topics

    int send = 0;

    //__________________________________________
    // DEFINE YOUR UART CONNECTIONS BELOW
    //__________________________________________
    //initialize connections of UARTs
    Connection halUartBlue = Connection(UART_IDX4, GPIO_043, GPIO_042, 0, // TX: PC11, RX: PC10
                                        &messages, &newMessages, &send);
    Connection halUartYellow = Connection(UART_IDX3, GPIO_026, GPIO_027, 1, // TX: PB10, RX: PB11
                                          &messages, &newMessages, &send);
    Connection connections[2] = {halUartBlue, halUartYellow};

    //__________________________________________
    // ASSIGN YOUR TOPICS BELOW
    //__________________________________________
    /**
     * Defines which connection/subscriber wants to receive messages from which topic/client
     */
    void initTopics() {
        subscribers[0].add(1);
        subscribers[1].add(0);
    }

    //__________________________________________
    class SwDefinedDma : public StaticThread<3400> {

        /**
         * routes messages
         */
        [[noreturn]] void run() override {
            //initialisation
            PRINTF("initializing hardware...\n");
            messages.init();
            initTopics();
            Message *msg;
            uint16_t dualByte;
            suspendCallerUntil(NOW() + 1500 * MILLISECONDS);

            //routes messages
            PRINTF("beginning with routing...\n");
            while (true) {
                while (newMessages.get(msg)) { // read descriptor of all new arrived messages
                    // add message to each subscriber, who is interested in it
                    for (uint32_t i = 0; i < NUM_OF_UARTS; i++) { // Forward to all channels which expect it
                        if (i == msg->channelX) continue; // do not send it back
                        if (!subscribers[i].find(msg->channelX)) continue;

                        connections[i].bufferOut = reinterpret_cast<uint8_t *>(msg->data);
                        connections[i].lengthOut = msg->len;
                        uint16_t last = 0;
                        while (true) {
                            dualByte = connections[i].getByteOrNODATA();
                            connections[i].halUart.write(&dualByte, 1);
                            while (!connections[i].halUart.isWriteFinished()) {
                                suspendCallerUntil(NOW() + 500 * NANOSECONDS);
                            }
                            if (last == 254 && dualByte == 3) {
                                break;
                            }
                            last = dualByte;
                        }

                    }
                    send++;
                    messages.free(msg);
                }
                suspendCallerUntil(NOW() + 100 * MILLISECONDS);
            }
        }
    } swDefinedDma;
}