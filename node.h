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


t_node * createNode(int value,int nb_sons);

void createSons(t_node* node, int height, int moves[]);
void displayTree(t_node* node, int level);
t_tree create_tree(t_node * root);

#endif //NODE_H