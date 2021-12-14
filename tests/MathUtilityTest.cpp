#include <vector>
#include <iostream>

#include "catch_amalgamated.hpp"
#include "MathUtility.hpp"

/*! \brief Test Bresenham Circle Algorithm for filled in circles.
*/
TEST_CASE("Bresenham Circle Algorithm", "[utilities]") {
    std::vector<std::pair<int,int>> vec;
    vec.push_back(std::make_pair(-1,-1));
    vec.push_back(std::make_pair(-1,0));
    vec.push_back(std::make_pair(-1,1));
    vec.push_back(std::make_pair(0,-1));
    vec.push_back(std::make_pair(0,0));
    vec.push_back(std::make_pair(0,1));
    vec.push_back(std::make_pair(1,-1));
    vec.push_back(std::make_pair(1,0));
    vec.push_back(std::make_pair(1,1));

    std::vector<std::pair<int,int>> output = MathUtility::BresenhamCircleAlgo(1);
    for (int i = 0; i < (int)output.size(); i++) {
        // std::cout<< output[i].first << " " << output[i].second << std::endl;
        REQUIRE(output[i].first == vec[i].first);
        REQUIRE(output[i].second == vec[i].second);
    }
}

/*! \brief Test EFLA for interpolating all points between two points.
*/
TEST_CASE("Extremely Fast Line Algorithm", "[utilities]") {
    std::vector<std::pair<int,int>> vec1;
    vec1.push_back(std::make_pair(1,1));
    vec1.push_back(std::make_pair(2,2));
    vec1.push_back(std::make_pair(3,3));
    vec1.push_back(std::make_pair(4,4));
    vec1.push_back(std::make_pair(5,5));

    std::vector<std::pair<int,int>> vec2;
    vec2.push_back(std::make_pair(-3,-4));
    vec2.push_back(std::make_pair(-2,-3));
    vec2.push_back(std::make_pair(-1,-2));
    vec2.push_back(std::make_pair(0,-2));
    vec2.push_back(std::make_pair(1,-1));
    vec2.push_back(std::make_pair(2,0));
    vec2.push_back(std::make_pair(3,1));
    vec2.push_back(std::make_pair(4,1));
    vec2.push_back(std::make_pair(5,2));

    std::vector<std::pair<int,int>> vec3;
    vec3.push_back(std::make_pair(3,-4));
    vec3.push_back(std::make_pair(2,-3));
    vec3.push_back(std::make_pair(1,-2));
    vec3.push_back(std::make_pair(0,-2));
    vec3.push_back(std::make_pair(-1,-1));
    vec3.push_back(std::make_pair(-2,0));
    vec3.push_back(std::make_pair(-3,1));
    vec3.push_back(std::make_pair(-4,1));
    vec3.push_back(std::make_pair(-5,2));

    // Test a straight vertical line
    std::vector<std::pair<int,int>> output1 = MathUtility::ExtremelyFastLineAlgo(1,1,5,5);
    for (int i = 0; i < (int)output1.size(); i++) {
        REQUIRE(output1[i].first == vec1[i].first);
        REQUIRE(output1[i].second == vec1[i].second);
    }
    // Test a positive sloped line
    std::vector<std::pair<int,int>> output2 = MathUtility::ExtremelyFastLineAlgo(-3,-4,5,2);
    for (int i = 0; i < (int)output2.size(); i++) {
        REQUIRE(output2[i].first == vec2[i].first);
        REQUIRE(output2[i].second == vec2[i].second);
    }
    // Test a negative sloped line
    std::vector<std::pair<int,int>> output3 = MathUtility::ExtremelyFastLineAlgo(3,-4,-5,2);
    for (int i = 0; i < (int)output3.size(); i++) {
        REQUIRE(output3[i].first == vec3[i].first);
        REQUIRE(output3[i].second == vec3[i].second);
    }
}