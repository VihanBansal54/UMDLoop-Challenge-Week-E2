cat > README.md << 'EOF'
# UMD Loop E2 — Zephyr Device Drivers

Four independent out-of-tree Zephyr applications, each implementing one of
the E1 systems as a proper Zephyr driver (devicetree overlay + binding +
config/data/api/init driver structs + Kconfig toggle).

| Folder | System | Peripheral |
|---|---|---|
| system1_rgb_led | RGB LED from hex value | GPIO |
| system2_servo | Servo angle control | PWM |
| system3_tone | Tone with ADC-scaled duration | PWM + ADC |
| system4_comms | Byte receive over UART | UART/USART |

## Build & flash any system
    cd ~/zephyrproject
    west build -p always -b esp32_devkitc/esp32/procpu ~/umdloop-e2-zephyr-drivers/system1_rgb_led
    west flash --esp-device /dev/ttyUSB0
EOF
git add README.md
git commit -m "Add README"
git push
