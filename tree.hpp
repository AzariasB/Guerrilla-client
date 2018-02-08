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

    struct Node{

        Node(){}

        Node(const Turn &t):action(t){}

        Turn action;
        std::vector<std::unique_ptr<Node>> childs;
    };

    void genTreeNode(std::unique_ptr<Node> &parent, const BattleField &field, std::size_t depth, int multi);

    const std::unique_ptr<Node> &getBestNode(const std::unique_ptr<Node> &parent) const;

    const std::unique_ptr<Node> &getWorstNode(const std::unique_ptr<Node> &parent) const;

    float minmax(const std::unique_ptr<Node> &parent, int depth, bool maximizePlayer) const;

    std::unique_ptr<Node> mRoot;

    std::size_t mTreeDepth = 0;

};

#endif // TREE_HPP
