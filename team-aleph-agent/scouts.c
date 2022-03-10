#include "team-aleph.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

// static const coords_t offsets[] = {
//     {-1, 0},
//     {-1, 1},
//     { 0, 1},
//     { 1, 1},
//     { 1, 0},
//     { 1,-1},
//     { 0,-1},
//     {-1,-1}
// };

int	get_info_from_coord(coords_t current, t_cell_history grid[NUM_ROWS][NUM_COLS])
{
	// int fd = open("trace3.txt", O_CREAT|O_WRONLY|O_APPEND, 0644);
	int	new_info;
	int temp_row;
	int temp_col;

	new_info = 0;
	for(int j = -VIEW_DISTANCE; j <= VIEW_DISTANCE; j++)
	{
		for(int i = -VIEW_DISTANCE; i <= VIEW_DISTANCE; i++)
		{
			{
				temp_row = current.row + j;
				temp_col = current.col + i;
				if (temp_row >= 0 && temp_row < NUM_ROWS && temp_col >= 0 && temp_col < NUM_COLS && grid[temp_row][temp_col].cell == NO_INFO)
				{
					new_info++;
				}
			}
		}
	}
	// dprintf(fd, "info %d\n", new_info);
	return (new_info);
}

command_t	best_scout_route(t_cell_history grid[NUM_ROWS][NUM_COLS], coords_t bee)
{
	command_t	best_command;
	dir_t		temp_direction;
	int			best_info;
	int			temp_info;
	coords_t			temp_coord;

	best_command.action = MOVE;
	best_command.direction = 0;
	best_info = 0;
	
	for(int i = 0; i < 8; i++)
	{
		temp_direction = i;
		temp_coord = direction_to_coords(bee, temp_direction);
		if (grid[temp_coord.row][temp_coord.col].cell != EMPTY_ALEPH)
			continue;
		temp_info = get_info_from_coord(temp_coord, grid);
		if (temp_info > best_info)
		{
			best_command.direction = temp_direction;
			best_info = temp_info;
		}
	}
	if (best_info == 0)
		best_command.direction = rand() % 8;
	return (best_command);
}