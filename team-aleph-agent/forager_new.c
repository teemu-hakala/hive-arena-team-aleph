#include "team-aleph.h"

command_t	best_waypoint_route_forager(t_cell_history grid[NUM_ROWS][NUM_COLS], t_bee *bee)
{
	coords_t	temp_coord;
	command_t	best;
	int			best_distance;
	int			temp_distance;
	bool		is_wall;

	best_distance = NUM_COLS;
	is_wall = false;
	for (int d = 0; d < 8; d++)
	{
		temp_coord = direction_to_coords(bee->coords, d);
		if (temp_coord.row < 0 || temp_coord.row >= NUM_ROWS
			|| temp_coord.col < 0 || temp_coord.col >= NUM_COLS)
			continue ;

		temp_distance = distance_between_points(temp_coord, bee->target);
		if (temp_distance == 0)
		{
			bee->target.row = -1;
			// if (grid[temp_coord.row][temp_coord.col].cell == TARGET_FLOWER)
			// 	return ((command_t){.action = FORAGE, .direction = d});
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

void	find_forage_flower(t_bee *current_bee, \
	t_cell_history grid[NUM_ROWS][NUM_COLS])
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
			if (grid[row][col].cell != FLOWER_ALEPH)
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

coords_t	find_explore_forage_target(t_cell_history grid[NUM_ROWS][NUM_COLS], agent_info_t info)
{
	coords_t	temp_coord;
	int			best_info;
	int			temp_info;
	coords_t	best;

	best_info = 0;

	for (int row = info.row - 10; row <= info.row + 10; row++)
	{
		for (int col = info.col - 10; col <= info.col + 10; col++)
		{
			temp_coord.row = row;
			temp_coord.col = col;
			if (temp_coord.row < 0 || temp_coord.row >= NUM_ROWS
				|| temp_coord.col < 0 || temp_coord.col >= NUM_COLS
				|| grid[temp_coord.row][temp_coord.col].cell == MARKED_FOR_EXPLORATION)
				continue ;
			temp_info = get_info_from_coord(temp_coord, grid);
			if (temp_info > best_info)
			{
				best_info = temp_info;
				best = temp_coord;
			}
		}
	}
	grid[best.row][best.col].cell = MARKED_FOR_EXPLORATION;
	grid[best.row][best.col].turn = info.turn;
	return (best);
}

command_t	best_explore_forage_route(t_cell_history grid[NUM_ROWS][NUM_COLS], t_bee *bee, agent_info_t info)
{
	coords_t	temp_coord;
	command_t	best;
	int			best_distance;
	int			temp_distance;
	bool		is_wall;
	coords_t	temp_target;

	temp_target = find_explore_forage_target(grid, info);
	best_distance = NUM_COLS;
	is_wall = false;
	for (int d = 0; d < 8; d++)
	{
		temp_coord = direction_to_coords(bee->coords, d);
		if (temp_coord.row < 0 || temp_coord.row >= NUM_ROWS
			|| temp_coord.col < 0 || temp_coord.col >= NUM_COLS)
			continue ;

		temp_distance = distance_between_points(temp_coord, temp_target);
		if (temp_distance == 0)
		{
			// if (grid[temp_coord.row][temp_coord.col].cell == TARGET_FLOWER)
			// 	return ((command_t){.action = FORAGE, .direction = d});
			bee->target.row = -1;
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

command_t new_forage_route(t_cell_history grid[NUM_ROWS][NUM_COLS], t_bee *bee, agent_info_t info)
{
	if (bee->target.row >= 0)
	{
		return (best_waypoint_route_forager(grid, bee));
	}
	else
	{	
		if (is_aleph_bee_with_flower(grid[info.row][info.col].cell))
			bee->target = hive_coords(info.player);
		else
			find_forage_flower(bee, grid);
		if (bee->target.row < 0)
			return (best_explore_forage_route(grid, bee, info));
		return (best_waypoint_route_forager(grid, bee));
	}
}
