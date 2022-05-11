//
// Created by aneso on 11/05/2022.
//

#ifndef SP_EXAM_OSCILLATOR_RESULT_H
#define SP_EXAM_OSCILLATOR_RESULT_H

namespace stock_trading {

    struct oscillator_result {
        double fast_oscillator = 0;
        double slow_oscillator = 0;
        bool oversold = false;
        bool overbought = false;
        int trend = 0;
        std::chrono::time_point<system_clock, milliseconds> period_end;
    };

} // stock_trading

#endif //SP_EXAM_OSCILLATOR_RESULT_H
