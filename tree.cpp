#include "tree.hpp"

Tree::Tree()
{

}

void Tree::generate(std::size_t depth, const BattleField &field)
{

}

Turn Tree::getBestAction() const
{
    //get best element of root

    return Turn();
}

const std::unique_ptr<Tree::Node> &Tree::getBestNode(const std::unique_ptr<Tree::Node> &parent) const
{
    if(parent->childs.size() == 0)return parent;

    const std::unique_ptr<Tree::Node> &best = *std::max_element(parent->childs.cbegin(), parent->childs.cend(), [](const auto&first, const auto &second){
        return first->action.getWeight() < second->action.getWeight();
    });

    return getWorstNode(best);
}

const std::unique_ptr<Tree::Node> &Tree::getWorstNode(const std::unique_ptr<Tree::Node> &parent) const
{
    if(parent->childs.size() == 0) return parent;

    const std::unique_ptr<Tree::Node> &worst = *std::min_element(parent->childs.cbegin(), parent->childs.cend(), [](const auto&first, const auto&second){
        return first->action.getWeight() < second->action.getWeight();
    });

    return getBestNode(worst);
}
