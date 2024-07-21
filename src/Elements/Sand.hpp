#ifndef Sand_hpp
#define Sand_hpp
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <random>
#include "BaseElements/SolidDynamic.hpp"

class Sand : public SolidDynamic {
public:
    Sand();
    ~Sand();
    void update(std::vector<std::vector<Pixel *>> &vec, int &row, int &col, int &vecWidth, int &vecHeight) override;
};


#endif /* Sand_hpp */
