//
// Created by victoryang00 on 1/14/23.
//

#include "cxlcontroller.h"

std::vector<std::string> tokenize(const std::string_view &s) {
    std::vector<std::string> res;
    std::string tmp;
    for (char c : s) {
        if (c == '(' || c == ')' || c == ':' || c == ',') {
            if (!tmp.empty()) {
                res.emplace_back(std::move(tmp));
            }
            res.emplace_back(1, c);
        } else {
            tmp += c;
        }
    }
    if (!tmp.empty()) {
        res.emplace_back(std::move(tmp));
    }
    return res;
}
void CXLController::insert_end_point(CXLMemExpander *end_point) { this->expanders.emplace_back(end_point); }
void CXLController::construct_topo(std::string_view newick_tree) {
    auto tokens = tokenize(newick_tree);
    std::vector<CXLEndPoint *> stk;
    stk.push_back(this);
    for (const auto &token : tokens) {
        if (token == "(") {
            auto cur = new CXLSwitch(num_switches++);
            reinterpret_cast<CXLSwitch *>(stk.back())->switches.push_back(cur);
        } else if (token == ")") {
            if (!stk.empty()) {
                stk.pop_back();
            } else {
                throw std::invalid_argument("Unbalanced number of parentheses");
            }
        } else if (token == ",") {
            if (!stk.empty()) {
                stk.pop_back();
            } else {
                throw std::invalid_argument("Unbalanced number of parentheses");
            }
            auto cur = new CXLSwitch(num_switches++);
            reinterpret_cast<CXLSwitch *>(stk.back())->switches.push_back(cur);
        } else {
            reinterpret_cast<CXLSwitch *>(stk.back())->expanders.emplace_back(this->expanders[atoi(token.c_str())]);
        }
    }
}

CXLController::CXLController(Policy p) { this->policy = p; }
double CXLController::calculate_latency(double weight, struct Elem *elem) { return 0; }
double CXLController::calculate_bandwidth(double weight, struct Elem *elem) { return 0; }
void CXLController::print() {}
