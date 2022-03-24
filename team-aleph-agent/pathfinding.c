#include "team-aleph.h"

#define PRISTINE_PATH_SQUARE 4200
#define INFINITY 88
#define PATH_COST_EMPTY 1
#define PATH_COST_WALL_UNDEFENDED 3

// % 2d

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

/*int	get_path_cost(t_cell_history cell_history)
{
	//	Take into account cell_history.adjacents
	//	Should be equal to 0 for a wall to break.
	if (is_grid_wall(cell_history.cell))
	{
		if (cell_history.adjacents == 0)
			return (PATH_COST_WALL_UNDEFENDED);
		else
			return (INFINITY);
	}
	else if (cell_history.cell == EMPTY_ALEPH)
		return (PATH_COST_EMPTY);

}

int	get_lowest_path_weight_from_adjacent_squares(t_cell_history grid[NUM_ROWS][NUM_COLS], \
	coords_t temp_coord)
{
	int			temp_weight;
	int			best_weight;
	coords_t	temp_coord;

	best_weight = PRISTINE_PATH_SQUARE;
	for (int d = 0; d < 8; d++)
	{
		temp_coord = direction_to_coords(temp_coord, d);
		if (temp_coord.row < 0 || temp_coord.row >= NUM_ROWS ||
			temp_coord.col < 0 || temp_coord.col >= NUM_COLS)
			continue ;
		temp_weight = grid[temp_coord.row][temp_coord.col].pathing_layer_cell;
		if (temp_weight < best_weight)
		{
			best_weight = temp_weight;
		}
	}
	return (best_weight);
}

coords_t	get_lowest_weighing_path_square_coordinates(t_cell_history grid[NUM_ROWS][NUM_COLS], coords_t best_temp_coord)
{
	coords_t	best_coords;
	coords_t	temp_coords;

	for (int d = 0; d < 8; d++)
	{
		direction_to_coords(best_temp_coord, d);
		direction_from_coords();
	}
}

command_t	find_path(t_cell_history grid[NUM_ROWS][NUM_COLS], t_bees *bees, \
	agent_info_t info)
{
	command_t	best_command;
	bool		layer_is_filled;
	bool		target_found;
	int			upcoming_path_weight;
	int			depth;
	coords_t	best_temp_coord;

	for (int row = 0; row < NUM_ROWS; row++)
	{
		for (int col = 0; col < NUM_COLS; col++)
		{
			grid[row][col].pathing_layer_cell = PRISTINE_PATH_SQUARE;
		}
	}
	grid[bees->bees[info.bee].coords.row][bees->bees[info.bee].coords.col].pathing_layer_cell = 0;

	best_command.action = MOVE;
	depth = 0;
	for (; depth < PRISTINE_PATH_SQUARE; depth++)
	{
		layer_is_filled = true;
		for (int row = 0; row < NUM_ROWS; row++)
		{
			for (int col = 0; col < NUM_COLS; col++)
			{
				if (grid[row][col].adjacents != 0) // update heatmap for foragers too.
					continue ;
				if (grid[row][col].pathing_layer_cell == PRISTINE_PATH_SQUARE)
				{
					layer_is_filled = false;
					upcoming_path_weight = get_lowest_path_weight_from_adjacent_squares(grid, (coords_t) {.row = row, .col = col});
					if (upcoming_path_weight != PRISTINE_PATH_SQUARE)
					{
						grid[row][col].pathing_layer_cell = upcoming_path_weight + get_path_cost(grid[row][col]);
						if (coords_equal((coords_t) {.row = row, .col = col}, bees->bees[info.bee].target))
							target_found = true;
					}
				}
			}
			if (target_found)
				break ;
		}
		if (layer_is_filled || target_found)
			break ;
	}
	if (target_found)
	{
		best_temp_coord = bees->bees[info.bee].target;
		for (; depth >= 1; depth--)
		{
			best_temp_coord = get_lowest_weighing_path_square_coordinates(grid, best_temp_coord);
		}
	}
	best_command.direction = direction_from_coords(bees->bees[info.bee].coords, best_temp_coord);
	if (is_grid_wall(grid[best_temp_coord.row][best_temp_coord.col].cell))
		best_command.action = GUARD;
	return (best_command);
}*/

int	cell_cost(grid_cell_t cell, int heat_level)
{
	if (is_grid_wall(cell) && heat_level == 0)
		return (PATH_COST_WALL_UNDEFENDED);
	else if (cell == EMPTY_ALEPH || cell == EMPTY_TARGET)
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
			+ cell_cost(grid[temp_coord.row][temp_coord.col].cell, grid[temp_coord.row][temp_coord.col].adjacents);
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
	// printf("pathing_layer after reset:\n\n");
	// print_pathing_layer(grid);

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
	// printf("pathing_layer after finding target:\n\n");
	// print_pathing_layer(grid);
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
	// printf("pathing_layer after retracing our steps:\n\n");
	// print_pathing_layer(grid);
	best_command.direction = direction_from_coords(bees->bees[info.bee].coords, best_coord);
	if (is_grid_wall(grid[best_coord.row][best_coord.col].cell))
		best_command.action = GUARD;
	else if (coords_equal(best_coord, target))
		best_command.action = FORAGE;
	else
		best_command.action = MOVE;
	return (best_command);
}
