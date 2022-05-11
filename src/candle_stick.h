//
// Created by aneso on 11/05/2022.
//

#include <iostream>
#include <vector>

#ifndef SP_EXAM_CANDLE_STICK_H
#define SP_EXAM_CANDLE_STICK_H

namespace stock_trading {

    struct candle_stick {
        double opening_price;
        double closing_price;
        double maximum_price;
        double minimum_price;

        void set_initial(double initial) {
            opening_price = initial;
            closing_price = initial;
            maximum_price = initial;
            minimum_price = initial;
        }
    };

} // stock_trading

#endif //SP_EXAM_CANDLE_STICK_H
