#ifndef TEAM_ALEPH_H
#define TEAM_ALEPH_H
#include "agent.h"

typedef enum grid_cell_e
{
	NO_INFO,
    EMPTY_ALEPH,
	BEE_ALEPH,
    BEE_ALEPH_0,
	BEE_ALEPH_1,
	BEE_ALEPH_2,
	BEE_ALEPH_3,
	BEE_ALEPH_4,
    BEE_ENEMY,
	BEE_ALEPH_WITH_FLOWER,
    BEE_ALEPH_0_WITH_FLOWER,
	BEE_ALEPH_1_WITH_FLOWER,
	BEE_ALEPH_2_WITH_FLOWER,
	BEE_ALEPH_3_WITH_FLOWER,
	BEE_ALEPH_4_WITH_FLOWER,
    BEE_ENEMY_WITH_FLOWER,
    FLOWER_ALEPH,
	TARGET_FLOWER,
    WALL_ALEPH,
	WALL_ENEMY,
    HIVE_ALEPH,
    HIVE_ENEMY
} grid_cell_t;

typedef struct
{
	grid_cell_t cell;
	int turn;
} t_cell_history;

void	update_grid(agent_info_t info, t_cell_history grid[NUM_ROWS][NUM_COLS]);
command_t	best_scout_route(t_cell_history grid[NUM_ROWS][NUM_COLS], coords_t bee);

#endif