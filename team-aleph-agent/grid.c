#include "team-aleph.h"

void	update_grid(agent_info_t info, t_cell_history grid[NUM_ROWS][NUM_COLS],\
	t_bee bees[NUM_BEES])
{
	int temp_row;
	int temp_col;

	bees[info.bee].coords.row = info.row;
	bees[info.bee].coords.col = info.col;
	switch(info.bee) {
		case 0:

			grid[info.row][info.col].cell = BEE_ALEPH_0;
			break;
		case 1:
			grid[info.row][info.col].cell = BEE_ALEPH_1;
			break;
		case 2:
			grid[info.row][info.col].cell = BEE_ALEPH_2;
			break;
		case 3:
			grid[info.row][info.col].cell = BEE_ALEPH_3;
			break;
		case 4:
			grid[info.row][info.col].cell = BEE_ALEPH_4;
			break;
		default:
			break;
	}

	for (int row = 0; row < VIEW_SIZE; row++)
	{
		for (int col = 0; col < VIEW_SIZE; col++)
		{
			if (info.cells[row][col] != OUTSIDE)
			{

				temp_row = info.row - VIEW_DISTANCE + row;
				temp_col = info.col - VIEW_DISTANCE + col;
				grid[temp_row][temp_col].turn = info.turn;
				switch(info.cells[row][col]) {
					case EMPTY:
						if (grid[temp_row][temp_col].cell == WALL_TARGET)
							grid[temp_row][temp_col].cell = EMPTY_TARGET;
						else if (grid[temp_row][temp_col].cell == EMPTY_TARGET)
							;
						else
							grid[temp_row][temp_col].cell = EMPTY_ALEPH;
						break;
					case BEE_0:
						if (info.player == 0)
						{
							grid[temp_row][temp_col].cell = BEE_ALEPH;
						} else
						{
							grid[temp_row][temp_col].cell = BEE_ENEMY;
						}
						break;
					case BEE_1:
						if (info.player == 1)
						{
							grid[temp_row][temp_col].cell = BEE_ALEPH;
						} else
						{
							grid[temp_row][temp_col].cell = BEE_ENEMY;
						}
						break;
					case BEE_0_WITH_FLOWER:
						if (info.player == 0)
						{
							grid[temp_row][temp_col].cell = BEE_ALEPH_WITH_FLOWER;
						} else
						{
							grid[temp_row][temp_col].cell = BEE_ENEMY_WITH_FLOWER;
						}
						break;
					case BEE_1_WITH_FLOWER:
						if (info.player == 1)
						{
							grid[temp_row][temp_col].cell = BEE_ALEPH_WITH_FLOWER;
						} else
						{
							grid[temp_row][temp_col].cell = BEE_ENEMY_WITH_FLOWER;
						}
						break;
					case FLOWER:
						if (grid[temp_row][temp_col].cell != TARGET_FLOWER)
							grid[temp_row][temp_col].cell = FLOWER_ALEPH;
						break;
					case WALL:
						if (grid[temp_row][temp_col].cell != WALL_ALEPH &&
							grid[temp_row][temp_col].cell != WALL_TARGET)
							grid[temp_row][temp_col].cell = WALL_ENEMY;
						break;
					case HIVE_0:
						if (info.player == 0)
							grid[temp_row][temp_col].cell = HIVE_ALEPH;
						else
							grid[temp_row][temp_col].cell = HIVE_ENEMY;
						break;
					case HIVE_1:
						if (info.player == 1)
							grid[temp_row][temp_col].cell = HIVE_ALEPH;
						else
							grid[temp_row][temp_col].cell = HIVE_ENEMY;
						break;
					default:
						break;
				}
			}
		}
	}
	for (int row = 0; row < NUM_ROWS; row++)
	{
		for (int col = 0; col < NUM_COLS; col++)
		{
			if (grid[row][col].cell == MARKED_FOR_EXPLORATION)
				grid[row][col].cell = NO_INFO;
		}
	}
}
