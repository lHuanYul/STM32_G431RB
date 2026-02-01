#include "main/lcd_1inch47.h"
#include "spi.h"
#include "tim.h"

Lcd1I47Parametar lcd_1i47_h = {
    .const_h = {
        .lcd = &waveshare_1inch47,
        .spi_h = {
            .hspix = &hspi3,
            .MOSI = { GPIOC, GPIO_PIN_12 },
            .SCK  = { GPIOC, GPIO_PIN_10 },
            .NSS  = { GPIOA, GPIO_PIN_15 },
        },
        .DC  = { GPIOC, GPIO_PIN_11 },
        .RST = { GPIOD, GPIO_PIN_2  },
        // PB4
        .htimx = &htim16,
        .TIM_CHANNEL_x = TIM_CHANNEL_1,
    },
};
