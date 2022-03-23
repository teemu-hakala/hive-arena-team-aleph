#include "team-aleph.h"

#define PRISTINE_PATH_SQUARE 42

command_t	find_path(t_cell_history grid[NUM_ROWS][NUM_COLS], t_bees *bees, \
	agent_info_t info)
{
	command_t	best_command;
	bool		layer_is_filled;
	int			upcoming_path_weight;

	for (int row = 0; row < NUM_ROWS; row++)
	{
		for (int col = 0; col < NUM_ROWS; col++)
		{
			grid[bees->bees[info.bee].coords.row][bees->bees[info.bee].coords.col].pathing_layer = PRISTINE_PATH_SQUARE;
		}
	}
	grid[bees->bees[info.bee].coords.row][bees->bees[info.bee].coords.col].pathing_layer = 0;

	for (int i = 0; i < 42; i++)
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
						grid[row][col].pathing_layer = upcoming_path_weight + get_path_cost();
				}
			}
		}
		if (layer_is_filled)
			break ;
	}
	return (best_command);
}
