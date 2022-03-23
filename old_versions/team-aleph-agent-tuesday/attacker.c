#include "team-aleph.h"

// static const coords_t attacker_targets[2][2] = {
// 	{{8, 24},
// 	{15, 24}},
// 	{{8, 6},
// 	{15, 6}}
// };

// coords_t	best_attack_target(int player, int index)
// {
// 	coords_t	best;

// 	if (index == 1)
// 		best = attacker_targets[player][0];
// 	else
// 		best = attacker_targets[player][1];
// 	// best_distance = NUM_COLS;
// 	// for (int t = 0; t < 2; t++)
// 	// {
// 	// 	temp_distance = distance_between_points(bee, attacker_targets[player][t]);
// 	// 	if (temp_distance < best_distance)
// 	// 	{
// 	// 		best_distance = temp_distance;
// 	// 		best = attacker_targets[player][t];
// 	// 	}
// 	// }
// 	return (best);
// }

command_t	best_waypoint_route_attacker(t_cell_history grid[NUM_ROWS][NUM_COLS], t_bee *bee)
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
			if (grid[temp_coord.row][temp_coord.col].cell == TARGET_FLOWER)
				return ((command_t){.action = GUARD, .direction = d});
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

// static const coords_t attacker_zones[2][2][2] = {
// 	{{{0, NUM_COLS / 2}, {12, NUM_COLS - 1}},
// 	{{12, NUM_COLS / 2}, {NUM_ROWS - 1, NUM_COLS - 1}}},
// 	{{{0, 0}, {12, NUM_COLS / 2 - 1}},
// 	{{12, 0}, {NUM_ROWS - 1, NUM_COLS / 2 - 1}}}
// };

void	find_attack_flower(t_bee *current_bee, \
	t_cell_history grid[NUM_ROWS][NUM_COLS], agent_info_t info)
{
	coords_t	best;
	int			best_distance;
	int			temp_distance;

	(void) info;
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


bool	no_info_in_attack_area(t_cell_history grid[NUM_ROWS][NUM_COLS], agent_info_t info)
{
	(void) info;
	for (int row = 0; row < NUM_ROWS; row++)
	{
		for (int col = 0; col < NUM_COLS; col++)
		{
			if (grid[row][col].cell == NO_INFO)
				return (true);
		}
	}
	return (false);
}

coords_t	find_explore_target(t_cell_history grid[NUM_ROWS][NUM_COLS], agent_info_t info)
{
	coords_t	temp_coord;
	int			best_info;
	int			temp_info;
	coords_t	best;

	best_info = 0;
	(void) info;

	for (int row = 0; row < NUM_ROWS; row++)
	{
		for (int col = 0; col < NUM_COLS; col++)
		{
			temp_coord.row = row;
			temp_coord.col = col;
			temp_info = get_info_from_coord(temp_coord, grid);
			if (temp_info > best_info)
			{
				best_info = temp_info;
				best = temp_coord;
			}
		}
	}
	return (best);
}

command_t	best_explore_route(t_cell_history grid[NUM_ROWS][NUM_COLS], t_bee *bee, agent_info_t info)
{
	coords_t	temp_coord;
	command_t	best;
	int			best_distance;
	int			temp_distance;
	bool		is_wall;
	coords_t	temp_target;

	temp_target = find_explore_target(grid, info);
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
			if (grid[temp_coord.row][temp_coord.col].cell == TARGET_FLOWER)
				return ((command_t){.action = GUARD, .direction = d});
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

command_t best_attack_route(t_cell_history grid[NUM_ROWS][NUM_COLS], t_bee *bee, agent_info_t info, t_bees *bees)
{

	if (bee->target.row >= 0)
	{
		return (best_waypoint_route_attacker(grid, bee));
	}
	else
		{
			find_attack_flower(bee, grid, info);
			if (bee->target.row < 0)
			{
				if (no_info_in_attack_area(grid, info))
					return (best_explore_route(grid, bee, info));
				else if (!no_flowers_in_forage_area(grid, bees->forage_distance, NUM_COLS * info.player, info.player))
				{
					bee->role = FORAGER;
					return (new_forage_route(grid, bee, info));
				}
				else
					return (best_scout_route(grid, &bees->bees[info.bee], info.player));
			}
			return (best_waypoint_route_attacker(grid, bee));
		}
}
