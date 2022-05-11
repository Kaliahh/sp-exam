//
// Created by aneso on 10/05/2022.
//

#include "../src/stock_parser.h"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"


TEST_CASE("parse_to_stock_from_file") {

    std::string file_path = "test.json";
    stock_trading::stock_parser p(file_path);

    stock_trading::stock s = p.parse_to_stock();

    CHECK(s.tag == "TEST");
    CHECK(s.shares == 1234);
    CHECK(s.trades[0].amount == 456);
    CHECK(s.trades[0].price == 123);
    CHECK(s.trades[0].get_time_milliseconds() == 1);
    CHECK(s.trades[1].amount == 70);
    CHECK(s.trades[1].price == 381);
    CHECK(s.trades[1].get_time_milliseconds() == 1351670401000);
}

json create_test_json() {
    return {
            {"tag",    "TEST"},
            {"shares", 1000},
            {"trades", {
                               {
                                       {"time", "1970-01-01T00:00:00.000+0000"},
                                       {"price", 100},
                                       {"amount", 100}
                               },
                               {
                                       {"time", "1970-01-01T00:00:00.001+0000"},
                                       {"price", 300},
                                       {"amount", 300}
                               },
                               {
                                       {"time", "1970-01-01T00:00:00.002+0000"},
                                       {"price", 200},
                                       {"amount", 200}
                               },
                               {
                                       {"time", "1970-01-01T00:00:01.000+0000"},
                                       {"price", 300},
                                       {"amount", 300}
                               },
                               {
                                       {"time", "1970-01-01T00:00:02.000+0000"},
                                       {"price", 400},
                                       {"amount", 400}
                               },
                               {
                                       {"time", "1970-01-01T00:01:00.000+0000"},
                                       {"price", 500},
                                       {"amount", 500}
                               },
                               {
                                       {"time", "1970-01-01T00:01:30.000+0000"},
                                       {"price", 50},
                                       {"amount", 50}
                               },
                               {
                                       {"time", "1970-01-01T00:01:30.000+0000"},
                                       {"price", 400},
                                       {"amount", 400}
                               },
                               {
                                       {"time", "1970-01-01T00:02:00.000+0000"},
                                       {"price", 600},
                                       {"amount", 600}
                               }
                       }
            }
    };
}

TEST_CASE("stock_parser_directly_from_json") {
    stock_trading::stock s = stock_trading::stock_parser::parse_to_stock(create_test_json());

    CHECK(s.tag == "TEST");
    CHECK(s.shares == 1000);
    CHECK(s.trades[0].amount == 100);
    CHECK(s.trades[0].price == 100);
    CHECK(s.trades[0].get_time_milliseconds() == 0);
    CHECK(s.trades[0].get_time() == std::chrono::time_point<system_clock, milliseconds>(duration<int, milli>(0)));
    CHECK(s.trades[1].amount == 300);
    CHECK(s.trades[1].price == 300);
    CHECK(s.trades[1].get_time_milliseconds() == 1);
    CHECK(s.trades[1].get_time() == std::chrono::time_point<system_clock, milliseconds>(duration<int, milli>(1)));
    CHECK(s.trades[3].get_time() == std::chrono::time_point<system_clock, seconds>(duration<int>(1)));
    CHECK(s.trades[6].get_time() == std::chrono::time_point<system_clock, seconds>(duration<int>(90)));
}

json create_test_json_with_double() {
    return {
            {"tag",    "TEST"},
            {"shares", 1000},
            {"trades",
                       {
                               {
                                       {"time", "1970-01-01T00:00:00.000+0000"},
                                       {"price", 100.1},
                                       {"amount", 100}
                               }
                       }
            }
    };
}

TEST_CASE("stock_parser_directly_from_json_with_double") {
    stock_trading::stock s = stock_trading::stock_parser::parse_to_stock(create_test_json_with_double());

    CHECK(s.trades[0].price == 100.1);
}


TEST_CASE("trades_can_be_ordered") {
    stock_trading::stock s = stock_trading::stock_parser::parse_to_stock(create_test_json());

    CHECK(s.trades[0] < s.trades[1]);
    CHECK(!(s.trades[1] < s.trades[0]));
    CHECK(s.trades[1] > s.trades[0]);
    CHECK(!(s.trades[0] > s.trades[1]));
}

TEST_CASE("sort_trades_ascending") {
    stock_trading::stock s = stock_trading::stock_parser::parse_to_stock(create_test_json());

    s.sort_trades(std::less<>());

    CHECK(s.trades[0] < s.trades[1]);
}

