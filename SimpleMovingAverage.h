//
// Created by deedeeaar on 21/06/2020.
//

#ifndef MERKLEREX_SIMPLEMOVINGAVERAGE_H
#define MERKLEREX_SIMPLEMOVINGAVERAGE_H

#include <string>
#include <vector>
#include <map>

class SimpleMovingAverage {
    const size_t m_size = 4;
    unsigned int m_index{0};
    std::vector<double> m_avgs;
public:
    SimpleMovingAverage();
    void insert(double avgPrice);
    double average() const;
};


#endif //MERKLEREX_SIMPLEMOVINGAVERAGE_H
