
#include "tree.hpp"
#include <float.h>

Tree::Tree()
{

}

void Tree::generate(std::size_t depth, const BattleField &field)
{
    mTreeDepth = depth;//save the depth for minmax
    mRoot = std::make_unique<Node>();
    genTreeNode(mRoot, field, depth, 1);
}

void Tree::genTreeNode(std::unique_ptr<Node> &parent, const BattleField &field, std::size_t depth, int coef)
{
    if(depth == 0)return;

    std::vector<Turn> turns = field.possibleTurns();
    for(auto &turn : turns){
        if(turn.getAction(0))turn.getAction(0)->setWeight(field.actionWeight(*turn.getAction(0))* coef);
        if(turn.getAction(1))turn.getAction(1)->setWeight(field.actionWeight(*turn.getAction(1))* coef );

        std::unique_ptr<Node> nwNode = std::make_unique<Node>(turn);
        BattleField copy = field;//copy field to simulate turn
        copy.setId(1-copy.getId());//switch field id
        turn.applyActions(copy);

        genTreeNode(nwNode, copy, depth-1, -coef);
        parent->childs.emplace_back(std::move(nwNode));
    }


}

const Turn &Tree::getBestAction() const
{
    float bestNodeVal = -INFINITY;
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
        float bestVal = -9999999.f;
        for(const auto &ptr : parent->childs){
            float v = minmax(ptr, depth-1, false);
            bestVal = qMax(bestVal, v);
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
