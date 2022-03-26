#include "team-aleph.h"

#define PRISTINE_PATH_SQUARE 4200
#define INFINITY 88
#define PATH_COST_EMPTY 1
#define PATH_COST_WALL_UNDEFENDED 3

void	print_pathing_layer(t_cell_history grid[NUM_ROWS][NUM_COLS])
{
	for (int row = 0; row < NUM_ROWS; row++)
	{
		for (int col = 0; col < NUM_COLS; col++)
		{
			if (grid[row][col].pathing_layer_cell > 99)
				printf(" # ");
			else
				printf("% 2d ", grid[row][col].pathing_layer_cell);
			fflush(stdout);
		}
		printf("\n");
	}
	printf("\n");
}

int	cell_cost(grid_cell_t cell, int heat_level, int bee)
{
	(void) bee;
	if (is_grid_wall(cell) && heat_level == 0)
		return (PATH_COST_WALL_UNDEFENDED);
	else if (cell == EMPTY_ALEPH || cell == EMPTY_TARGET || (bee != DEFENDER_BEE_INDEX && bee != FORAGER_BEE_INDEX && cell == NO_INFO))
		return (PATH_COST_EMPTY);
	else
		return (PRISTINE_PATH_SQUARE / 2);
}

bool	set_adjacent_cells_levels(t_cell_history grid[NUM_ROWS][NUM_COLS], int depth, t_bees *bees, \
	agent_info_t info, coords_t current_coord)
{
	coords_t	temp_coord;
	bool		target_found;

	target_found = false;
	for (int d = 0; d < 8; d++)
	{
		temp_coord = direction_to_coords(current_coord, d);
		if (temp_coord.row < 0 || temp_coord.row >= NUM_ROWS ||
			temp_coord.col < 0 || temp_coord.col >= NUM_COLS)
			continue ;
		if (grid[temp_coord.row][temp_coord.col].pathing_layer_cell != PRISTINE_PATH_SQUARE)
			continue ;
		grid[temp_coord.row][temp_coord.col].pathing_layer_cell = depth \
			+ cell_cost(grid[temp_coord.row][temp_coord.col].cell, grid[temp_coord.row][temp_coord.col].adjacents, info.bee);
		if (coords_equal(temp_coord, bees->bees[info.bee].target))
		{
			target_found = true;
			break ;
		}
	}
	return (target_found);
}

command_t	find_path(t_cell_history grid[NUM_ROWS][NUM_COLS], t_bees *bees, \
	agent_info_t info, coords_t	target)
{
	command_t	best_command;
	bool		target_found;
	int			depth;
	int			temp_depth;
	int 		best_depth;
	coords_t	best_coord;
	coords_t	temp_coord;
	coords_t	prev_coord;

	for (int row = 0; row < NUM_ROWS; row++)
	{
		for (int col = 0; col < NUM_COLS; col++)
		{
			grid[row][col].pathing_layer_cell = PRISTINE_PATH_SQUARE;
		}
	}
	grid[bees->bees[info.bee].coords.row][bees->bees[info.bee].coords.col].pathing_layer_cell = 0;

	depth = 0;
	for (; depth < PRISTINE_PATH_SQUARE; depth++)
	{
		for (int row = 0; row < NUM_ROWS; row++)
		{
			for (int col = 0; col < NUM_COLS; col++)
			{
				if (grid[row][col].pathing_layer_cell == depth)
				{
					target_found = set_adjacent_cells_levels(grid, depth, bees, info, \
						(coords_t) {.row = row, .col = col});
					if (target_found)
						break ;
				}
			}
			if (target_found)
						break ;
		}
		if (target_found)
						break ;
	}
	best_coord.row = target.row;
	best_coord.col = target.col;
	prev_coord.row = target.row;
	prev_coord.col = target.col;
	for (depth = grid[best_coord.row][best_coord.col].pathing_layer_cell; depth > 0; )
	{
		best_depth = depth;
		for (int d = 0; d < 8; d++)
		{
			temp_coord = direction_to_coords(prev_coord, d);
			if (temp_coord.row < 0 || temp_coord.row >= NUM_ROWS ||
				temp_coord.col < 0 || temp_coord.col >= NUM_COLS)
				continue ;
			temp_depth = grid[temp_coord.row][temp_coord.col].pathing_layer_cell;
			if (temp_depth == 0)
				break ;
			if (temp_depth < best_depth)
			{
				best_depth = temp_depth;
				best_coord.row = temp_coord.row;
				best_coord.col = temp_coord.col;
			}
		}
		depth = best_depth;
		prev_coord.row = best_coord.row;
		prev_coord.col = best_coord.col;
		if (temp_depth == 0)
				break ;
	}
	best_command.direction = direction_from_coords(bees->bees[info.bee].coords, best_coord);
	if (is_grid_wall(grid[best_coord.row][best_coord.col].cell))
		best_command.action = GUARD;
	else if (coords_equal(best_coord, target))
		best_command.action = FORAGE;
	else
		best_command.action = MOVE;
	return (best_command);
}
