#include "team-aleph.h"

command_t	choose_action(agent_info_t info, t_cell_history grid[NUM_ROWS][NUM_COLS], t_bees *bees)
{
	switch(bees->bees[info.bee].role) {
		case FORAGER:
			return (new_forage_route(grid, &bees->bees[info.bee], info, bees));
		case BUILDER:
			return (best_builder_action(info, grid, bees));
		case DEFENDER:
			return (best_defender_action(info, grid, bees));
		default:
			return (new_forage_route(grid, &bees->bees[info.bee], info, bees));
	}
}
