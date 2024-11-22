#include <stdio.h>
#include "map.h"
#include "node.h"
#include "moves.h"
#include "loc.h"
#define INT_MAX 2147483647

t_position findNearestReachablePoint(t_map map) {
    t_position basePos = getBaseStationPosition(map);
    t_position nearestPoint = { .x = -1, .y = -1 };
    int minCost = INT_MAX;

    for (int i = 0; i < map.y_max; i++) {
        for (int j = 0; j < map.x_max; j++) {
            if (map.soils[i][j] != CREVASSE && map.costs[i][j] < minCost) {
                minCost = map.costs[i][j];
                nearestPoint.x = j;
                nearestPoint.y = i;
            }
        }
    }

    if (nearestPoint.x != -1 && nearestPoint.y != -1) {
        printf("Nearest reachable point to base is at: [%d, %d] with cost: %d\n", nearestPoint.x, nearestPoint.y, minCost);
    } else {
        printf("No reachable point found.\n");
    }

    return nearestPoint;
}

int main() {
    t_map map;

    // The following preprocessor directive checks if the code is being compiled on a Windows system.
    // If either _WIN32 or _WIN64 is defined, it means we are on a Windows platform.
    // On Windows, file paths use backslashes (\), hence we use the appropriate file path for Windows.
#if defined(_WIN32) || defined(_WIN64)
    map = createMapFromFile("..\\maps\\example1.map");
#else
    map = createMapFromFile("../maps/example1.map");
#endif
/*
    printf("Map created with dimensions %d x %d\n", map.y_max, map.x_max);
    for (int i = 0; i < map.y_max; i++){
        for (int j = 0; j < map.x_max; j++){
            printf("%d ", map.soils[i][j]);
        }
        printf("\n");
    }
    // printf the costs, aligned left 5 digits
    for (int i = 0; i < map.y_max; i++){
        for (int j = 0; j < map.x_max; j++){
            printf("%-5d ", map.costs[i][j]);
        }
        printf("\n");
    }
    displayMap(map);*/


    //localisation de base
    t_localisation startLoc = defineRobotPosition(map);
    displayMapRobot( map, startLoc.pos);

    // tests arbre
    int nb_moves = 9;  //test avec 4 moves differents
    int nb_choices = 5  ;  // on en prend que 3 sur les 4
    t_move* moves = chooseMove(nb_moves);
    t_node* root1 = createNode(0, nb_moves, STAY, startLoc ); //root de l'arbre avec move = "STAY"
    createSons(root1, nb_moves, nb_choices, moves, startLoc, map);
    printf("\nMouvements disponibles (%d moves et %d choix) :\n", nb_moves, nb_choices);
    for (int i=0; i<nb_moves; i++) {
        // display la liste de mouvement dispo
        printf("Movement: %s \n", t_move_to_string(moves[i]));
    }
    /*
    printf("\nArbre associe (chemin prefix) :\n");
    displayTree(root1, nb_choices); //display l'arbre
    printf("\n");
    */

    t_path best_path = {0};
    int min_cost = evaluateTree(root1, &best_path);

    if (min_cost == INT_MAX) {
        printf("No valid path to base found.\n");
        findNearestReachablePoint(map);
    } else {
        printf("\nOptimal path to base (total cost: %d):\n", min_cost);
        for (int i = 0; i < best_path.num_moves; i++) {
            t_localisation newLoc = move(startLoc, best_path.moves[i]);
            int move_cost = map.costs[newLoc.pos.y][newLoc.pos.x];
            printf("%d. %s (cost: %d)\n", i+1, t_move_to_string(best_path.moves[i]), move_cost);
            startLoc = newLoc;  // Update position for next move
        }
    }

    return 0;
}
