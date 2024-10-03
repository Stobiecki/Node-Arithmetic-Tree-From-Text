#pragma once

template<class T>// value can be int, double, etc. 
class Node
{
public:
	T value; // Cached value
	bool isInvalid = true; // Indicator that tells us if we have to recalculate node

    virtual T Calculate()
    {
        //if (!isInvalid)
        //    return value;  // Je�li w�ze� nie jest "brudny", zwracamy zbuforowany wynik

        //isInvalid = false;
        return value;
    }
	virtual ~Node() {};
    virtual void markInvalid() { isInvalid = true; }
	Node() = default;
};