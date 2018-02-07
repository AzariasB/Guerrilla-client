
#include "tree.hpp"
#include <float.h>

Tree::Tree()
{

}

void Tree::generate(std::size_t depth, const BattleField &field)
{
    field.possibleTurns();

        mTreeDepth = depth;
}

const Turn &Tree::getBestAction() const
{

    float bestNodeVal = -99999.f;
    Turn *bestTurn = 0;

    for(const auto &node : mRoot->childs){
        float mmax = minmax(node, mTreeDepth, true);
        if(mmax > bestNodeVal){
            bestNodeVal = mmax;
            bestTurn = &node->action;
        }
    }

    return *bestTurn;
}


float Tree::minmax(const std::unique_ptr<Node> &parent, int depth, bool maximizePlayer) const
{
    if(parent->childs.size() == 0 || depth == 0)return parent->action.getWeight();

    if(maximizePlayer){
        float bestVal = -9999.f;
        for(const auto &ptr : parent->childs){
            float v = minmax(ptr, depth-1, false);
            bestVal = qMin(bestVal, v);
        }
        return bestVal;
    }

    //minimize then
    float bestVal = FLT_MAX;
    for(const auto &ptr : parent->childs){
        float v = minmax(ptr, depth-1, true);
        bestVal = qMin(v, bestVal);
    }
    return bestVal;
}
