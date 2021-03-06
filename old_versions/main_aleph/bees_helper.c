#include "team-aleph.h"

static const coords_t scout_targets[2][5] = {
	{{3, 3},
	{ 7, 7},
	{12, 9},
	{17, 7},
	{21, 3}},
	{{3, 26},
	{ 7, 22},
	{12, 20},
	{17, 22},
	{21, 26}}
};

void	initialize_bees(t_bee bees[NUM_BEES], int player)
{
	for (int b = 0; b < NUM_BEES; b++)
	{
		bees[b].role = WAYPOINT;
		bees[b].target = scout_targets[player][b];
	}
}

int	distance_between_points(coords_t coord0, coords_t coord1)
{
	if (abs(coord0.row - coord1.row) > abs(coord0.col - coord1.col))
		return (abs(coord0.row - coord1.row));
	else
		return (abs(coord0.col - coord1.col));
}

bool	is_aleph_bee_with_flower(grid_cell_t bee)
{
	return (bee >= BEE_ALEPH_WITH_FLOWER && bee <= BEE_ALEPH_4_WITH_FLOWER);
}

coords_t	hive_coords(int player)
{
	if (player == 0)
		return ((coords_t){.row = NUM_ROWS / 2, .col = 1});
	else
		return ((coords_t){.row = NUM_ROWS / 2, .col = NUM_COLS - 2});
}

int	get_forage_distance(int player)
{
	return (NUM_COLS / 2 + 1 - player * (NUM_COLS % 2));
}

bool	no_flowers_in_forage_area(t_cell_history grid[NUM_ROWS][NUM_COLS], int forage_distance, int player)
{
	int iteration_step;
	int	stop_line;

	iteration_step = -1 + 2 * player;
	if (player == 0)
		stop_line = forage_distance / 2;
	else
		stop_line = forage_distance * 3 / 2 - 1;
	for (int col = forage_distance - 1; col != stop_line; col += iteration_step)
	{
		for (int row = 0; row < NUM_ROWS; row++)
		{
			if (grid[row][col].cell == FLOWER_ALEPH)
				return (false);
		}
	}
	return (true);
}

command_t	best_waypoint_route(t_cell_history grid[NUM_ROWS][NUM_COLS], t_bee *bee, int player)
{
	coords_t	temp_coord;
	command_t	best;
	int			best_distance;
	int			temp_distance;
	bool		is_wall;

	is_wall = false;
	best_distance = NUM_COLS;
	for (int d = 0; d < 8; d++)
	{
		temp_coord = direction_to_coords(bee->coords, d);
		if (temp_coord.row < 0 || temp_coord.row >= NUM_ROWS
			|| temp_coord.col < 0 || temp_coord.col >= NUM_COLS)
			continue ;
		temp_distance = distance_between_points(temp_coord, bee->target);
		if (temp_distance == 0)
		{
			bee->role = SCOUT;
			bee->target.col = NUM_COLS - NUM_COLS * player;
			return ((command_t){.action = MOVE, .direction = d});
		}
		if (grid[temp_coord.row][temp_coord.col].cell != EMPTY_ALEPH && grid[temp_coord.row][temp_coord.col].cell != WALL_ENEMY)
			continue ;
		if (temp_distance < best_distance || (temp_distance == best_distance && grid[temp_coord.row][temp_coord.col].cell != WALL_ENEMY && is_wall))
		{
			best_distance = temp_distance;
			best.action = MOVE;
			best.direction = d;
			if (grid[temp_coord.row][temp_coord.col].cell == WALL_ENEMY)
				is_wall = true;
			else
				is_wall = false;
		}
	}
	if (is_wall)
		best.action = GUARD;
	return (best);
}
