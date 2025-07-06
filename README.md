# ESP32 BLE Relay Controller

Simple Bluetooth Low Energy relay controller for Waveshare ESP32-S3-Relay-6CH board.

## Pin Configuration
- Relays: GPIO 1, 2, 41, 42, 45, 46
- Buzzer: GPIO 21

## BLE Details
- Device Name: `ESP32 S3 Relay 6CH`
- Service UUID: `4fafc201-1fb5-459e-8fcc-c5c9c331914b`
- Characteristic UUID: `beb5483e-36e1-4688-b7f5-ea07361b26a8`

## Commands (Waveshare Protocol)
- `0x31`: Toggle Relay 1
- `0x32`: Toggle Relay 2
- `0x33`: Toggle Relay 3
- `0x34`: Toggle Relay 4
- `0x35`: Toggle Relay 5
- `0x36`: Toggle Relay 6
- `0x38`: All Relays OFF

## Serial Test Commands
- `1-6`: Toggle respective relay
- `0`: All relays off

## Code Location
`ESP32_SimpleBLE/ESP32_SimpleBLE.ino`