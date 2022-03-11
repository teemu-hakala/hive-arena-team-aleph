#ifndef TEAM_ALEPH_H
#define TEAM_ALEPH_H
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <strings.h>
#include <stdbool.h>
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

typedef enum e_role
{
	SCOUT,
	FORAGER,
	HIVE_FORAGER,
	ATTACKER,
	BUILDER
} t_role;
typedef struct
{
	t_role		role;
	coords_t	coords;
	bool		with_flower;
	coords_t	target;
} t_bee;

typedef struct
{
	t_bee	bees[NUM_BEES];
	int		attackers;
	int		hive_foragers;
	int		forage_distance;
}	t_bees;

void	update_grid(agent_info_t info, \
	t_cell_history grid[NUM_ROWS][NUM_COLS], t_bee bees[NUM_BEES]);
command_t	best_scout_route(t_cell_history grid[NUM_ROWS][NUM_COLS], coords_t bee);
void	initialize_bees(t_bee bees[NUM_BEES]);
bool	is_aleph_bee_with_flower(grid_cell_t bee);
int	distance_between_points(coords_t coord0, coords_t coord1);
coords_t	hive_coords(int player);
command_t	best_forage_route(agent_info_t info, \
	t_cell_history grid[NUM_ROWS][NUM_COLS], \
	t_bees *bees);

#endif
