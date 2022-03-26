#include "team-aleph.h"

command_t think(agent_info_t info)
{
	static t_cell_history	grid[NUM_ROWS][NUM_COLS];
	static t_bees			bees;

	if (info.turn == 0 || info.turn == 1)
	{
		initialize_bees(&bees);
		bees.builders = 0;
		bees.foraging_target = (coords_t){.row = -1, .col = -1};
	}

	update_grid(info, grid, bees.bees);
	return (choose_action(info, grid, &bees));
}

int main(int argc, char **argv)
{
    if (argc < 3)
        panic("Usage: ./agent arena_host arena_ip");

    srand(time(NULL) + getpid());

    char *host = argv[1];
    int port = atoi(argv[2]);
    char *team_name = "Team ALEPH";

    agent_main(host, port, team_name, think);
}
