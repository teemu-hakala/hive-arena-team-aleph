#include "team-aleph.h"

void	initialize_bees(t_bee bees[NUM_BEES])
{
	for (int b = 0; b < NUM_BEES; b++)
	{
		if (b == DEFENDER_BEE_INDEX)
			bees[b].role = DEFENDER;
		else
			bees[b].role = FORAGER;
		bees[b].target.row = -1;
	}
}

dir_t	direction_from_coords(coords_t bee_coords, \
	coords_t foraging_target_coords)
{
	int		row_diff;
	int		col_diff;

	row_diff = foraging_target_coords.row - bee_coords.row;
	col_diff = foraging_target_coords.col - bee_coords.col;
	if (row_diff == -1 && col_diff == 0)
		return (N);
	else if (row_diff == -1  && col_diff == 1)
		return (NE);
	else if (row_diff ==  0 && col_diff == 1)
		return (E);
	else if (row_diff ==  1 && col_diff == 1)
		return (SE);
	else if (row_diff == 1 && col_diff == 0)
		return (S);
	else if (row_diff == 1 && col_diff == -1)
		return (SW);
	else if (row_diff == 0 && col_diff == -1)
		return (W);
	else if (row_diff == -1 && col_diff == -1)
		return (NW);
	return (N);
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
		return ((coords_t){.row = 12, .col = 1});
	else
		return ((coords_t){.row = 12, .col = 28});
}

int	get_forage_distance(int player)
{
	if (player == 0)
		return (15);
	else
		return(14);
}

int	get_hive_forage_distance(int player)
{
	if (player == 0)
		return (6);
	else
		return(23);
}


bool	coords_equal(coords_t coords0, coords_t coords1)
{
	return (coords0.row == coords1.row && coords0.col == coords1.col);
}

bool	enemy_bee_is_close_and_adjacent_flower(t_cell_history grid[NUM_ROWS][NUM_COLS], t_bee *current_bee)
{
	coords_t	temp_coord;

	for (int row = current_bee->coords.row - VIEW_DISTANCE; row <= current_bee->coords.row + VIEW_DISTANCE; row++)
	{
		for (int col = current_bee->coords.col - VIEW_DISTANCE; col <= current_bee->coords.col + VIEW_DISTANCE; col++)
		{
			if (row < 0 || row >= NUM_ROWS || col < 0 || col >= NUM_COLS)
				continue ;
			if (grid[row][col].cell == BEE_ENEMY)
			{
				for (int d = 0; d < 8; d++)
				{
					temp_coord = direction_to_coords((coords_t) {.row = row, .col = col}, d);
					if (distance_between_points(current_bee->coords, temp_coord) == 1 \
						&& (grid[temp_coord.row][temp_coord.col].cell == FLOWER_ALEPH \
						|| grid[temp_coord.row][temp_coord.col].cell == TARGET_FLOWER))
					{
						current_bee->target = temp_coord;
						return (true);
					}
				}
			}
		}
	}
	return (false);
}

bool	enemy_bee_is_close(t_cell_history grid[NUM_ROWS][NUM_COLS], t_bee *current_bee)
{
	for (int row = current_bee->coords.row - VIEW_DISTANCE; row <= current_bee->coords.row + VIEW_DISTANCE; row++)
	{
		for (int col = current_bee->coords.col - VIEW_DISTANCE; col <= current_bee->coords.col + VIEW_DISTANCE; col++)
		{
			if (row < 0 || row >= NUM_ROWS || col < 0 || col >= NUM_COLS)
				continue ;
			if (grid[row][col].cell == BEE_ENEMY)
			{
				return (true);
			}
		}
	}
	return (false);
}

bool	no_flowers_in_forage_area(t_cell_history grid[NUM_ROWS][NUM_COLS], int forage_distance, int hive_forage_distance, int player)
{
	int iteration_step;

	iteration_step = -1 + 2 * player;
	for (int col = forage_distance; col != hive_forage_distance; col += iteration_step)
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

bool	is_grid_wall(grid_cell_t grid)
{
	return (grid == WALL_ALEPH || grid == WALL_ENEMY || grid == WALL_TARGET);
}

int		find_neighbour(grid_cell_t type, t_cell_history grid[NUM_ROWS][NUM_COLS], coords_t location)
{
	coords_t temp_coord;

	for (int d = 0; d < 8; d++)
	{
		temp_coord = direction_to_coords(location, d);
		if (temp_coord.row < 0 || temp_coord.row >= NUM_ROWS
			|| temp_coord.col < 0 || temp_coord.col >= NUM_COLS)
			continue ;
		if (grid[temp_coord.row][temp_coord.col].cell == type)
			return (d);
	}
	return (-1);
}
