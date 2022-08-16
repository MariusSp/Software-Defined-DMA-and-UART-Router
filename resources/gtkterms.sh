trap "kill 0" EXIT

gtkterm -p /dev/ttyUSB0 -s 115200 &
gtkterm -p /dev/ttyUSB1 -s 115200 &
gtkterm -p /dev/ttyUSB2 -s 115200 &

wait


