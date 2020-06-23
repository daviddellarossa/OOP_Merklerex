//
// Created by deedeeaar on 21/06/2020.
//

#include "RollingAverageManager.h"
#include <numeric>
#include <algorithm>

void RollingAverageManager::insert(double avgPrice) {
    ++m_index %= m_size;
    m_avgs[m_index] = avgPrice;
}



double RollingAverageManager::average() const {
    auto countNonZero = std::count_if(m_avgs.begin(), m_avgs.end(), [](auto x){ return x != 0;});
    if(countNonZero == 0)
        return 0;
    return std::accumulate(m_avgs.begin(), m_avgs.end(), 0.0)/ countNonZero;
}

RollingAverageManager::RollingAverageManager() : m_index{0}, m_avgs(m_size, 0) {

}
