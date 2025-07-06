#!/bin/bash
# Simple script to send BLE commands to ESP32 relay

DEVICE="F0:9E:9E:55:1A:81"
SERVICE_UUID="4fafc201-1fb5-459e-8fcc-c5c9c331914b"
CHAR_UUID="beb5483e-36e1-4688-b7f5-ea07361b26a8"

echo "Testing ESP32 S3 Relay 6CH Bluetooth control..."

# Toggle relay 1 (0x31)
echo "Toggling Relay 1..."
echo -e "connect $DEVICE\nmenu gatt\nselect-attribute /org/bluez/hci0/dev_F0_9E_9E_55_1A_81/service0025/char0026\nwrite 0x31\nexit" | bluetoothctl

sleep 2

# Toggle relay 2 (0x32)
echo "Toggling Relay 2..."
echo -e "menu gatt\nselect-attribute /org/bluez/hci0/dev_F0_9E_9E_55_1A_81/service0025/char0026\nwrite 0x32\nexit" | bluetoothctl

sleep 2

# Turn all off (0x38)
echo "Turning all relays OFF..."
echo -e "menu gatt\nselect-attribute /org/bluez/hci0/dev_F0_9E_9E_55_1A_81/service0025/char0026\nwrite 0x38\nexit" | bluetoothctl

echo "Test complete!"