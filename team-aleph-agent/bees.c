#include "team-aleph.h"

command_t	choose_action(agent_info_t info, t_cell_history grid[NUM_ROWS][NUM_COLS], t_bees *bees)
{
	switch(bees->bees[info.bee].role) {
		case FORAGER:
			if (info.turn > TURNS_FORAGING && !is_aleph_bee_with_flower(grid[info.row][info.col].cell))
				bees->bees[info.bee].role = ATTACKER;
			else
				return (new_forage_route(grid, &bees->bees[info.bee], info));
		case ATTACKER:
			return (best_attack_route(grid, &bees->bees[info.bee], info, bees));
		default:
			return (best_scout_route(grid, &bees->bees[info.bee], info.player));
	}
}
