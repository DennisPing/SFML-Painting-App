/** 
 *  @file   MathUtility.cpp 
 *  @brief  Math utility functions
 *  @author Dennis Ping 
 *  @date   2021-11-13
 ***********************************************/

#include <vector>
#include <cmath>
#include <iostream>
#include <queue>
#include <set>

#include "MathUtility.hpp"
#include "App.hpp"

/*! \brief  Return a vector of intermediate pixels between (x1,y1) and (x2,y2).
*           This is the Extremely Fast Line Algorithm (EFLA) Variation E
*           Author: Po-Han Lin (2005)
*           Source: http://www.edepot.com/algorithm.html 
*/
std::vector<std::pair<int, int>> MathUtility::ExtremelyFastLineAlgo(int x1, int y1, int x2, int y2) {
    std::vector<std::pair<int, int>> pixelsVector;
        bool yLonger = false;
        int shortLen = y2 - y1;
        int longLen = x2 - x1;
        if (abs(shortLen) > abs(longLen)) {
            int swap = shortLen;
            shortLen = longLen;
            longLen = swap;				
            yLonger = true;
        }
        int decInc;
        if (longLen == 0) decInc = 0;
        else decInc = (shortLen << 16) / longLen;

        if (yLonger) {
            if (longLen > 0) {
                longLen += y1;
                for (int j = 0x8000 + (x1 << 16); y1 <= longLen; ++y1) {
                    // Make a new pair and push it back into the vector
                    pixelsVector.emplace_back(std::make_pair(j >> 16, y1));	
                    j += decInc;
                }
                return pixelsVector;
            }
            longLen += y1;
            for (int j = 0x8000 + (x1 << 16); y1 >= longLen; --y1) {
                pixelsVector.emplace_back(std::make_pair(j >> 16, y1));
                j -= decInc;
            }
            return pixelsVector;
        }

        if (longLen > 0) {
            longLen += x1;
            for (int j = 0x8000 + (y1 << 16); x1 <= longLen; ++x1) {
                pixelsVector.emplace_back(std::make_pair(x1, j >> 16));
                j += decInc;
            }
            return pixelsVector;
        }
        longLen += x1;
        for (int j = 0x8000 + (y1 << 16); x1 >= longLen; --x1) {
            pixelsVector.emplace_back(std::make_pair(x1, j >> 16));
            j -= decInc;
        }
        return pixelsVector;
}

/*! \brief  Return a vector of a filled in circle using a modified version of Bresenham's Circle Algorithm.
*           Cache this circle template so that App does not need to always compute this.
*           Author: Linus Arver (2021)
*           Source: https://funloop.org/post/2021-03-15-bresenham-circle-drawing-algorithm.html 
*/
std::vector<std::pair<int,int>> MathUtility::BresenhamCircleAlgo(int radius) {
    std::set<std::pair<int, int>> outerSet;
    int x = 0;
    int y = -radius;
    int F_M = 1 - radius;
    int dir_east = 3;
    int dir_northeast = -(radius << 1) + 5;
    // Emplace all the mirror points of (x,y)
    outerSet.emplace(std::make_pair(x, y));
    outerSet.emplace(std::make_pair(x, -y));
    outerSet.emplace(std::make_pair(-x, y));
    outerSet.emplace(std::make_pair(-x, -y));
    outerSet.emplace(std::make_pair(y, x));
    outerSet.emplace(std::make_pair(y, -x));
    outerSet.emplace(std::make_pair(-y, x));
    outerSet.emplace(std::make_pair(-y, -x));

    while (x < -y) {
        if (F_M <= 0) {
            F_M += dir_east;
        } else {
            F_M += dir_northeast;
            dir_northeast += 2;
            y += 1;
        }
        dir_east += 2;
        dir_northeast += 2;
        x += 1;
        // Emplace all the mirror points of (x,y)
        outerSet.emplace(std::make_pair(x, y));
        outerSet.emplace(std::make_pair(x, -y));
        outerSet.emplace(std::make_pair(-x, y));
        outerSet.emplace(std::make_pair(-x, -y));
        outerSet.emplace(std::make_pair(y, x));
        outerSet.emplace(std::make_pair(y, -x));
        outerSet.emplace(std::make_pair(-y, x));
        outerSet.emplace(std::make_pair(-y, -x));
    }

    // O(n^2) loop to fill pixels in because math is hard.
    // It's ok, we will cache this circle template anyway.
    for (int j=0; j < radius; j++) {
        for (int k=0; k < radius; k++) {
            if (pow(j, 2) + pow(k, 2) < pow(radius, 2)) {
                outerSet.emplace(std::make_pair(j,k));
                outerSet.emplace(std::make_pair(j,-k));
                outerSet.emplace(std::make_pair(-j,k));
                outerSet.emplace(std::make_pair(-j,-k));
            }
        }
    }
    // Convert our set to a vector for easier access
    std::vector<std::pair<int,int>> circleTemplate;
    // Reserve memory in vector to get 4x performance: https://github.com/facontidavide/CPP_Optimizations_Diary/blob/master/docs/reserve.md 
    circleTemplate.reserve((outerSet.size()*8));
    // Use std::transform to fill the vector
    std::transform(outerSet.begin(), outerSet.end(), std::back_inserter(circleTemplate), [](const std::pair<int,int>& p) { return p; });
    return circleTemplate;
}

