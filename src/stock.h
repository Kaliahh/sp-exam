//
// Created by aneso on 10/05/2022.
//

#include <iostream>
#include <vector>
#include "trade.h"
#include "candle_stick.h"
#include "oscillator_result.h"
#include "../include/json.h"

#ifndef SP_EXAM_STOCK_H
#define SP_EXAM_STOCK_H

using json = nlohmann::json;

// TODO: Oscillator class


namespace stock_trading {

    struct stock {
        std::string tag;
        long long shares{};
        std::vector<trade> trades;

        stock() = default;

        explicit stock(const json& parsed_stock) {
            tag = parsed_stock["tag"];
            shares = parsed_stock["shares"];
            trades = std::vector<trade>{};

            for (const auto& json_trade : parsed_stock["trades"]) {
                trades.emplace_back(trade(json_trade));
            }

            sort_trades(std::less<>());
        }

        static vector<stock_trading::oscillator_result> compute_oscillator(const vector<candle_stick>& candle_sticks,
                                                                           const std::chrono::time_point<system_clock, milliseconds>& long_look_back,
                                                                           const std::chrono::time_point<system_clock, milliseconds>& short_look_back) {
            vector<stock_trading::oscillator_result> result{};
            auto index = 0;

            while (index < candle_sticks.size()) {

                auto [maximum, minimum] = calculate_maximum_and_minimum(candle_sticks, long_look_back, index);

                // Avoid divide by zero
                if (maximum == minimum) {
                    index++;
                    continue;
                }

                oscillator_result temp{};
                temp.period_end = candle_sticks[index].closing_time;
                temp.fast_oscillator = calculate_fast_oscillator(candle_sticks[index].closing_price,
                                                                 maximum,
                                                                 minimum);

                temp.slow_oscillator = calculate_slow_oscillator(result, temp.fast_oscillator, short_look_back);

                set_flags(temp);

                result.emplace_back(temp);

                index++;
            }

            return result;
        }

        static void set_flags(oscillator_result& temp) {
            if (temp.fast_oscillator <= 20 && temp.slow_oscillator <= 20) {
                temp.oversold = true;
            }
            else if (temp. fast_oscillator >= 80 && temp.slow_oscillator >= 80) {
                temp.overbought = true;
            }

            if (temp.fast_oscillator > temp.slow_oscillator) {
                temp.trend = 1;
            }
            else if (temp.fast_oscillator < temp.slow_oscillator) {
                temp.trend = -1;
            }
            else {
                temp.trend = 0;
            }
        }

        static double calculate_fast_oscillator(const double& closing_price, const double& maximum, const double& minimum) {
            return ((closing_price - minimum) / (maximum - minimum)) * 100;
        }

        static double calculate_slow_oscillator(const vector<oscillator_result>& result, double initial, const std::chrono::time_point<system_clock, milliseconds>& short_look_back) {
            auto sum = initial;
            auto iterations = 1;

            if (!result.empty()) {

                std::chrono::time_point<system_clock, milliseconds> start_time = (*std::rbegin(result)).period_end;

                for (auto i = std::rbegin(result) ; i < std::rend(result); i++) {
                    if ((*i).period_end.time_since_epoch() >= start_time - short_look_back) {
                        sum += (*i).fast_oscillator;
                        iterations++;
                    }
                    else {
                        break;
                    }
                }
            }

            return sum / iterations;
        }

        static tuple<double, double> calculate_maximum_and_minimum(const vector<candle_stick>& candle_sticks,
                                                            const std::chrono::time_point<system_clock, milliseconds>& long_look_back,
                                                            int index) {
            auto maximum_price_for_period = candle_sticks[index].maximum_price;
            auto minimum_price_for_period = candle_sticks[index].minimum_price;

            for (int i = index; i >= 0; i--) {
                if (candle_sticks[i].closing_time.time_since_epoch() >= (candle_sticks[index].opening_time - long_look_back)) {
                    if (candle_sticks[i].maximum_price > maximum_price_for_period) {
                        maximum_price_for_period = candle_sticks[i].maximum_price;
                    }
                    else if (candle_sticks[i].minimum_price < minimum_price_for_period) {
                        minimum_price_for_period = candle_sticks[i].minimum_price;
                    }
                }
                else {
                    break;
                }
            }

            return {maximum_price_for_period, minimum_price_for_period};
        }



        vector<candle_stick> compute_candlesticks(const std::chrono::time_point<system_clock, milliseconds>& time_step) {
            vector<candle_stick> result{};
            auto time_pivot = trades[0].get_time() + time_step.time_since_epoch();
            auto index = 0;

            while (index < trades.size()) {
                candle_stick cs{};

                if (trades[index].get_time() < time_pivot) {
                    if (index == 0) {
                        cs.set_initial(trades[index].price, trades[index].get_time());
                    }
                    else {
                        cs.set_initial(result[result.size() - 1].closing_price, trades[index].get_time());
                    }
                }
                else {
                    time_pivot += time_step.time_since_epoch();
                    continue;
                }

                while (index < trades.size() && trades[index].get_time() < time_pivot) {
                    if (trades[index].price > cs.maximum_price) {
                        cs.maximum_price = trades[index].price;
                    }
                    else if (trades[index].price < cs.minimum_price) {
                        cs.minimum_price = trades[index].price;
                    }
                    index++;
                }
                cs.closing_price = trades[index - 1].price;
                cs.closing_time = trades[index - 1].get_time();

                result.emplace_back(cs);

                time_pivot += time_step.time_since_epoch();
            }

            return result;
        }

        template<class SortFunc>
        void sort_trades(SortFunc func) {
            std::sort(std::begin(trades), std::end(trades),
                      [func](const auto& a, const auto& b) {return func(a, b); });
        }
    };

} // stock_trading

#endif //SP_EXAM_STOCK_H
