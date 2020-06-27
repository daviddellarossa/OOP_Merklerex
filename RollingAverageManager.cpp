//
// Created by deedeeaar on 21/06/2020.
//

#include "RollingAverageManager.h"
#include <numeric>
#include <algorithm>

void RollingAverageManager::insert(double avgPrice) {
    //Calculate the next position
    ++m_index %= m_size;
    //Add the price
    m_avgs[m_index] = avgPrice;
}



double RollingAverageManager::average() const {
    //Calculate the number non-zero elements in the array
    //Element with value 0 are excluded from the average calculation
    auto countNonZero = std::count_if(m_avgs.begin(), m_avgs.end(), [](auto x){ return x != 0;});
    //If all elements are zero, return 0
    if(countNonZero == 0)
        return 0;
    //Do the math
    return std::accumulate(m_avgs.begin(), m_avgs.end(), 0.0)/ countNonZero;
}

RollingAverageManager::RollingAverageManager() : m_index{0}, m_avgs(m_size, 0) {

}
