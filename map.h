//
// Created by flasque on 19/10/2024.
//

#ifndef UNTITLED1_MAP_H
#define UNTITLED1_MAP_H
#define COST_UNDEF 65535

#include "loc.h"
/**
 * @brief Enum for the possible soils of the map
 */
typedef enum e_soil
{
    BASE_STATION,
    PLAIN,
    ERG,
    REG,
    CREVASSE
} t_soil;

/**
 * @brief Array of costs for the soils
 */
static const int _soil_cost[5] = {0, 1, 2, 4, 10000};

/**
 * @brief Structure for the map

 */
typedef struct s_map
{
    t_soil  **soils;
    int     **costs;
    int     x_max;
    int     y_max;
} t_map;

/**
 * @brief Function to initialise the map from a file
 * @param filename : the name of the file
 * @return the map
 */
t_map createMapFromFile(char *);

/**
 * @brief Function to get the position of the base station
 *        (it browse the map to find a soil = "B")
 * @param map : the current map
 * @return the position of the base station in the map
 */
t_position getBaseStationPosition(t_map map);

/**
 * @brief Function to randomly define a localisation for our rover
 *        (when the rover landed on the planet :
 *        we need its position ('pos' structure)
 *        and its orientation ('ori' structure))
 * @param map : the current map
 * @return the localisation of the rover after launched on the map
 */
t_localisation defineRobotPosition(t_map map);

/**
 * @brief display the map with characters
 * @param map : the map to display
 */
void displayMap(t_map);

/**
 * @brief display the map with the rover on it with characters
 * @param map : the map to display
 */
void displayMapRobot(t_map , t_position );

#endif //UNTITLED1_MAP_H
