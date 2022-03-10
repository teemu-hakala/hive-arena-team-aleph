#include "agent.h"

static const coords_t offsets[] = {
    {-1, 0},
    {-1, 1},
    { 0, 1},
    { 1, 1},
    { 1, 0},
    { 1,-1},
    { 0,-1},
    {-1,-1}
};

int	get_info_from_coord(coords_t current, cell_t **grid)
{
	int	new_info;

	new_info = 0;
	for(int i; i < 8; i++)
	{
		if (grid[current.row + offsets[i].row][current.col + offsets[i].col] == NO_INFO)
			new_info++;
	}
	return (new_info);
}

command_t	best_scout_route(cell_t **grid, coords_t bee)
{
	command_t	best_command;
	dir_t		temp_direction;
	int			best_info;
	int			temp_info;

	best_command.action = MOVE;
	best_command.direction = 0;
	best_info = 0;
	
	for(int i; i < 8; i++)
	{
		temp_direction = i;
		temp_info = get_info_from_coord(direction_to_coords(bee, temp_direction), grid);
		if (temp_info > best_info)
		{
			best_command.direction = temp_direction;
			best_info = temp_info;
		}
	}
	return (best_command);
}