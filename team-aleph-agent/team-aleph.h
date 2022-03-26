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

#define TURNS_FORAGING 0
#define DEFENDER_FORAGE_TURNS 0
#define DEFENDER_BEE_INDEX 3
#define FORAGER_BEE_INDEX 4
typedef enum grid_cell_e
{
	NO_INFO,
    EMPTY_ALEPH,
	EMPTY_TARGET,
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
	WALL_TARGET,
    HIVE_ALEPH,
    HIVE_ENEMY,
	MARKED_FOR_EXPLORATION,
	NO_GO
} grid_cell_t;

static const coords_t builder_offsets[] = {
	{1, 0},
	{-1, 0},
	{0, 1},
	{1, 1},
	{-1, 1},
};

typedef struct
{
	grid_cell_t cell;
	int			turn;
	bool		is_stack;
	bool		target_stack;
	int			adjacents;
	int			pathing_layer_cell;
} t_cell_history;

typedef enum e_role
{
	SCOUT,
	FORAGER,
	HIVE_FORAGER,
	ATTACKER,
	BUILDER,
	WAYPOINT,
	DEFENDER
} t_role;

typedef struct
{
	t_role		role;
	coords_t	coords;
	bool		with_flower;
	coords_t	target;
	int			builder_index;
} t_bee;

typedef struct
{
	t_bee	bees[NUM_BEES];
	int		attackers;
	int		builders;
	int		hive_foragers;
	int		forage_distance;
	int		hive_forage_distance;
	coords_t	foraging_target;
	int			guard_attempts;
	coords_t	top_left_stack[2];
}	t_bees;

void	update_grid(agent_info_t info, \
	t_cell_history grid[NUM_ROWS][NUM_COLS], t_bee bees[NUM_BEES]);
void	initialize_bees(t_bee bees[NUM_BEES]);
bool	is_aleph_bee_with_flower(grid_cell_t bee);
int	distance_between_points(coords_t coord0, coords_t coord1);
coords_t	hive_coords(int player);
command_t	choose_action(agent_info_t info, t_cell_history grid[NUM_ROWS][NUM_COLS], t_bees *bees);
int	get_info_from_coord(coords_t current, t_cell_history grid[NUM_ROWS][NUM_COLS]);
bool	coords_equal(coords_t coords0, coords_t coords1);
command_t new_forage_route(t_cell_history grid[NUM_ROWS][NUM_COLS], t_bee *bee, agent_info_t info, t_bees *bees);
command_t	best_builder_action(agent_info_t info, \
			t_cell_history grid[NUM_ROWS][NUM_COLS], t_bees *bees);
bool		is_grid_wall(grid_cell_t grid);
command_t	best_defender_action(agent_info_t info, \
	t_cell_history grid[NUM_ROWS][NUM_COLS], \
	t_bees *bees);
dir_t	direction_from_coords(coords_t bee_coords, \
	coords_t foraging_target_coords);
void	update_heatmap(t_cell_history grid[NUM_ROWS][NUM_COLS], int player, t_bees *bees);
int		find_neighbour(grid_cell_t type, t_cell_history grid[NUM_ROWS][NUM_COLS], coords_t location);
command_t	find_path(t_cell_history grid[NUM_ROWS][NUM_COLS], t_bees *bees, \
	agent_info_t info, coords_t target);

#endif
