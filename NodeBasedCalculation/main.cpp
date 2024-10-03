#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <unordered_map>
#include <tuple>
#include <chrono>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <variant>

#include "StaticData.h"
#include "Node.h"
#include "FunctionNode.h"
#include "PrefixOperationNode.h"
#include "InfixOperationNode.h"
#include "PostfixOperationNode.h"
#include "ValueNode.h"
#include "Expression.h"

int main() 
{
    StaticData::DataInit();

    std::string input;
    std::cout << "Input expression: ";
    std::getline(std::cin, input);

    //int iter = 10'000'000;
    int iter = 100'000;

    auto start = std::chrono::high_resolution_clock::now();
    auto startTreeBuilding = std::chrono::high_resolution_clock::now();

    Expression<double> expr(input);
    expr.PrepareData();

    auto endTreeBuilding = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < iter - 2; i++)
        expr.Calculate();
    double result = expr.Calculate();

    auto end = std::chrono::high_resolution_clock::now();
    auto treeBuildingDuration = std::chrono::duration_cast<std::chrono::nanoseconds>(endTreeBuilding - startTreeBuilding).count();
    double treeBuildingDuration_ms = treeBuildingDuration / 1'000.0;
    double treeBuildingDuration_sec = treeBuildingDuration / 1'000'000'000.0;

    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    double duration_ns = duration;
    double duration_ms = duration / 1'000.0;
    double duration_mils = duration / 1'000'000.0;
    double duration_sec = duration / 1'000'000'000.0;

    expr.LogDataToConsole();

    std::cout << "\n/////////////////////////////////////////////////////////////////";
    std::cout << "\n\nExpression :                " << input << std::endl;
    std::cout << "\nTime of generating node tree: " << std::fixed << std::setprecision(10) << std::setw(20) << std::setfill(' ') << treeBuildingDuration_ms << " microseconds" << std::endl;
    std::cout << "Time of generating node tree: " << std::fixed << std::setprecision(10) << std::setw(20) << std::setfill(' ') << treeBuildingDuration_sec << " secunds" << std::endl;
    std::cout << "\nNumber of iterations:         " << std::fixed << std::setprecision(10) << std::setw(20) << std::setfill(' ') << iter << std::endl;
    std::cout << "Time of tree evaluation:      " << std::fixed << std::setprecision(10) << std::setw(20) << std::setfill(' ') << duration_ns << " nanoseconds" << std::endl;
    std::cout << "Time of tree evaluation:      " << std::fixed << std::setprecision(10) << std::setw(20) << std::setfill(' ') << duration_ms << " microseconds" << std::endl;
    std::cout << "Time of tree evaluation:      " << std::fixed << std::setprecision(10) << std::setw(20) << std::setfill(' ') << duration_mils << " milliseconds" << std::endl;
    std::cout << "Time of tree evaluation:      " << std::fixed << std::setprecision(10) << std::setw(20) << std::setfill(' ') << duration_sec << " secunds" << std::endl;
    std::cout << "\nResult value:                 " << std::fixed << std::setprecision(12) << std::setw(20) << std::setfill(' ') << result << std::endl;

    return 0;
}
