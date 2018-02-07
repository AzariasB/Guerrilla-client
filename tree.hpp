#ifndef TREE_HPP
#define TREE_HPP

#include "battlefield.hpp"
#include "action.hpp"
#include <memory>
#include <vector>

class Tree
{
public:


    Tree();

    void generate(std::size_t depth, const BattleField &field);

    const Turn &getBestAction() const;

private:
    struct Node;

    using child_nodes = std::vector<std::unique_ptr<Node>>;

    struct Node{
        Turn action;
        child_nodes childs;
    };

    const std::unique_ptr<Node> &getBestNode(const std::unique_ptr<Node> &parent) const;

    const std::unique_ptr<Node> &getWorstNode(const std::unique_ptr<Node> &parent) const;

    float minmax(const std::unique_ptr<Node> &parent, int depth, bool maximizePlayer) const;

    std::unique_ptr<Node> mRoot;

    std::size_t mTreeDepth = 0;

};

#endif // TREE_HPP
