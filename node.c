#include "node.h"
#include "moves.h"
#include "map.h"
#include "loc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INT_MAX 2147483647

t_node * createNode(int value, int level, int nb_sons, t_move move, t_localisation loc) {
    t_node * node = (t_node *)malloc(sizeof(t_node));
    node->value = value;
    node->nb_sons = nb_sons;
    node->loc = loc;
    node->move = move;
    
    // Initialize movements array with the current move
    for (int i = 0; i < 5; i++) {
        if (i == level) {
            node->movements[i] = move;
        } else {
            node->movements[i] = STAY;  // Use STAY as sentinel value
        }
    }
    
    // Initialize sons array
    for (int i = 0; i < 9; i++) {
        node->sons[i] = NULL;
    }
    
    return node;
}

void createSons(t_node* node, int nb_moves, int nb_choices, t_move moves[], t_localisation loc, t_map map, int level) {
    if (level >= nb_choices) {
        return;
    }
    
    for (int i = 0; i < nb_moves; i++) {
        t_localisation newLoc = move(loc, moves[i]);
        if (isValidLocalisation(newLoc.pos, map.x_max, map.y_max)) {
            t_node* son = createNode(map.costs[newLoc.pos.y][newLoc.pos.x], 
                                   level + 1, 
                                   nb_moves, 
                                   moves[i], 
                                   newLoc);
            
            // Copy parent's movement history
            for (int j = 0; j < level; j++) {
                son->movements[j] = node->movements[j];
            }
            // Add current move
            son->movements[level] = moves[i];
            
            node->sons[i] = son;
            createSons(son, nb_moves, nb_choices, moves, newLoc, map, level + 1);
        }
    }
}

void displayTree(t_node* root, int level) {
    if (root == NULL) {
        return;
    }
    
    // Print indentation
    for (int i = 0; i < level; i++) {
        printf("  ");
    }
    
    // Print node info and movement history
    // printf("Node cost: %d, Movements: ", root->value);
    // for (int i = 0; i < level; i++) {
    //     printf("%s -> ", t_move_to_string(root->movements[i]));
    // }
    // printf("\n");
    
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


t_node* minCost(t_node* root) {
    if (root == NULL) {
        return NULL;
    }
    
    // Check if current position is valid (not a crevasse and within bounds)
    if (root->value >= 10000) {
        return NULL;
    }
    
    // Start with the current node as minimum
    t_node* min_node = root;
    int min_cost = root->value;
    
    // Check all children
    for (int i = 0; i < root->nb_sons; i++) {
        t_node* son_min = minCost(root->sons[i]);
        
        if (son_min != NULL && son_min->value < min_cost) {
            min_node = son_min;
            min_cost = son_min->value;
        }
    }
    
    return min_node;
}

void displayNodePath(t_node* node) {
    printf("Path to this node: ");
    for (int i = 0; i < 5; i++) {
        if (node->movements[i] != STAY) {
            printf("%s -> ", t_move_to_string(node->movements[i]));
        }
    }
}