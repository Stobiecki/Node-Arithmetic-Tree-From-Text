# Node-Arithmetic-Tree-From-Text
Quick text-to-value conversion via node tree, RPN and modified Shunting yard algorithm in C++.

This project allows the user to calculate full-on expressions using Expression<double> class to determine the expression's value during the runtime. The project was written in such a way that the creation of the tree itself needs a bit of an overhead so that it can be calculated very fast, multiple times in quick succession.

# Capabilities
The trees are built in such a way that they are perfect for, e.g., a set of variables that can be twekable before each re-calculation of a tree.

Currently, there are a set of few predefined (yet fully expandable) behaviors in the project:
- values
- variables
- prefix operators
- infix operators
- sufix operators
- brackets (here called sections)
- functions (with a custom amount of arguments)

# Example code:

```c++
int main()
{
    // static data initialization
    StaticData::DataInit();

    // loading input
    std::string input = "2+-3*pow(2,3.1)";

    // initialization of an expression tree
    Expression<double> expr(input);
    expr.PrepareData();

    // tree calculation
    double result = expr.Calculate();

    // result display
    std::cout << "\nResult: " << result;
}
```

# Plans for the future?
- This project is only a part of the bigger project, in which these calculations are essential.
- Validation of an expression. Currently not supported, so using expressions that are not parsed will yield runtime errors.
- In the future, it is planned to add semi-reflection functions (currently unavailable) during runtime to create custom behavior by the user.
- Exchangable nodes post-initialization of a tree.

Nodes planned to be implemented in the future:
- arrays
- matrices

# Bugs?
Most of the basic behaviors are (so far) without any major bugs. If you will find any, please let me know. :-)


