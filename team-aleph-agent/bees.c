#include "team-aleph.h"

command_t	choose_action(agent_info_t info, t_cell_history grid[NUM_ROWS][NUM_COLS], t_bees *bees)
{
	switch(bees->bees[info.bee].role) {
		case SCOUT:
			if ((info.player == 0 && bees->bees[info.bee].coords.col >= bees->forage_distance)
				|| (info.player == 1 && bees->bees[info.bee].coords.col < bees->forage_distance))
			{
				if (bees->attackers < 2)
				{
					bees->bees[info.bee].role = ATTACKER;
					bees->attackers++;
					return (best_scout_route(grid, bees->bees[info.bee].coords, info.player));
				}
				else
				{
					bees->bees[info.bee].role = FORAGER;
					return (best_forage_route(info, grid, bees));
				}
			}
			else
			{
				return (best_scout_route(grid, bees->bees[info.bee].coords, info.player));
			}
		case FORAGER:
			if ((((info.player == 0 && bees->bees[info.bee].coords.col < bees->forage_distance / 2)
				|| (info.player == 1 && bees->bees[info.bee].coords.col >= bees->forage_distance * 3 / 2))
				&& bees->hive_foragers < 1)
				|| no_flowers_in_forage_area(grid, bees->forage_distance, info.player))
			{
				bees->bees[info.bee].role = HIVE_FORAGER;
				bees->hive_foragers++;
				return (best_forage_route(info, grid, bees));
			}
			else
			{
				return (best_forage_route(info, grid, bees));
			}
		case HIVE_FORAGER:
			return (best_forage_route(info, grid, bees));
		case ATTACKER:
			return (best_scout_route(grid, bees->bees[info.bee].coords, info.player));
		case WAYPOINT:
			return (best_waypoint_route(grid, &bees->bees[info.bee]));
		default:
			return (best_scout_route(grid, bees->bees[info.bee].coords, info.player));
	}
}
