//
// Created by deedeeaar on 21/06/2020.
//

#ifndef MERKLEREX_ROLLINGAVERAGEMANAGER_H
#define MERKLEREX_ROLLINGAVERAGEMANAGER_H

#include <string>
#include <vector>
#include <map>

/**
 * This class manages a price average.
 * Prices are stored in a vector of doubles of size set by m_size, defaulted to 4.
 * The average returned will be the average of all elements contained in the vector.
 * The current position, the head, of the vector is contained in m_index, starting from 0, and contains the position
 * of the last element added. When this index reaches the end of the vector, it rolls over, overwriting the oldest
 * elements with new ones, and keeping the average of always the last m_size elements added.
 */
class RollingAverageManager {
    /** Size of the vector, initialized to 4 */
    const size_t m_size = 4;
    /** Index of the last element put in the vector */
    unsigned int m_index{0};
    /** Vector of last four price averages */
    std::vector<double> m_avgs;
public:
    /** Create a new instance of RollingAverageManager*/
    RollingAverageManager();
    /** Insert a new price average in the vector */
    void insert(double avgPrice);
    /** Calculate the average */
    double average() const;
};


#endif //MERKLEREX_ROLLINGAVERAGEMANAGER_H
