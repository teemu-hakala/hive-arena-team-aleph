#include "team-aleph.h"

static const coords_t attacker_targets[2][2] = {
	{{8, 24},
	{15, 24}},
	{{8, 6},
	{15, 6}}
};

coords_t	best_attack_target(int player, int index)
{
	coords_t	best;

	if (index == 1)
		best = attacker_targets[player][0];
	else
		best = attacker_targets[player][1];
	// best_distance = NUM_COLS;
	// for (int t = 0; t < 2; t++)
	// {
	// 	temp_distance = distance_between_points(bee, attacker_targets[player][t]);
	// 	if (temp_distance < best_distance)
	// 	{
	// 		best_distance = temp_distance;
	// 		best = attacker_targets[player][t];
	// 	}
	// }
	return (best);
}

command_t	best_waypoint_route_attacker(t_cell_history grid[NUM_ROWS][NUM_COLS], t_bee *bee)
{
	coords_t	temp_coord;
	command_t	best;
	int			best_distance;
	int			temp_distance;

	best_distance = NUM_COLS;
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

static const coords_t attacker_zones[2][2][2] = {
	{{{0, NUM_COLS / 2}, {12, NUM_COLS - 1}},
	{{12, NUM_COLS / 2}, {NUM_ROWS - 1, NUM_COLS - 1}}},
	{{{0, 0}, {12, NUM_COLS / 2 - 1}},
	{{12, 0}, {NUM_ROWS - 1, NUM_COLS / 2 - 1}}}
};

void	find_attack_flower(t_bee *current_bee, \
	t_cell_history grid[NUM_ROWS][NUM_COLS], agent_info_t info)
{
	coords_t	best;
	int			best_distance;
	int			temp_distance;
	int			attacker;

	if (info.bee == 1)
		attacker = 0;
	else
		attacker = 1;

	best_distance = NUM_COLS;
	best.row = -1;
	best.col = -1;
	for (int row = attacker_zones[info.player][attacker][0].row; row <= attacker_zones[info.player][attacker][1].row; row++)
	{
		for (int col = attacker_zones[info.player][attacker][0].col; col <= attacker_zones[info.player][attacker][1].col; col++)
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
				bee->role = FORAGER;
				return (best_forage_route(info, grid, bees));
			}
			return (best_waypoint_route_attacker(grid, bee));
		}
	// best_command.action = MOVE;
	// best_command.direction = 0;
	// best_info = 0;

	// for(int i = 0; i < 8; i++)
	// {
	// 	temp_direction = i;
	// 	temp_coord = direction_to_coords(bee->coords, temp_direction);
	// 	if (temp_coord.row < 0 || temp_coord.row >= NUM_ROWS
	// 		|| temp_coord.col < 0 || temp_coord.col >= NUM_COLS
	// 		|| grid[temp_coord.row][temp_coord.col].cell != EMPTY_ALEPH)
	// 		continue;
	// 	temp_info = get_info_from_coord(temp_coord, grid);
	// 	if (temp_info > best_info)
	// 	{
	// 		best_command.direction = temp_direction;
	// 		best_info = temp_info;
	// 	}
	// }
	// if (best_info == 0)
	// {
	// 	if (player == 0)
	// 		best_command.direction = E;
	// 	else
	// 		best_command.direction = W;
	// }
	// return (best_command);
}
