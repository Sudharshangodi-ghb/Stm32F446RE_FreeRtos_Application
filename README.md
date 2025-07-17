# stm32f446re - freertos
STM32F446RE Nucleo FreeRTOS with C Programming


# STM32F446RE Hardware Connections (Project IO configuration for each component)
|   Peripheral   |   Connected Device      |   STM32F446RE Pins                                   |   Notes                                                    |
| -------------- | ----------------------- | ---------------------------------------------------- | ---------------------------------------------------------- |
|   User LED     | LD2 (Green)             |  PA5                                                 | Active HIGH                                                |
|   Button       | B1 (User Button)        |  PC13                                                | Active LOW, pull-up enabled                                |
|   UART1        | HC-05 Bluetooth         |  PA9 (TX1) ,  PA10 (RX1)                             | For wireless serial communication                          |
|   UART2        | ST-LINK VCP (Debug)     |  PA2 (TX2) ,  PA3 (RX2)                              | Used for  printf() /debug                                  |
|   UART3        | GSM (SIM800)            |  PC10 (TX3) ,  PC11 (RX3)                            | Alternate to PB10/PB11                                     |
|   ADC1         | LM35 Temperature Sensor |  PA0 (ADC123_IN0)                                    | Analog voltage measurement                                 |
|   PWM (TIM4)   | Servo Motor             |  PB6 (TIM4_CH1)                                      | 50Hz PWM, duty cycle controlled                            |
|   SPI1         | MPU6050 (SPI option)    |  PB3 (SCK) ,  PB4 (MISO) ,  PB5 (MOSI) ,  PA4 (NSS)  | Use if MPU6050 is configured for SPI (usually I2C default) |
|   I2C3         | LCD (PCF8574 Backpack)  |  PC9 (SDA3) ,  PA8 (SCL3)                            | Alternate I2C if PB8/PB9 used for CAN                      |
|   CAN1         | MCP2551 Transceiver     |  PB8 (CAN1_RX) ,  PB9 (CAN1_TX)                      | Standard CAN interface                                     |

