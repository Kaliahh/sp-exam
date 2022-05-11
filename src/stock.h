//
// Created by aneso on 10/05/2022.
//

#include <iostream>
#include <vector>
#include "trade.h"
#include "candle_stick.h"
#include "../include/json.h"

#ifndef SP_EXAM_STOCK_H
#define SP_EXAM_STOCK_H

using json = nlohmann::json;

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

        vector<candle_stick> compute_candlesticks(const std::chrono::time_point<system_clock, milliseconds>& time_step) {
            vector<candle_stick> result{};
            auto time_pivot = trades[0].get_time() + time_step.time_since_epoch();
            auto index = 0;

            while (index < trades.size()) {
                candle_stick cs{};

                if (trades[index].get_time() < time_pivot) {
                    if (index == 0) {
                        cs.set_initial(trades[index].price);
                    }
                    else {
                        cs.set_initial(result[result.size() - 1].closing_price);
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
                    cs.closing_price = trades[index].price;
                    index++;
                }

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
