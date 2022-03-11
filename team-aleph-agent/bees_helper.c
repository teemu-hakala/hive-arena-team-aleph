#include "team-aleph.h"

void	initialize_bees(t_bee bees[NUM_BEES])
{
	for (int b = 0; b < NUM_BEES; b++)
	{
		bees[b].role = FORAGER;//SCOUT;
		bees[b].target.row = -1;
	}
}

int	distance_between_points(coords_t coord0, coords_t coord1)
{
	if (abs(coord0.row - coord1.row) > abs(coord0.col - coord1.col))
		return (abs(coord0.row - coord1.row));
	else
		return (abs(coord0.col - coord1.col));
}

bool	is_aleph_bee_with_flower(grid_cell_t bee)
{
	return (bee >= BEE_ALEPH_WITH_FLOWER && bee <= BEE_ALEPH_4_WITH_FLOWER);
}

coords_t	hive_coords(int player)
{
	if (player == 0)
		return ((coords_t){.row = NUM_ROWS / 2, .col = 1});
	else
		return ((coords_t){.row = NUM_ROWS / 2, .col = NUM_COLS - 2});
}
