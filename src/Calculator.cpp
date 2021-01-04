#include <Calculator.h>

Calculator::~Calculator()
{
    Reset();
}

void Calculator::Reset()
{
    resetTree(expHead);
    expHead = nullptr;
    lastNodeAdded = nullptr;
    lastOperationNode = nullptr;
    bExpValidState = false;
    bLastOperationIsMultiply = false;
}

bool Calculator::processNewInput(inputType inType, int16_t digit)
{
    if(inType==inputType::equal) // Caller should have called evaluteExpression
        return false;

    if(!bExpValidState && inType!=inputType::digit) // Only digits allowed when not yet in a valid expression state
        return false;

    if(lastNodeAdded!=nullptr && lastNodeAdded->content==inputType::digit && inType == inputType::digit)
    {
        int16_t signedDigit = lastNodeAdded->Num<0 ? 0-digit : digit;
        lastNodeAdded->Num = lastNodeAdded->Num * 10 + signedDigit;
        return true;
    }

    expTreeNode* pNode = new expTreeNode();
    pNode->content = inType;
    pNode->Num = digit;
    
    if(expHead==nullptr)
    {
        expHead = pNode;
    }
    else
    {
        switch (inType)
        {
        case inputType::digit:
            //In this case, always add the new node to the left of the last added.
            //The last added must be an operation node.
            lastNodeAdded->pRightNode = pNode;
            pNode->pParentNode = lastNodeAdded;
            break;
        
        case inputType::multiply:
        case inputType::divide:
            if (bLastOperationIsMultiply)
                if(lastOperationNode==expHead)
                    InsertNodeAtTree(pNode, expHead);
                else
                    InsertNodeAtTree(pNode, lastOperationNode);
            else
                if (lastNodeAdded == expHead)
                    InsertNodeAtTree(pNode, expHead);
                else
                    InsertNodeAtTree(pNode, lastNodeAdded);
            lastOperationNode = pNode;
            bLastOperationIsMultiply = true;
            break;

        case inputType::add:
        case inputType::subtract:
            InsertNodeAtTree(pNode, expHead);
            lastOperationNode = pNode;
            bLastOperationIsMultiply = false;
            break;
            
        default:
            break;
        }
    }
    
    lastNodeAdded = pNode;
    bExpValidState = pNode->content == inputType::digit;

    return true;
}

int16_t Calculator::evaluateExpression()
{
    return EvaluteTree(expHead);
}

/////////// Private methods
///////////////////////////

void Calculator::resetTree(expTreeNode* node)
{
    if(node!=nullptr)
    {
        resetTree(node->pLeftNode);
        resetTree(node->pRightNode);

        delete node;
        node = nullptr;
    }
}

int16_t Calculator::EvaluteTree(expTreeNode* pNode)
{
    if(pNode==nullptr)
        return 0;

    if(pNode->content==inputType::digit)
        return pNode->Num;

    int nLeftOp = EvaluteTree(pNode->pLeftNode);
    int nRightOp = EvaluteTree(pNode->pRightNode);
    switch (pNode->content)
    {
    case inputType::multiply:
        return nLeftOp * nRightOp;

    case inputType::divide:
        return nLeftOp / nRightOp;

    case inputType::add:
        return nLeftOp + nRightOp;

    case inputType::subtract:
        return nLeftOp - nRightOp;

    default:
        return 0;
    }
}

void Calculator::InsertNodeAtTree(expTreeNode* &pNode, expTreeNode* &pTree)
{
    if (pTree == nullptr || pNode == nullptr)  /// very strange
        return;

    if(pTree->pParentNode!=nullptr)
    {
        if(pTree->pParentNode->pLeftNode==pTree)
            pTree->pParentNode->pLeftNode=pNode;
        else
            pTree->pParentNode->pRightNode=pNode;
    }

    pNode->pParentNode = pTree->pParentNode;
    pNode->pLeftNode = pTree;
    pTree->pParentNode = pNode;
    pTree = pNode;
}
