//
// Created by aneso on 11/05/2022.
//

#include <iostream>
#include <vector>

#ifndef SP_EXAM_CANDLE_STICK_H
#define SP_EXAM_CANDLE_STICK_H

namespace stock_trading {

    struct candle_stick {
        double opening_price{};
        double closing_price{};
        double maximum_price{};
        double minimum_price{};
        time_point<system_clock, milliseconds> opening_time{};
        time_point<system_clock, milliseconds> closing_time{};

        void set_initial(double price, time_point<system_clock, milliseconds> time) {
            opening_price = price;
            closing_price = price;
            maximum_price = price;
            minimum_price = price;
            opening_time = time;
            closing_time = time;
        }
    };

} // stock_trading

#endif //SP_EXAM_CANDLE_STICK_H
