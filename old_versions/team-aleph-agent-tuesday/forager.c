#include "team-aleph.h"

void	find_flower_in_forage_distance(t_bee *current_bee, int forage_distance, \
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
	for (int col = forage_distance; col >= 0 && col < NUM_COLS; col += iteration_step)
	{
		// printf("iterstep %d col %d\n", iteration_step, col);
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

void	find_flower(t_bee *current_bee, int forage_distance, \
	t_cell_history grid[NUM_ROWS][NUM_COLS], int player, int hive_forage_distance)
{
	// if (enemy_bee_is_close_and_adjacent_flower(grid, current_bee))
	// {
	// 	return ;
	// }
	switch (current_bee->role)
	{
		case FORAGER:
			find_flower_in_forage_distance(current_bee, forage_distance, \
				grid, player);
			break ;
		case HIVE_FORAGER:
			find_flower_in_forage_distance(current_bee, hive_forage_distance, \
				grid, player);
			break ;
		default:
			break ;
	}
}

coords_t find_stack_cell(agent_info_t info, t_bee bee, t_cell_history grid[NUM_ROWS][NUM_COLS], t_bees bees)
{
	coords_t	best_cell;
	coords_t	temp_cell;
	int			best_distance;
	int			temp_distance;

	best_cell.row = -1;
	best_distance = NUM_COLS;
	for (int idx = 0; idx < 2; idx++)
	{
		for (int s = 0; s < 5; s++)
		{
			temp_cell.row = bees.top_left_stack[idx].row + stack_cells[s].row;
			temp_cell.col = bees.top_left_stack[idx].col + stack_cells[s].col;
			if (grid[temp_cell.row][temp_cell.col].cell != EMPTY_ALEPH || grid[temp_cell.row][temp_cell.col].target_stack)
				continue;
			temp_distance = distance_between_points(bee.coords,	temp_cell);
			if (temp_distance < best_distance)
			{
				best_cell = temp_cell;
				best_distance = temp_distance;
			}
		}
	}
	if (best_cell.row < 0)
		return (hive_coords(info.player));
	grid[best_cell.row][best_cell.col].target_stack = true;
	return (best_cell);
}

coords_t	target_for_flower(agent_info_t info, t_cell_history grid[NUM_ROWS][NUM_COLS], t_bees *bees)
{
	// (void) grid;
	// (void) bees;
	// return (hive_coords(info.player));
	switch(bees->bees[info.bee].role)
	{
		case HIVE_FORAGER:
			return (hive_coords(info.player));
		case FORAGER:
			if (enemy_bee_is_close(grid, &bees->bees[info.bee]))
				return (hive_coords(info.player));
			return(find_stack_cell(info, bees->bees[info.bee], grid, *bees));
		default:
			return (hive_coords(info.player));
	}
}

bool	target_is_invalid(t_cell_history grid[NUM_ROWS][NUM_COLS], t_bee *current_bee, agent_info_t info)
{
	if (grid[current_bee->target.row][current_bee->target.col].cell != TARGET_FLOWER \
		&& !is_aleph_bee_with_flower(grid[info.row][info.col].cell))
	{
		current_bee->target.row = -1;
		return (true);
	}
	else if (grid[current_bee->target.row][current_bee->target.col].cell != TARGET_FLOWER \
		&& !coords_equal(hive_coords(info.player), current_bee->target) \
		&& distance_between_points(current_bee->coords, current_bee->target) < 2
		&& enemy_bee_is_close(grid, current_bee))
	{
		grid[current_bee->target.row][current_bee->target.col].target_stack = false;
		current_bee->target = hive_coords(info.player);
		return (true);
	}
	return (false);
}

command_t	best_forage_route(agent_info_t info, \
	t_cell_history grid[NUM_ROWS][NUM_COLS], \
	t_bees *bees)
{
	coords_t	temp_coord;
	command_t	best;
	int			best_distance;
	int			temp_distance;
	bool		is_wall;

	is_wall = false;
	if (bees->bees[info.bee].target.row < 0 || target_is_invalid(grid, &bees->bees[info.bee], info))
	{
		if (bees->bees[info.bee].target.row < 0)
		{
			if (is_aleph_bee_with_flower(grid[info.row][info.col].cell))
				bees->bees[info.bee].target = target_for_flower(info, grid, bees);
			else
				find_flower(&bees->bees[info.bee], bees->forage_distance, \
					grid, info.player, bees->hive_forage_distance);
		}
		// printf("player %d turn %d bee %d target %d %d cell %d\n", info.player, info.turn, info.bee, bees->bees[info.bee].target.row,
		// bees->bees[info.bee].target.col, grid[bees->bees[info.bee].target.row][bees->bees[info.bee].target.col].cell);
	}
	// printf("%d %d\n", bees->bees[info.bee].target.row, bees->bees[info.bee].target.col);
	if (bees->bees[info.bee].target.row < 0)
	{
		if (!no_flowers_in_forage_area(grid, 0, NUM_COLS, info.player))
		{
			bees->bees[info.bee].role = ATTACKER;
			return (best_attack_route(grid, &bees->bees[info.bee], info, bees));
		}
		else
			return (best_scout_route(grid, &bees->bees[info.bee], info.player));
	}
	best_distance = NUM_COLS;
	for (int d = 0; d < 8; d++)
	{
		temp_coord = direction_to_coords(bees->bees[info.bee].coords, d);
		if (temp_coord.row < 0 || temp_coord.row >= NUM_ROWS
			|| temp_coord.col < 0 || temp_coord.col >= NUM_COLS)
			continue ;
		temp_distance = distance_between_points(temp_coord, bees->bees[info.bee].target);
		// printf("dir %d dist %d\n", d, temp_distance);
		if (temp_distance == 0)
		{
			bees->bees[info.bee].target.row = -1;
			if (grid[temp_coord.row][temp_coord.col].target_stack)
				grid[temp_coord.row][temp_coord.col].target_stack = false;
			return ((command_t){.action = FORAGE, .direction = d});
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
