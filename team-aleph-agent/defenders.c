#include "team-aleph.h"

coords_t	defender_target(int player)
{
	if (player == 0)
		return ((coords_t) {.row = 13, .col = 2});
	else
		return ((coords_t) {.row = 13, .col = 27});
}

void	update_foraging_target(t_cell_history grid[NUM_ROWS][NUM_COLS], \
	t_bees *bees, int player)
{
	(void) grid;
	if (player == 0)
	{
		bees->foraging_target.row = 14;
		bees->foraging_target.col = 2;
	}
	else
	{
		bees->foraging_target.row = 14;
		bees->foraging_target.col = 27;
	}
}

command_t	best_waypoint_route_defender(t_cell_history grid[NUM_ROWS][NUM_COLS], t_bee *bee)
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

command_t	best_defender_action(agent_info_t info, \
	t_cell_history grid[NUM_ROWS][NUM_COLS], \
	t_bees *bees)
{
	int		flower_direction;

	if (info.turn < DEFENDER_FORAGE_TURNS)
		return (new_forage_route(grid, &bees->bees[info.bee], info));
	bees->bees[info.bee].target = defender_target(info.player);
	if (coords_equal(bees->bees[info.bee].coords, bees->bees[info.bee].target))
	{
		if (is_aleph_bee_with_flower(grid[info.row][info.col].cell))
		{
			if (info.player == 0)
				return ((command_t) {.action = FORAGE, .direction = NW});
			else
				return ((command_t) {.action = FORAGE, .direction = NE});
		}
		flower_direction = -1; //flower_in_reach(grid, bees->bees[info.bee].coords);
		if (flower_direction >= 0)
			return ((command_t) {.action = FORAGE, .direction = flower_direction});
		//update_heatmap(grid);
		update_foraging_target(grid, bees, info.player);
		if (distance_between_points(bees->bees[FORAGER_BEE_INDEX].coords, \
			bees->foraging_target) == 1)
		{
			bees->guard_attempts--;
			return ((command_t) {.action = GUARD, \
				.direction = direction_from_coords(bees->bees[info.bee].coords, \
				bees->foraging_target)});
		}
	}
	else
	{
		return (best_waypoint_route_defender(grid, &bees->bees[info.bee]));
	}
	return ((command_t) {.action = FORAGE, .direction = N});
}
