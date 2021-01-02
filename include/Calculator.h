#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <commonDefs.h>

struct expTreeNode 
{
    inputType content;
    int16_t Num;
    expTreeNode* pLeftNode = nullptr;
    expTreeNode* pRightNode = nullptr;
};

class Calculator
{
private:
    expTreeNode* expHead = nullptr;
    expTreeNode* lastNodeAdded = nullptr;
    expTreeNode* lastAddedParent = nullptr;

    bool bExpValidState = false;

    void resetTree(expTreeNode* node);
    int16_t EvaluteTree(expTreeNode* node);

public:
    Calculator() {}
    ~Calculator();

    void Reset();

    bool processNewInput(inputType inType, int16_t digit=0);
    int16_t evaluateExpression();
};

#endif