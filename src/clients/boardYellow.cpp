#include "modules/client.h"

static Application boardYellow("boardYellow", 2000);

Client client = Client(UART_IDX3, GPIO_026, GPIO_027);// TX: PB10, RX: PB11