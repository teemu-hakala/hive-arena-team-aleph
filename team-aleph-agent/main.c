#include "team-aleph.h"

command_t think(agent_info_t info)
{
	coords_t bee_coords;
	static t_cell_history	grid[NUM_ROWS][NUM_COLS];
	static t_bees			bees;
	command_t				command;

	if (info.turn == 0 || info.turn == 1)
	{
		bees.forage_distance = get_forage_distance(info.player);
		initialize_bees(bees.bees, info);
		create_stacks(grid, info.player, &bees);
	}

	bee_coords.row = info.row;
	bee_coords.col = info.col;
	if (!coords_equal(bees.bees[info.bee].previous_position, bees.bees[info.bee].previous_turn))
		bees.bees[info.bee].previous_position = bees.bees[info.bee].previous_turn;
	bees.bees[info.bee].previous_turn = bee_coords;
	printf("bee %d prev pos %d %d, prev turn %d %d, ccurrent %d %d\n", info.bee, bees.bees[info.bee].previous_position.row, bees.bees[info.bee].previous_position.col, bees.bees[info.bee].previous_turn.row, bees.bees[info.bee].previous_turn.col, info.row, info.col);
	update_grid(info, grid, bees.bees);
	// printf("bee %d role %d\n", info.bee, bees.bees[info.bee].role);
	command = choose_action(info, grid, &bees);
	return (command);
}

int main(int argc, char **argv)
{
    if (argc < 3)
        panic("Usage: ./agent arena_host arena_ip");

    srand(time(NULL) + getpid());

    char *host = argv[1];
    int port = atoi(argv[2]);
    char *team_name = "team_aleph_agent";

    agent_main(host, port, team_name, think);
}
