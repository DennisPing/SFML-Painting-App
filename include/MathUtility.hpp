/** 
 *  @file   MathUtility.hpp 
 *  @brief  Math utility interface
 *  @author Dennis Ping 
 *  @date   2021-10-23
 ***********************************************/

#ifndef MATHUTILITY_H
#define MATHUTILITY_H

#include <vector>

// A utility class for drawing lines and shapes.
class MathUtility {

    public:
        // Extremely Fast Line Algorithm (EFLA)
        static std::vector<std::pair<int, int>> ExtremelyFastLineAlgo(int x1, int y1, int x2, int y2);

        // Bresenham's Circle Algorithm
        static std::vector<std::pair<int, int>> BresenhamCircleAlgo(int radius);

        // Flood Fill Algorithm
        // std::vector<std::pair<int, int>> Utility::FloodFill(sf::Image& image, int x, int y, sf::Color newColor);
};

#endif