#ifndef TREE_HPP
#define TREE_HPP

#include "battlefield.hpp"
#include "action.hpp"
#include <memory>
#include <vector>

/**
 * @brief The Tree class
 * used to calculate the best action to
 * perform, using minmax
 */
class Tree
{
public:

    /**
     * @brief Tree constructor
     */
    Tree();

    /**
     * @brief generate generates the tree nodes with the given depth,
     * using the given battlefield as a starting state, this process
     * can take a veeeeeery long time.
     * @param depth
     * @param field
     */
    void generate(std::size_t depth, const BattleField &field);

    /**
     * @brief getBestAction calculates the best action
     * that can be performed using the tree generated
     * previously
     * @return
     */
    const Turn &getBestAction() const;

private:
    /**
     * @brief The Node struct
     * internal structure used to represent
     * a node.
     * A node contains a Turn and a list
     * of all the sub-nodes (or none if this node
     * is a leaf)
     */
    struct Node{

        Node(){}

        /**
         * @brief Node constructor
         * @param t
         */
        Node(const Turn &t):action(t){}

        Turn action;


        std::vector<std::unique_ptr<Node>> childs;
    };

    /**
     * @brief genTreeNode generate a node of the tree (recursive function)
     * Calculates all the possible move of the given field and creates all the
     * corresponding nodes
     * @param parent the parent node
     * @param field the current state of the field
     * @param depth the actual depth of the tree, the function stops whenever the depth = 0
     * @param coef the multiplication coef for the weight of each actions
     */
    void genTreeNode(std::unique_ptr<Node> &parent, const BattleField &field, std::size_t depth, int coef);

    /**
     * @brief minmax performs the minmax for the given node, and return the highest possible
     * score
     * @param parent
     * @param depth
     * @param maximizePlayer
     * @return the minmax score of the given node
     */
    float minmax(const std::unique_ptr<Node> &parent, int depth, bool maximizePlayer) const;

    /**
     * @brief mRoot
     * the root of the tree
     */
    std::unique_ptr<Node> mRoot;

    /**
     * @brief mTreeDepth depth of the tree
     */
    std::size_t mTreeDepth = 0;

};

#endif // TREE_HPP
