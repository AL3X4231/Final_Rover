//
// Created by flasque on 19/10/2024.
//

#include "moves.h"
#include "stdio.h"
#include <time.h>
#include <stdlib.h>



/* prototypes of local functions */
/* local functions are used only in this file, as helper functions */

/**
 * @brief Function to rotate the robot according to a move and its actual orientation
 * @param ori : the actual orientation of the robot
 * @param move : the move to do
 * @return the new orientation of the robot
 */
t_orientation rotate(t_orientation, t_move );

/**
 * @brief function to translate the robot according to a move and its actual position
 * @param loc : the actual localisation of the robot
 * @param move : the move to do
 * @return the new localisation of the robot
 */
t_localisation translate(t_localisation , t_move);

/* definition of local functions */

t_orientation rotate(t_orientation ori, t_move move)
{
    int rst;
    switch (move)
    {
        case T_LEFT:
            rst=3;
            break;
        case T_RIGHT:
            rst=1;
            break;
        case U_TURN:
            rst=2;
            break;
        default:
            break;
    }
    return (ori+rst)%4;
}

t_localisation translate(t_localisation loc, t_move move)
{
    /** rules for coordinates:
     *  - x grows to the right with step of +1
     *  - y grows to the bottom with step of +1
     *  - the origin (x=0, y=0) is at the top left corner
     */
    t_position res = loc.pos;
    switch (move) {
        case F_10:
            switch (loc.ori) {
                case NORTH:
                    res.y = loc.pos.y - 1;
                    break;
                case EAST:
                    res.x = loc.pos.x + 1;
                    break;
                case SOUTH:
                    res.y = loc.pos.y + 1;
                    break;
                case WEST:
                    res.x = loc.pos.x - 1;
                    break;
                default:
                    break;
            }
            break;
        case F_20:
            switch (loc.ori) {
                case NORTH:
                    res.y = loc.pos.y - 2;
                    break;
                case EAST:
                    res.x = loc.pos.x + 2;
                    break;
                case SOUTH:
                    res.y = loc.pos.y + 2;
                    break;
                case WEST:
                    res.x = loc.pos.x - 2;
                    break;
                default:
                    break;
            }
            break;
        case F_30:
            switch (loc.ori) {
                case NORTH:
                    res.y = loc.pos.y - 3;
                    break;
                case EAST:
                    res.x = loc.pos.x + 3;
                    break;
                case SOUTH:
                    res.y = loc.pos.y + 3;
                    break;
                case WEST:
                    res.x = loc.pos.x - 3;
                    break;
                default:
                    break;
            }
            break;
        case B_10:
            switch (loc.ori) {
                case NORTH:
                    res.y = loc.pos.y + 1;
                    break;
                case EAST:
                    res.x = loc.pos.x - 1;
                    break;
                case SOUTH:
                    res.y = loc.pos.y - 1;
                    break;
                case WEST:
                    res.x = loc.pos.x + 1;
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
        return loc_init(res.x, res.y, loc.ori);

}

/* definitions of exported functions */

char *getMoveAsString(t_move move)
{
    return _moves[move];
}

t_localisation move(t_localisation loc, t_move move)
{
    t_localisation new_loc;
    new_loc.ori = rotate(loc.ori, move);
    new_loc = translate(loc, move);
    return new_loc;
}

void updateLocalisation(t_localisation *p_loc, t_move m)
{
    *p_loc = move(*p_loc, m);
    return;
}

t_move* chooseMove(int nbmove) {
    srand(time(NULL));

    t_move* list = malloc(nbmove * sizeof(t_move));
    if (list == NULL) {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    int r;

    // Threshold values for each move type
    int max_F10 = 21;
    int max_F20 = 36;
    int max_F30 = 43;
    int max_B10 = 50;
    int max_RIGHT = 71;
    int max_LEFT = 92;
    int max_UTurn = 99;

    for (int i = 0; i < nbmove; i++) {
        r = rand() % (max_UTurn+1); // Generate a random number between 1 and max_UTurn

        if (r <= max_F10) {
            list[i] = F_10;
            max_F10--;
            max_F20--;
            max_F30--;
            max_B10--;
            max_RIGHT--;
            max_LEFT--;
            max_UTurn--;

        } else if (r <= max_F20) {
            list[i] = F_20;
            max_F20--;
            max_F30--;
            max_B10--;
            max_RIGHT--;
            max_LEFT--;
            max_UTurn--;
        } else if (r <= max_F30) {
            list[i] = F_30;
            max_F30--;
            max_B10--;
            max_RIGHT--;
            max_LEFT--;
            max_UTurn--;
        } else if (r <= max_B10) {
            list[i] = B_10;
            max_B10--;
            max_RIGHT--;
            max_LEFT--;
            max_UTurn--;
        } else if (r <= max_RIGHT) {
            list[i] = T_RIGHT;
            max_RIGHT--;
            max_LEFT--;
            max_UTurn--;
        } else if (r <= max_LEFT) {
            list[i] = T_LEFT;
            max_LEFT--;
            max_UTurn--;
        } else {
            list[i] = U_TURN;
            max_UTurn--;
        }
    }

    return list;

}



