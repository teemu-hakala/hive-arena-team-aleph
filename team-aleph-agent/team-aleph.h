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

#define TURNS_FORAGING 300

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
	WALL_TARGET,
    HIVE_ALEPH,
    HIVE_ENEMY,
	MARKED_FOR_EXPLORATION
} grid_cell_t;

static const coords_t stack_cells[] = {
    {0, 0},
    {0, 2},
    { 1, 1},
    { 2, 0},
    { 2, 2},
};

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
} t_cell_history;

typedef enum e_role
{
	SCOUT,
	FORAGER,
	HIVE_FORAGER,
	ATTACKER,
	BUILDER,
	WAYPOINT
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
	coords_t	top_left_stack[2];
}	t_bees;

void	update_grid(agent_info_t info, \
	t_cell_history grid[NUM_ROWS][NUM_COLS], t_bee bees[NUM_BEES]);
command_t	best_scout_route(t_cell_history grid[NUM_ROWS][NUM_COLS], t_bee *bee, int player);
void	initialize_bees(t_bee bees[NUM_BEES]);
bool	is_aleph_bee_with_flower(grid_cell_t bee);
int	distance_between_points(coords_t coord0, coords_t coord1);
coords_t	hive_coords(int player);
command_t	best_forage_route(agent_info_t info, \
	t_cell_history grid[NUM_ROWS][NUM_COLS], \
	t_bees *bees);
int	get_forage_distance(int player);
int	get_hive_forage_distance(int player);
command_t	choose_action(agent_info_t info, t_cell_history grid[NUM_ROWS][NUM_COLS], t_bees *bees);
void	create_stacks(t_cell_history grid[NUM_ROWS][NUM_COLS], int player, t_bees *bees);
bool	no_flowers_in_forage_area(t_cell_history grid[NUM_ROWS][NUM_COLS], int forage_distance, int hive_forage_distance, int player);
command_t	best_waypoint_route(t_cell_history grid[NUM_ROWS][NUM_COLS], t_bee *bee, int player);
command_t best_attack_route(t_cell_history grid[NUM_ROWS][NUM_COLS], t_bee *bee, agent_info_t info, t_bees *bees);
int	get_info_from_coord(coords_t current, t_cell_history grid[NUM_ROWS][NUM_COLS]);
coords_t	best_attack_target(int player, int index);
bool	coords_equal(coords_t coords0, coords_t coords1);
bool	enemy_bee_is_close_and_adjacent_flower(t_cell_history grid[NUM_ROWS][NUM_COLS], t_bee *current_bee);
bool	enemy_bee_is_close(t_cell_history grid[NUM_ROWS][NUM_COLS], t_bee *current_bee);
command_t new_forage_route(t_cell_history grid[NUM_ROWS][NUM_COLS], t_bee *bee, agent_info_t info);
command_t	best_builder_action(agent_info_t info, \
			t_cell_history grid[NUM_ROWS][NUM_COLS], t_bees *bees);
bool		is_grid_wall(grid_cell_t grid);

#endif
