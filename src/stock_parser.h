//
// Created by aneso on 10/05/2022.
//

#ifndef SP_EXAM_STOCK_PARSER_H
#define SP_EXAM_STOCK_PARSER_H

#include <iostream>
#include <fstream>
#include "../include/json.h"
#include "stock.h"

using json = nlohmann::json;

namespace stock_trading {

    struct stock_parser {
    private:
        std::string file_path;
        json parsed_stock;

    public:
        explicit stock_parser(const std::string& path) {
            file_path = path;
        }

        stock parse_to_stock() {
            if (parsed_stock.empty()) {
                std::ifstream file(file_path);
                file >> parsed_stock;
            }

            return std::move(stock(parsed_stock));
        }

        static stock parse_to_stock(const json& j) {
            return std::move(stock(j));
        }
    };

} // stock_trading

#endif //SP_EXAM_STOCK_PARSER_H
