#pragma once
#include <random>

namespace rng {
    inline std::mt19937& getEngine() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        return gen;
    }
}