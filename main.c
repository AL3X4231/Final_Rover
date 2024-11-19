#include <stdio.h>
#include "map.h"
#include "node.h"
#include "moves.h"

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
    displayMap(map);
    */

    // tests arbre
    int nb_moves = 4;  //test avec 4 moves differents
    int nb_choices = 3  ;  // on en prend que 3 sur les 4
    t_move* moves = chooseMove(nb_moves);
    t_node* root1 = createNode(0, nb_moves, STAY); //root de l'arbre avec move = "STAY"
    createSons(root1, nb_moves,nb_choices, moves);
    printf("\nMouvements disponibles (%d moves et %d choix) :\n", nb_moves, nb_choices);
    for (int i=0; i<nb_moves; i++) {
        // display la liste de mouvement dispo
        printf("Movement: %s \n", t_move_to_string(moves[i]));
    }
    printf("Arbre associe (chemin prefix) :\n");
    displayTree(root1, nb_choices); //display l'arbre
    printf("\n");

    return 0;
}
