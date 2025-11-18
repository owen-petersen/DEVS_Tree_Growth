#ifndef TREE_TYPE_EN_HPP
#define TREE_TYPE_EN_HPP


#include <iostream>

enum class TreeTypeEn : int {
    oak = 0,
};

std::ostream& operator<<(std::ostream& os, TreeTypeEn e) {
    switch (e) {
        case TreeTypeEn::oak:       os << "oak";   break;
        default:                    os << "UnknownTreeType"; break;
    }
    return os;
}

#endif