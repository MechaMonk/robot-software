#ifndef MAP_H
#define MAP_H

#include <aversive/obstacle_avoidance/obstacle_avoidance.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <ch.h>

#define MAP_SIZE_X_MM 3000
#define MAP_SIZE_Y_MM 2000
#define MAP_NUM_ALLY_EDGES 4
#define MAP_ALLY_SIZE_FACTOR 1.25 // for added margin
#define MAP_NUM_OPPONENT 2
#define MAP_NUM_OPPONENT_EDGES 4

struct _map {
    poly_t* rocky_area_obstacle[3];

    poly_t* ally;

    poly_t* opponents[MAP_NUM_OPPONENT];
    uint8_t last_opponent_index;

    mutex_t lock;
    struct obstacle_avoidance oa;

    bool enable_opponent;
};

/** Initialize the map of the Eurobot table with the static obstacles and
 * opponents
 */
void map_init(struct _map* map, int robot_size, bool enable_wall);

/** Set the position of the ally
 */
void map_set_ally_obstacle(struct _map* map, int32_t x, int32_t y, int32_t ally_size, int32_t robot_size);

/** Set the position of the opponent identified by its index
 */
void map_set_opponent_obstacle(struct _map* map, int index, int32_t x, int32_t y, int32_t opponent_size, int32_t robot_size);

/** Get the position of the opponent identified by its index
 */
poly_t* map_get_opponent_obstacle(struct _map* map, int index);

/** Update opponent obstacle position and size
 */
void map_update_opponent_obstacle(struct _map* map, int32_t x, int32_t y, int32_t opponent_size, int32_t robot_size);

/** Set the points of a rectangle given its center position and size
 */
void map_set_rectangular_obstacle(poly_t* opponent, int center_x, int center_y, int size_x, int size_y, int robot_size);
void map_set_rectangular_obstacle_from_corners(poly_t* opponent, int bottom_left_x, int bottom_left_y, int32_t top_right_x, int32_t top_right_y, int robot_size);

#ifdef __cplusplus
}
#endif

#endif /* MAP_H */
