//
// Created by aneso on 10/05/2022.
//

#include <iostream>
#include "../include/json.h"
#include "../include/date.h"
#include <string>

#ifndef SP_EXAM_TRADE_H
#define SP_EXAM_TRADE_H

using json = nlohmann::json;
using namespace std;
using namespace std::chrono;
using namespace date;

namespace stock_trading {

    struct trade {

        int32_t price;
        int32_t amount;

        explicit trade(const json& json_trade) {
            time = string_to_milliseconds(json_trade["time"]);
            price = json_trade["price"];
            amount = json_trade["amount"];
        }

        int64_t get_time_milliseconds() {
            return time.time_since_epoch().count();
        }

        time_point<system_clock, milliseconds> get_time() {
            return time;
        }

        friend bool operator<(const trade& a, const trade& b);
        friend bool operator>(const trade& a, const trade& b);



    private:
        time_point<system_clock, milliseconds> time;

        static time_point<system_clock, milliseconds> string_to_milliseconds(const string& s) {
            string date = s;
            std::replace(date.begin(), date.end(), '-', ' ');
            std::replace(date.begin(), date.end(), 'T', ' ');

            istringstream stream{date};
            time_point<system_clock, milliseconds> time;
            stream >> parse("%Y %m %d %T %z", time);
            return time;
        }
    };

    bool operator<(const trade& a, const trade& b) {
        return a.time < b.time;
    }

    bool operator>(const trade& a, const trade& b) {
        return b < a;
    }

} // stock_trading

#endif //SP_EXAM_TRADE_H
