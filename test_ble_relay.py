#!/usr/bin/env python3
"""
Test BLE connection to ESP32 S3 Relay 6CH
"""

import asyncio
from bleak import BleakClient, BleakScanner

# Device address and UUIDs from your code
DEVICE_ADDRESS = "F0:9E:9E:55:1A:81"
SERVICE_UUID = "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
CHARACTERISTIC_UUID = "beb5483e-36e1-4688-b7f5-ea07361b26a8"

async def test_relay():
    print("Searching for ESP32 S3 Relay 6CH...")
    
    try:
        # Connect to the device
        async with BleakClient(DEVICE_ADDRESS) as client:
            print(f"Connected to {DEVICE_ADDRESS}")
            
            # List services
            services = await client.get_services()
            print("\nServices found:")
            for service in services:
                print(f"  {service.uuid}: {service.description}")
            
            # Test relay commands
            commands = [
                (0x31, "Toggle Relay 1"),
                (0x32, "Toggle Relay 2"),
                (0x38, "Turn all relays OFF")
            ]
            
            for cmd, description in commands:
                print(f"\nSending command: {description} (0x{cmd:02X})")
                await client.write_gatt_char(CHARACTERISTIC_UUID, bytes([cmd]))
                await asyncio.sleep(2)  # Wait 2 seconds between commands
                
            print("\nTest complete!")
            
    except Exception as e:
        print(f"Error: {e}")

async def scan_devices():
    print("Scanning for BLE devices...")
    devices = await BleakScanner.discover()
    for device in devices:
        if device.name and "ESP32" in device.name:
            print(f"Found: {device.address} - {device.name}")

if __name__ == "__main__":
    # First scan for devices
    asyncio.run(scan_devices())
    
    # Then test the relay control
    print("\n" + "="*50 + "\n")
    asyncio.run(test_relay())