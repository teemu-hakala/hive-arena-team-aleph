#include "team-aleph.h"

void	initialize_bees(t_bees *bees)
{
	for (int b = 0; b < NUM_BEES; b++)
	{
		if (b == DEFENDER_BEE_INDEX)
			bees->bees[b].role = DEFENDER;
		else if (b == FORAGER_BEE_INDEX)
			bees->bees[b].role = FORAGER;
		else
		{
			bees->bees[b].role = BUILDER;
			bees->bees[b].builder_index = bees->builders++;
		}
		bees->bees[b].target.row = -1;
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
	return (bee == BEE_ALEPH_WITH_FLOWER);
}

coords_t	hive_coords(int player)
{
	if (player == 0)
		return ((coords_t){.row = 12, .col = 1});
	else
		return ((coords_t){.row = 12, .col = 28});
}

bool	coords_equal(coords_t coords0, coords_t coords1)
{
	return (coords0.row == coords1.row && coords0.col == coords1.col);
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
