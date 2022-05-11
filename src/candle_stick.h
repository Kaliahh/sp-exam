//
// Created by aneso on 11/05/2022.
//

#include <iostream>
#include <vector>

#ifndef SP_EXAM_CANDLE_STICK_H
#define SP_EXAM_CANDLE_STICK_H

namespace stock_trading {

    struct candle_stick {
        int32_t opening_price;
        int32_t closing_price;
        int32_t maximum_price;
        int32_t minimum_price;

        void set_initial(int32_t initial) {
            opening_price = initial;
            closing_price = initial;
            maximum_price = initial;
            minimum_price = initial;
        }
    };

} // stock_trading

#endif //SP_EXAM_CANDLE_STICK_H