/*! \brief  A pairing function that maps two values to a single unique value.
*           Essentially a hash function for pairs of signed integers.
*           The C++ std lib pair hash only works for combinations, not permutations where order matters.
*           Author: Matthew Szudzik (2006)
*           Source: https://www.vertexfragment.com/ramblings/cantor-szudzik-pairing-functions/  
*/
namespace std {
    template<>
    struct hash<std::pair<int,int>> {
        size_t operator()(const std::pair<int,int>& p) const {
            int one = p.first;
            int two = p.second;
            int const a = (one >= 0.0 ? 2.0 * one : (-2.0 * one) - 1.0);
            int const b = (two >= 0.0 ? 2.0 * two : (-2.0 * two) - 1.0);
            return (a >= b ? (a * a) + a + b : (b * b) + a) * 0.5;
        }
    };
}

// /*! \brief 	Return a vector of pairs of (x,y) coordinates that within inside the radius of the paintbrush center.
// *           This is the brute force version of Bresenham's Circle Algorithm
// *           Source: https://stackoverflow.com/questions/1201200/fast-algorithm-for-drawing-filled-circles
// *           Author: Daniel Earwicker (2009)
// * */
// std::vector<std::pair<int, int>> Utility::MidpointCircleAlgo(int xCenter, int yCenter, int radius) {
    // std::vector<std::pair<int, int>> pixelsVector;
    // int radius_sqr = radius * radius;
    // for (int x = -radius; x < radius ; x++) {
    //     int hh = (int)std::sqrt(radius_sqr - x * x);
    //     int rx = xCenter + x;
    //     int ph = yCenter + hh;

    //     for (int y = yCenter-hh; y < ph; y++) {
    //         pixelsVector.push_back(std::make_pair(rx, y));
    //     }
    // }
// }

// std::vector<std::pair<int, int>> Utility::MidpointCircleAlgo(int xCenter, int yCenter, int radius) {
//     std::vector<std::pair<int, int>> pixelsVector;
//     int x = radius;
//     int y = 0;
//     int radiusError = 1 - x;
//     while (x >= y) {  // iterate to the circle diagonal
//         // use symmetry to draw the two horizontal lines at this Y with a special case to draw
//         // only one line at the centerY where y == 0
//         int startX = -x + xCenter;
//         int endX = x + xCenter;         
//         for (int i = startX; i <= endX; i++) {
//             pixelsVector.push_back(std::make_pair(i, y + yCenter));
//         }
//         if (y != 0) {
//             for (int i = startX; i <= endX; i++) {
//                 pixelsVector.push_back(std::make_pair(i, -y + yCenter));
//             }
//         }
//         // move Y one line
//         y++;
//         // calculate or maintain new x
//         if (radiusError<0) {
//             radiusError += 2 * y + 1;
//         } 
//         else {
//             // we're about to move x over one, this means we completed a column of X values, use
//             // symmetry to draw those complete columns as horizontal lines at the top and bottom of the circle
//             // beyond the diagonal of the main loop
//             if (x >= y)
//             {
//                 startX = -y + 1 + xCenter;
//                 endX = y - 1 + xCenter;
//                 for (int i = startX; i <= endX; i++) {
//                     pixelsVector.push_back(std::make_pair(i, x + yCenter));
//                     pixelsVector.push_back(std::make_pair(i, -x + yCenter));
//                 }
//             }
//             x--;
//             radiusError += 2 * (y - x + 1);
//         }
//     }
//     return pixelsVector;
// }

// Source: https://en.wikipedia.org/wiki/Flood_fill
// std::vector<std::pair<int, int>> Utility::FloodFill(sf::Image& image, int x, int y, sf::Color color) {
//     std::vector<std::pair<int, int>> pixelsVector;
//     std::queue<std::pair<int, int>> myQueue;
//     std::unordered_set<std::pair<int, int>> mySet;
//     myQueue.push(std::make_pair(x, y));
//     while (!myQueue.empty()) {
//         std::pair<int, int> pixel = myQueue.front();
//         myQueue.pop();
//         if (image.getPixel(pixel.first, pixel.second) != color) {
//             mySet.insert(pixel);
//             myQueue.push(std::make_pair(pixel.first + 1, pixel.second));
//             myQueue.push(std::make_pair(pixel.first - 1, pixel.second));
//             myQueue.push(std::make_pair(pixel.first, pixel.second + 1));
//             myQueue.push(std::make_pair(pixel.first, pixel.second - 1));
//         }
//     }
//     for (auto& pixel : mySet) {
//         pixelsVector.push_back(pixel);
//     }
//     return pixelsVector;
// }
//