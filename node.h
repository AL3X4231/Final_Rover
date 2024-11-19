//
// Created by jerem on 21/10/2024.
//

#ifndef NODE_H
#define NODE_H
#include "moves.h"
#include "map.h"


typedef struct t_node {
    int value ;
    int nb_sons;
    t_soil soil;
    t_move move;
    struct t_node * sons[9];

}t_node;


t_node * createNode(int value,int nb_sons, t_move move);

void createSons(t_node* node, int height, int nb_choices,t_move moves[]);
void displayTree(t_node* node, int level);


#endif //NODE_H