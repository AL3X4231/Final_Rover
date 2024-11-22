#include "node.h"
#include "moves.h"
#include "map.h"
#include "loc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INT_MAX 2147483647

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
    // Early exit conditions
    if (nb_choices == 0) {
        return;
    }
    
    // If we reached the base, stop exploring
    if (loc.pos.x == 2 && loc.pos.y == 1) {
        return;
    }
    
    // Calculate Manhattan distance to goal
    int dist_to_goal = abs(loc.pos.x - 2) + abs(loc.pos.y - 1);
    
    // If we can't reach the goal with remaining moves, stop exploring
    if (dist_to_goal > nb_choices) {
        return;
    }
    
    for (int i = 0; i < nb_moves; i++) {
        t_localisation newLoc = move(loc, moves[i]);
        
        // Skip if move is invalid
        if (!isValidPosition(newLoc, map.x_max, map.y_max)) {
            continue;
        }
        
        // Cost is always based on the terrain at the new location
        int cost = map.costs[newLoc.pos.y][newLoc.pos.x];
        if (cost >= 10000) {
            continue;  // Skip invalid terrain
        }
        
        // Create the node
        node->sons[i] = createNode(cost, nb_moves-1, moves[i], newLoc);
        
        // Create next level moves
        t_move new_tab[nb_moves-1];
        int index = 0;
        for (int j = 0; j < nb_moves; j++) {
            if (j != i) {
                new_tab[index++] = moves[j];
            }
        }
        
        createSons(node->sons[i], nb_moves-1, nb_choices-1, new_tab, newLoc, map);
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

int evaluateTree(t_node* node, t_path* best_path) {
    if (node == NULL) {
        return INT_MAX;
    }
    
    // If we reached the base, return 0 (don't count the base's cost)
    if (node->loc.pos.x == 2 && node->loc.pos.y == 1) {
        return 0;  // Changed from node->value
    }
    
    // If leaf node and not at base, invalid path
    if (node->nb_sons == 0) {
        return INT_MAX;
    }
    
    int min_cost = INT_MAX;
    t_path current_path = {0};
    
    // Find best path through children
    for (int i = 0; i < node->nb_sons; i++) {
        if (node->sons[i] != NULL) {
            t_path temp_path = {0};
            int child_cost = evaluateTree(node->sons[i], &temp_path);
            
            if (child_cost != INT_MAX) {
                // Include the cost of the current move
                int total_cost = node->sons[i]->value + child_cost;
                if (total_cost < min_cost) {
                    min_cost = total_cost;
                    current_path.moves[0] = node->sons[i]->move;
                    memcpy(&current_path.moves[1], temp_path.moves, temp_path.num_moves * sizeof(t_move));
                    current_path.num_moves = temp_path.num_moves + 1;
                }
            }
        }
    }
    
    if (min_cost != INT_MAX && best_path != NULL) {
        *best_path = current_path;
    }
    
    return min_cost;
}
