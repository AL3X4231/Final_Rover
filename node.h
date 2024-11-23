//
// Created by jerem on 21/10/2024.
//

#ifndef NODE_H
#define NODE_H
#include "moves.h"
#include "map.h"

/**
 * @brief Structure for the path in each node
 *        it allows to save the movements done before
 *        to reach the position of the rover in each cases
 */
typedef struct {
    t_move moves[10];
    int num_moves;
} t_path;

/**
 * @brief Structure of nodes in the tree
 *        value (cost);
 *        nb_sons (the numbers of sons the node has);
 *        movements[5] (tab to register the path, the movements done);
 *        loc (current localisation);
 *        move (movement we've just done);
 *        sons[9] (tab of pointers to reach parent's sons nodes)
 */
typedef struct t_node {
    int value ;
    int nb_sons;
    t_move movements[5];
    t_localisation loc;
    t_move move;
    struct t_node * sons[9];

}t_node;

typedef void (*NodeCallback)(t_node* node);

t_node * createNode(int value,int level,int nb_sons, t_move move, t_localisation loc);
/**
* @brief function to create a node and fill its attributes
* @param value : cost of the node
* @param level : the heigth at which the node appears in the tree
* @param nb_sons : the number of sons the node has
* @param move : the movement tested and attributed to the node
* @param loc : the localisation of the rover just after he did the move
* @return a pointer leading to the node created
*/

void createSons(t_node* node, int nb_moves, int nb_choices,t_move moves[],t_localisation loc, t_map map, int level);
/**
* @brief  recursive to create all the sons of a node
* @brief all sons are also nodes structure
* @param node : pointer to the node "parent of sons"
* @param nb_moves : number of different moves we have at the begining
*                   (it decreases at each heigth in the tree)
* @param nb_choices :  number of moves allowed
*                   (for example we have a list of 9 different movements
*                   but we can pick at most 5)
*                   (nb_choices define the number of sons we will have)
* @param moves[] : tab of remaining movements we will use for the sons
* @param loc : the localisation of the rover before using the sons moves
* @param map : the map to compute next localisations and cost
* @param level : the level at which we are in the tree
* the function returns nothing
*/

void displayTree(t_node* node, int level);
/**
* @brief recursive function to display the tree
*       (it browses the nodes in the tree with the prefix method)
* @param node : the node is the root of the tree
* @param level : the heigth of the tree
* @return : the function returns nothing
*/

int evaluateTree(t_node* node, t_path* best_path);
/**
* @brief function browses the tree and find the smaller cost
* @param node : the node is the root of the tree
* @param level : the heigth of the tree
* the function returns nothing
*/

t_node* minCost(t_node* root);
/**
* @brief function browses the entire tree and find the
*        node with the smaller cost so that we can retrieve
*        the path with the best movements
* @param root : the root of the tree
* @return : this function returns the node with the smaller cost
*/

void displayNodePath(t_node* node);
/**
* @brief function simply display the path of the node
*        (in other words, it displays each movement (in node->movements[])
*        the rover have done to reach the current localisation)
*        (it allows to retrieve the best movements to do when we found
*        the node with the lowest cost)
* @param node : the root of the tree
* the function returns nothing
*/

#endif //NODE_H