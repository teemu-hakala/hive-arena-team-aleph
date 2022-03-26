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
	coords_t	temp_forage_coord;
	coords_t	temp_bee_coord;
	int			best_adjacents;
	coords_t	best_forage_coord;

	(void) player;
	best_forage_coord.row = -1;
	best_forage_coord.col = -1;
	best_adjacents = 10;
	for (int d = 0; d < 8; d++)
	{
		temp_forage_coord = direction_to_coords(bees->bees[DEFENDER_BEE_INDEX].coords, d);
		if (temp_forage_coord.row < 0 || temp_forage_coord.row >= NUM_ROWS
			|| temp_forage_coord.col < 0 || temp_forage_coord.col >= NUM_COLS)
			continue ;
		for (int d1 = 0; d1 < 8; d1++)
		{
			temp_bee_coord = direction_to_coords(temp_forage_coord, d1);
			if (temp_bee_coord.row < 0 || temp_bee_coord.row >= NUM_ROWS
				|| temp_bee_coord.col < 0 || temp_bee_coord.col >= NUM_COLS)
				continue ;
			if (grid[temp_bee_coord.row][temp_bee_coord.col].adjacents == 0
				&& grid[temp_forage_coord.row][temp_forage_coord.col].adjacents < best_adjacents)
			{
				best_adjacents = grid[temp_forage_coord.row][temp_forage_coord.col].adjacents;
				best_forage_coord = temp_forage_coord;
			}
		}
	}
	bees->foraging_target = best_forage_coord;
}

command_t	best_defender_action(agent_info_t info, \
	t_cell_history grid[NUM_ROWS][NUM_COLS], \
	t_bees *bees)
{
	int		flower_direction;

	if (is_aleph_bee_with_flower(grid[info.row][info.col].cell))
	{
		if (info.player == 0)
			return ((command_t) {.action = FORAGE, .direction = NW});
		else
			return ((command_t) {.action = FORAGE, .direction = NE});
	}
	flower_direction = find_neighbour(FLOWER_ALEPH, grid, bees->bees[info.bee].coords);
	if (flower_direction >= 0)
		return ((command_t) {.action = FORAGE, .direction = flower_direction});
	update_heatmap(grid, info.player, bees);
	if (distance_between_points(bees->bees[FORAGER_BEE_INDEX].coords, \
		bees->foraging_target) == 1)
	{
		return ((command_t) {.action = GUARD, \
			.direction = direction_from_coords(bees->bees[info.bee].coords, \
			bees->foraging_target)});
	}
	update_foraging_target(grid, bees, info.player);
	return ((command_t) {.action = FORAGE, .direction = N});
}
