# [Sistemi Operativi] Hermes
## Communication between multiple Arduino(s) with nrf24L01 radio module
According to some, Hermes is considered the personification of the wind, in fact he inherits: lightness, speed, inconstancy in purposes, joking mood.
[Hermes, Greek mythology](http://mitologiagreca.blogspot.com/2007/07/hermes.html)

## Reference
[nrf24L01 Library of Kehribar](https://github.com/kehribar/nrf24L01_plus)


## Devices
[nrf24L01 datasheet](https://www.sparkfun.com/datasheets/Components/SMD/nRF24L01Pluss_Preliminary_Product_Specification_v1_0.pdf)

## Pins
I/O from arduino's view
*  nrf24L01
    *  SPI (Serial Peripheral Interface) Protocol:
        *  (I) MISO -> Slave's pin for sending data to the master
        *  (O) MOSI -> Master's pin for sending data to the slaves
        *  (O) SCK  -> The clock pulses (synchronize data transmission generated by the master)
        *  (O) CSN  -> Chip select
    *  Register map:
        * (O) CE    -> Chip that sets RX or TX mode
    
