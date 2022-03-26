#include "team-aleph.h"

void	print_grid(t_cell_history grid[NUM_ROWS][NUM_COLS], agent_info_t info)
{
	printf("player %d turn %d\n\n", info.player, info.turn);
	for (int row = 0; row < NUM_ROWS; row++)
	{
		for (int col = 0; col < NUM_COLS; col++)
		{
			switch (grid[row][col].cell)
			{
				case NO_INFO:
					printf(". ");
					break ;
				case MARKED_FOR_EXPLORATION:
					printf("* ");
					break ;
				case FLOWER_ALEPH:
					printf("f ");
					break ;
				case TARGET_FLOWER:
					printf("F ");
					break ;
				default:
					printf("# ");
					break ;
			}
			fflush(stdout);
		}
		printf("\n");
	}
	printf("\n");
}

void	find_forage_flower(t_bee *current_bee, \
	t_cell_history grid[NUM_ROWS][NUM_COLS], t_bees *bees)
{
	coords_t	best;
	int			best_distance;
	int			temp_distance;

	best_distance = NUM_COLS;
	best.row = -1;
	best.col = -1;
	for (int row = 0; row < NUM_ROWS; row++)
	{
		for (int col = 0; col < NUM_COLS; col++)
		{
			if (grid[row][col].cell != FLOWER_ALEPH || distance_between_points(bees->foraging_target, (coords_t){.row = row, .col = col}) == 0)
				continue ;
			temp_distance = distance_between_points(current_bee->coords, (coords_t){.row = row, .col = col});
			if (temp_distance < best_distance)
			{
				best_distance = temp_distance;
				best.row = row;
				best.col = col;
			}
		}
	}
	current_bee->target = best;
	if (best.row >= 0)
		grid[best.row][best.col].cell = TARGET_FLOWER;
}

void	find_last_forager_flower(t_bee *current_bee, \
	t_cell_history grid[NUM_ROWS][NUM_COLS], t_bees *bees)
{
	coords_t	best;
	int			best_distance;
	int			temp_distance;

	best_distance = NUM_COLS;
	best.row = -1;
	best.col = -1;
	for (int row = 0; row < NUM_ROWS; row++)
	{
		for (int col = 0; col < NUM_COLS; col++)
		{
			if ((grid[row][col].cell != FLOWER_ALEPH && grid[row][col].cell != TARGET_FLOWER) || distance_between_points(bees->foraging_target, (coords_t){.row = row, .col = col}) == 0)
				continue ;
			temp_distance = distance_between_points(current_bee->coords, (coords_t){.row = row, .col = col});
			if (temp_distance < best_distance)
			{
				best_distance = temp_distance;
				best.row = row;
				best.col = col;
			}
		}
	}
	current_bee->target = best;
}

coords_t	find_explore_forage_target(t_cell_history grid[NUM_ROWS][NUM_COLS], agent_info_t info)
{
	coords_t	temp_coord;
	int			best_distance;
	int			temp_distance;
	coords_t	best;

	best.row = 0;
	best.col = 0;
	best_distance = NUM_COLS;
	for (int row = 0; row < NUM_ROWS; row++)
	{
		for (int col = 0; col < NUM_COLS; col++)
		{
			temp_coord.row = row;
			temp_coord.col = col;
			if (temp_coord.row < 0 || temp_coord.row >= NUM_ROWS
				|| temp_coord.col < 0 || temp_coord.col >= NUM_COLS)
				continue ;
			if (grid[temp_coord.row][temp_coord.col].cell == NO_INFO)
			{
				temp_distance = distance_between_points(temp_coord, (coords_t) {.row = info.row, .col = info.col});
				if (temp_distance < best_distance)
				{
					best_distance = temp_distance;
					best = temp_coord;
				}
			}
		}
	}
	grid[best.row][best.col].cell = MARKED_FOR_EXPLORATION;
	grid[best.row][best.col].turn = info.turn;
	return (best);
}

command_t	best_explore_forage_route(t_cell_history grid[NUM_ROWS][NUM_COLS], t_bees *bees, agent_info_t info)
{
	coords_t	temp_target;

	temp_target = find_explore_forage_target(grid, info);
	return (find_path(grid, bees, info, temp_target));
}

command_t new_forage_route(t_cell_history grid[NUM_ROWS][NUM_COLS], t_bee *bee, agent_info_t info, t_bees *bees)
{
	if (bee->target.row >= 0)
	{
		if (bees->foraging_target.row > 0 && is_aleph_bee_with_flower(grid[info.row][info.col].cell))
			bee->target = bees->foraging_target;
		return (find_path(grid, bees, info, bees->bees[info.bee].target));
	}
	else
	{
		if (is_aleph_bee_with_flower(grid[info.row][info.col].cell))
		{
			if (bees->foraging_target.row < 0)
				bee->target = hive_coords(info.player);
			else
				bee->target = bees->foraging_target;
		}
		else
		{
			bee->target.row = -1;
			if (info.bee == FORAGER_BEE_INDEX)
				find_last_forager_flower(bee, grid, bees);
			else
				find_forage_flower(bee, grid, bees);
		}
		if (bee->target.row < 0)
			return (best_explore_forage_route(grid, bees, info));
		return (find_path(grid, bees, info, bees->bees[info.bee].target));
	}
}
