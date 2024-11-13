//
// Created by jerem on 21/10/2024.
//

#ifndef NODE_H
#define NODE_H



typedef struct t_node {
    int value ;
    int nb_sons;
    struct t_node * sons[9];

}t_node;

typedef struct t_tree {
    t_node * root;
}t_tree;


t_node * create_node(int value);

void createSons(int height, int moves[]);

t_tree create_tree(t_node * root);

#endif //NODE_H