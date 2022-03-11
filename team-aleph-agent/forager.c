#include "team-aleph.h"

void	find_flower(t_bee *current_bee, int forage_distance, \
	t_cell_history grid[NUM_ROWS][NUM_COLS], int player)
{
	coords_t	best;
	int			best_distance;
	int			temp_distance;
	int			iteration_step;

	best.row = -1;
	best.col = -1;
	best_distance = NUM_COLS;
	iteration_step = -1 + 2 * player;
	for (int col = forage_distance - 1; col >= 0 && col < NUM_COLS; col += iteration_step)
	{
		for (int row = 0; row < NUM_ROWS; row++)
		{
			if (grid[row][col].cell != FLOWER_ALEPH)
				continue ;
			temp_distance = distance_between_points((coords_t){.row = row, .col = col}, current_bee->coords);
			if (temp_distance < best_distance)
			{
				best_distance = temp_distance;
				best.row = row;
				best.col = col;
			}
		}
		if (best.col == col)
		{
			current_bee->target = best;
			grid[best.row][best.col].cell = TARGET_FLOWER;
			return ;
		}
	}

}

command_t	best_forage_route(agent_info_t info, \
	t_cell_history grid[NUM_ROWS][NUM_COLS], \
	t_bees *bees)
{
	coords_t	temp_coord;
	command_t	best;
	int			best_distance;
	int			temp_distance;

	if (bees->bees[info.bee].target.row < 0)
	{
		if (is_aleph_bee_with_flower(grid[info.row][info.col].cell))
			bees->bees[info.bee].target = hive_coords(info.player);
		else
			find_flower(&bees->bees[info.bee], bees->forage_distance, \
				grid, info.player);
	}
	// printf("%d %d\n", bees->bees[info.bee].target.row, bees->bees[info.bee].target.col);
	if (bees->bees[info.bee].target.row < 0)
		return (best_scout_route(grid, bees->bees[info.bee].coords));
	best_distance = NUM_COLS;
	for (int d = 0; d < 8; d++)
	{
		temp_coord = direction_to_coords(bees->bees[info.bee].coords, d);

		temp_distance = distance_between_points(temp_coord, bees->bees[info.bee].target);
		// printf("dir %d dist %d\n", d, temp_distance);
		if (temp_distance == 0)
		{
			bees->bees[info.bee].target.row = -1;
			return ((command_t){.action = FORAGE, .direction = d});
		}
		if (grid[temp_coord.row][temp_coord.col].cell != EMPTY_ALEPH)
			continue ;
		if (temp_distance < best_distance)
		{
			best_distance = temp_distance;
			best.action = MOVE;
			best.direction = d;
		}
	}
	return (best);
}
