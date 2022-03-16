#include "team-aleph.h"

static const coords_t attacker_targets[2][2] = {
	{{15, 24},
	{8, 24}},
	{{8, 6},
	{15, 6}}
};

coords_t	best_attack_target(coords_t bee, int player)
{
	coords_t	best;
	int			best_distance;
	int			temp_distance;

	best_distance = NUM_COLS;
	for (int t = 0; t < 2; t++)
	{
		temp_distance = distance_between_points(bee, attacker_targets[player][t]);
		if (temp_distance < best_distance)
		{
			best_distance = temp_distance;
			best = attacker_targets[player][t];
		}
	}
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
		if (grid[temp_coord.row][temp_coord.col].cell != EMPTY_ALEPH)
			continue ;
		temp_distance = distance_between_points(temp_coord, bee->target);
		if (temp_distance == 0)
		{
			bee->target.row = -1;
			return ((command_t){.action = MOVE, .direction = d});
		}
		if (temp_distance < best_distance)
		{
			best_distance = temp_distance;
			best.action = MOVE;
			best.direction = d;
		}
	}
	return (best);
}

command_t best_attack_route(t_cell_history grid[NUM_ROWS][NUM_COLS], t_bee *bee, int player)
{
	command_t	best_command;
	dir_t		temp_direction;
	int			best_info;
	int			temp_info;
	coords_t			temp_coord;

	if (bee->target.row >= 0)
	{
		return (best_waypoint_route_attacker(grid, bee));
	}

	best_command.action = MOVE;
	best_command.direction = 0;
	best_info = 0;

	for(int i = 0; i < 8; i++)
	{
		temp_direction = i;
		temp_coord = direction_to_coords(bee->coords, temp_direction);
		if (temp_coord.row < 0 || temp_coord.row >= NUM_ROWS
			|| temp_coord.col < 0 || temp_coord.col >= NUM_COLS
			|| grid[temp_coord.row][temp_coord.col].cell != EMPTY_ALEPH)
			continue;
		temp_info = get_info_from_coord(temp_coord, grid);
		if (temp_info > best_info)
		{
			best_command.direction = temp_direction;
			best_info = temp_info;
		}
	}
	if (best_info == 0)
	{
		if (player == 0)
			best_command.direction = E;
		else
			best_command.direction = W;
	}
	return (best_command);
}
