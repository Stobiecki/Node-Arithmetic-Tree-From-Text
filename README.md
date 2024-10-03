# Node-Arithmetic-Tree-From-Text
Quick text-to-value conversion via node tree, RPN and modified Shunting yard algorithm in C++

Example code:

```c++
int main()
{
    // static data initialization
    StaticData::DataInit();

    // loading input
    std::string input;
    std::cout << "Input expression: ";
    std::getline(std::cin, input);

    // initialization of an expression tree
    Expression<double> expr(input);
    expr.PrepareData();

    // tree calculation
    double result = expr.Calculate();

    // result display
    std::cout << "\nResult: " << result;
}
```
