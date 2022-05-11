#include <iostream>
#include "stock.h"
#include "stock_parser.h"
#include "trade.h"





void file_to_candle_sticks_benchmarking(const double& iterations,
                                        const std::chrono::time_point<system_clock, milliseconds>& time_step,
                                        const string& stock,
                                        const bool& verbose = false) {
    double time_sum = 0;

    for (int i = 0; i < iterations; i++) {
        if (verbose) {
            cout << "Iteration " << i << endl;
        }
        auto t0 = chrono::high_resolution_clock::now();

        string file_path = "data/" + stock + ".json";
        stock_trading::stock_parser p(file_path);

        stock_trading::stock s = p.parse_to_stock();

        vector<stock_trading::candle_stick> result = s.compute_candlesticks(time_step);

        auto t1 = chrono::high_resolution_clock::now();

        time_sum += chrono::duration<double, milli>(t1-t0).count();
    }

    double average = time_sum / iterations;

    cout << stock << ": " << iterations << " iterations, ";
    if (average >= 1000) {
        cout << average / 1000 << " seconds";
    }
    else {
        cout << average << " milliseconds";
    }

    cout << " average" << endl;
}

int main() {
    std::chrono::time_point<system_clock, seconds> tp(duration<int>(60));
    // Biggest file
    file_to_candle_sticks_benchmarking(3, tp, "HMB", true);

    // Smallest file:
    file_to_candle_sticks_benchmarking(10, tp, "PAALB");

    // Half as big as biggest file:
    file_to_candle_sticks_benchmarking(3, tp, "NOVOB", true);
}