#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <commonDefs.h>

struct expTreeNode 
{
    inputType content;
    int16_t Num;
    expTreeNode* pParentNode = nullptr;
    expTreeNode* pLeftNode = nullptr;
    expTreeNode* pRightNode = nullptr;
};

class Calculator
{
private:
    expTreeNode* expHead = nullptr;
    expTreeNode* lastNodeAdded = nullptr;
    expTreeNode* lastOperationNode = nullptr;

    bool bExpValidState = false;
    bool bLastOperationIsMultiply = false;

    void resetTree(expTreeNode* node);
    int16_t EvaluteTree(expTreeNode* node);
    void InsertNodeAtTree(expTreeNode* &pNode, expTreeNode* &pTree);

public:
    Calculator() {}
    ~Calculator();

    void Reset();

    bool processNewInput(inputType inType, int16_t digit=0);
    int16_t evaluateExpression();
};

#endif