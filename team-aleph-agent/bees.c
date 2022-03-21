#include "team-aleph.h"

command_t	choose_action(agent_info_t info, t_cell_history grid[NUM_ROWS][NUM_COLS], t_bees *bees)
{
	switch(bees->bees[info.bee].role) {
		case FORAGER:
			if (info.turn > TURNS_FORAGING && !is_aleph_bee_with_flower(grid[info.row][info.col].cell))
			{
				if (bees->builders < 3)
				{
					bees->bees[info.bee].target.row = -1;
					bees->bees[info.bee].role = BUILDER;
					bees->bees[info.bee].builder_index = bees->builders++;
					return (best_builder_action(info, grid, bees));
				}
				else
					bees->bees[info.bee].role = ATTACKER;
				return (best_attack_route(grid, &bees->bees[info.bee], info, bees));
			}
			else
				return (new_forage_route(grid, &bees->bees[info.bee], info));
		case ATTACKER:
			return (best_attack_route(grid, &bees->bees[info.bee], info, bees));
		case BUILDER:
			return (best_builder_action(info, grid, bees));
		default:
			return (best_scout_route(grid, &bees->bees[info.bee], info.player));
	}
}
