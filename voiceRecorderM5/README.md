# voiceRecorderM5
A single channel voice recorder that uses the `M5Stack Go` microphone and the esp32 internal ADC/DAC.

![M5Stack Go Base](https://docs.m5stack.com/assets/img/product_pics/base/m5go_base_04.webp)

Instead of I2S, this sketch uses an ESP32 `hw_timer`, the built-in ADC/DAC and ISRs.<br>
One ISR to sample the mic through the ADC and a second ISR to stream samples to the DAC.

Because of the used 'technology', sample rate is limited to about 22kHz and sound quality is crap.
