#include "node.h"
#include "moves.h"
#include "map.h"
#include "loc.h"
#include <stdio.h>
#include <stdlib.h>

t_node * createNode(int value, int nb_sons, t_move move, t_localisation loc) {
    t_node * node = (t_node *)malloc(sizeof(t_node));
    node->value = value;
    node->nb_sons = nb_sons;
    node->move = move;
    node->loc = loc;
    for (int i=0; i < nb_sons; i++) {
        node->sons[i]= NULL;
    }
    return node;
}

void createSons(t_node* node, int nb_moves, int nb_choices, t_move moves[], t_localisation loc, t_map map) {
    if (nb_choices == 0) {
        return;
    } else {
        for (int i=0; i < nb_moves; i++) {
            // value doit etre remplacée par le cost
            int cost;
            t_localisation newLoc = move(loc, moves[i]); //focntion de newloc

            //test si le robot est toujours sur la carte
            //printf("\nx_max at y max : %d %d", map.x_max, map.y_max);
            //printf("\nposition du bot : %d %d", newLoc.pos.x, newLoc.pos.y);
            if(isValidPosition(newLoc, map.x_max,  map.y_max)){
                //printf("valid");
                cost = map.costs[newLoc.pos.x][newLoc.pos.y];
            } else {
                //printf("\nThe robot jumps  of the map !?!\n");
                cost = 10000;
            }
            node->sons[i] = createNode(cost,nb_moves-1, moves[i], newLoc); ///rajouter soil

            t_move new_tab[nb_moves-1];
            // Construire un tableau sans le mouvement actuel
            int index = 0;
            for (int j = 0; j < nb_moves; j++) {
                if (j != i) {
                    new_tab[index++] = moves[j];
                }
            }
            createSons(node->sons[i],nb_moves-1,nb_choices-1, new_tab, newLoc, map);
        }
    }
}

void displayTree(t_node* root, int level) {
    if (root == NULL) {
        return;
    }
    // print la root
    printf("Node cost: %d, Number of sons: %d , Movement: %s \n", root->value, root->nb_sons, t_move_to_string(root->move));

    for (int i = 0; i < level; i++) {
        printf("  ");  // Indentation pour chaque niveau dans l'arbre
    }

    for (int i = 0; i < root->nb_sons; i++) {
        displayTree(root->sons[i], level + 1);
    }
}

int evaluateTree(t_node* node) {
    if (node == NULL) {
        return INT_MAX; // No valid cost for NULL nodes
    }

    if (node->nb_sons == 0) {
        // Leaf node: return its cost
        //printf("Leaf Node - Cost: %d\n", node->value);
        return node->value;
    }

    // Initialize minimum cost to a very high value
    int min_cost = INT_MAX;

    // Iterate over all children to find the minimum cost
    for (int i = 0; i < node->nb_sons; i++) {
        if (node->sons[i] != NULL) {
            int child_cost = evaluateTree(node->sons[i]);
            //printf("Child Node - Current Cost: %d, Child Cost: %d, Min Cost: %d\n",node->value, child_cost, min_cost);
            if (child_cost < min_cost) {
                min_cost = child_cost;
            }
        }
    }

    // If no valid children, return current node's cost (for edge cases)
    if (min_cost == INT_MAX) {
        return node->value;
    }

    // Return the current node's cost plus the minimum cost of its children
    //printf("Node - Cost: %d, Min Child Cost: %d, Total Cost: %d\n", node->value, min_cost, node->value + min_cost);
    return node->value + min_cost;
}


