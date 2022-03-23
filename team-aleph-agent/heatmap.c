#include "team-aleph.h"

void	print_heatmap(t_cell_history grid[NUM_ROWS][NUM_COLS], t_bees *bees)
{
	for (int row = 0; row < NUM_ROWS; row++)
	{
		for (int col = 0; col < NUM_COLS; col++)
		{
			if (coords_equal((coords_t) {.row = row, .col = col}, bees->foraging_target))
				printf(". ");
			else
				printf("%d ", grid[row][col].adjacents);
			fflush(stdout);
		}
		printf("\n");
	}
	printf("\n");

}

int		can_move(grid_cell_t grid)
{
	return(grid == EMPTY_ALEPH ||
			grid == EMPTY_TARGET ||
			grid == WALL_ALEPH ||
			grid == WALL_ENEMY ||
			grid == WALL_TARGET);
}

void	update_heatmap(t_cell_history grid[NUM_ROWS][NUM_COLS], int player, t_bees *bees)
{
	coords_t	temp_coord;
	for (int row = 0; row < NUM_ROWS; row++)
	{
		for (int col = 0; col < NUM_COLS; col++)
		{
			grid[row][col].adjacents = 0;
		}
	}
	for (int row = 0; row < NUM_ROWS; row++)
	{
		for (int col = 0; col < NUM_COLS; col++)
		{
			if (grid[row][col].cell == BEE_ENEMY || grid[row][col].cell == BEE_ENEMY_WITH_FLOWER)
			{
				grid[row][col].adjacents = 5;
				for (int d = 0; d < 8; d++)
				{
					temp_coord = direction_to_coords((coords_t) {.row = row, .col = col}, d);
					if (temp_coord.row < 0 || temp_coord.row >= NUM_ROWS
						|| temp_coord.col < 0 || temp_coord.col >= NUM_COLS)
						continue ;
					grid[temp_coord.row][temp_coord.col].adjacents++;
				}
			}
			else if (!can_move(grid[row][col].cell))
				grid[row][col].adjacents = 5;
		}
	}
	(void) player;
	(void) bees;
	//if (player == 0)
		//print_heatmap(grid, bees);
}
