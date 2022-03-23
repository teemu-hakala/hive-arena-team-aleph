#include "team-aleph.h"

#define PRISTINE_PATH_SQUARE 42

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
		for (int col = 0; col < NUM_ROWS; col++)
		{
			grid[bees->bees[info.bee].coords.row][bees->bees[info.bee].coords.col].pathing_layer = PRISTINE_PATH_SQUARE;
		}
	}
	grid[bees->bees[info.bee].coords.row][bees->bees[info.bee].coords.col].pathing_layer = 0;

	depth = 0;
	for (; depth < 42; depth++)
	{
		layer_is_filled = true;
		for (int row = 0; row < NUM_ROWS; row++)
		{
			for (int col = 0; col < NUM_COLS; col++)
			{
				if (grid[row][col].pathing_layer == PRISTINE_PATH_SQUARE)
				{
					layer_is_filled = false;
					upcoming_path_weight = get_lowest_path_weight_from_adjacent_squares(grid, (coords_t) {.row = row, .col = col});
					if (upcoming_path_weight != PRISTINE_PATH_SQUARE)
					{
						grid[row][col].pathing_layer = upcoming_path_weight + get_path_cost(grid[row][col].cell);
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
	return (best_command);
}