TEST_CASE("sort_trades_descending") {
    stock_trading::stock s = stock_trading::stock_parser::parse_to_stock(create_test_json());

    s.sort_trades(std::greater<>());

    CHECK(s.trades[0] > s.trades[1]);
}


TEST_CASE("compute_candle_sticks_one_millisecond") {
    stock_trading::stock s = stock_trading::stock_parser::parse_to_stock(create_test_json());

    std::chrono::time_point<system_clock, milliseconds> tp(duration<int, milli>(1));

    vector<stock_trading::candle_stick> result = s.compute_candlesticks(tp);

    CHECK(result[0].opening_price == 100);
    CHECK(result[0].closing_price == 100);
    CHECK(result[0].maximum_price == 100);
    CHECK(result[0].minimum_price == 100);

    CHECK(result[1].opening_price == 100);
    CHECK(result[1].closing_price == 300);
    CHECK(result[1].maximum_price == 300);
    CHECK(result[1].minimum_price == 100);

    CHECK(result[2].opening_price == 300);
    CHECK(result[2].closing_price == 200);
    CHECK(result[2].maximum_price == 300);
    CHECK(result[2].minimum_price == 200);
}

TEST_CASE("compute_candle_sticks_one_second") {
    stock_trading::stock s = stock_trading::stock_parser::parse_to_stock(create_test_json());

    std::chrono::time_point<system_clock, seconds> tp(duration<int>(1));

    vector<stock_trading::candle_stick> result = s.compute_candlesticks(tp);

    CHECK(result[0].opening_price == 100);
    CHECK(result[0].closing_price == 200);
    CHECK(result[0].maximum_price == 300);
    CHECK(result[0].minimum_price == 100);

    CHECK(result[1].opening_price == 200);
    CHECK(result[1].closing_price == 300);
    CHECK(result[1].maximum_price == 300);
    CHECK(result[1].minimum_price == 200);

    CHECK(result[2].opening_price == 300);
    CHECK(result[2].closing_price == 400);
    CHECK(result[2].maximum_price == 400);
    CHECK(result[2].minimum_price == 300);
}

TEST_CASE("compute_candle_stick_one_minute") {
    stock_trading::stock s = stock_trading::stock_parser::parse_to_stock(create_test_json());

    std::chrono::time_point<system_clock, seconds> tp(duration<int>(60));

    vector<stock_trading::candle_stick> result = s.compute_candlesticks(tp);

    CHECK(result[0].opening_price == 100);
    CHECK(result[0].closing_price == 400);
    CHECK(result[0].maximum_price == 400);
    CHECK(result[0].minimum_price == 100);

    CHECK(result[1].opening_price == 400);
    CHECK(result[1].closing_price == 400);
    CHECK(result[1].maximum_price == 500);
    CHECK(result[1].minimum_price == 50);

    CHECK(result[2].opening_price == 400);
    CHECK(result[2].closing_price == 600);
    CHECK(result[2].maximum_price == 600);
    CHECK(result[2].minimum_price == 400);
}

json create_test_json_for_oscillator() {
    return {
            {"tag",    "TEST"},
            {"shares", 1000},
            {"trades",
                       {
                               {
                                       {"time", "1970-01-01T00:00:00.000+0000"},
                                       {"price", 100},
                                       {"amount", 100}
                               },
                               {
                                       {"time", "1970-01-01T00:01:00.000+0000"},
                                       {"price", 300},
                                       {"amount", 100}
                               }
                       }
            }
    };
}

TEST_CASE("calculate_oscillator") {
    stock_trading::stock s = stock_trading::stock_parser::parse_to_stock(create_test_json_for_oscillator());

    std::chrono::time_point<system_clock, seconds> time_step(duration<int>(60));

    vector<stock_trading::candle_stick> candle_sticks = s.compute_candlesticks(time_step);

    std::chrono::time_point<system_clock, seconds> long_look_back(duration<int>(60 * 10));
    std::chrono::time_point<system_clock, seconds> short_look_back(duration<int>(60 * 2));

    auto result = stock_trading::stock::compute_oscillator(candle_sticks, long_look_back, short_look_back);

    double most_recent_closing_price = 200;
    double lowest_price_traded_of_X_previous_trading_sessions = 100;
    double highest_price_traded_of_X_previous_trading_sessions = 200;

    double fast_oscillator = ((most_recent_closing_price - lowest_price_traded_of_X_previous_trading_sessions) /
            (highest_price_traded_of_X_previous_trading_sessions - lowest_price_traded_of_X_previous_trading_sessions))
            * 100;

    double slow_oscillator = fast_oscillator / 2;

    CHECK(result[0].fast_oscillator == fast_oscillator);
    CHECK(result[0].slow_oscillator == slow_oscillator);
}