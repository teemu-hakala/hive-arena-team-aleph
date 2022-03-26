/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builders.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsamoilo <nsamoilo@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 17:28:30 by jraivio           #+#    #+#             */
/*   Updated: 2022/03/24 17:59:29 by nsamoilo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "team-aleph.h"

static coords_t		get_target(int player, t_bee bee)
{
	coords_t	target;

	target = hive_coords(player ? 0 : 1);
	target.row += builder_offsets[bee.builder_index].row;
	target.col += builder_offsets[bee.builder_index].col * (player ? 1 : -1);
	return (target);
}

static command_t	get_best_move(t_cell_history grid[NUM_ROWS][NUM_COLS], \
	t_bees *bees, agent_info_t info)
{
	coords_t	temp_coord;
	command_t	best;
	
	best = find_path(grid, bees, info, bees->bees[info.bee].target);
	temp_coord = direction_to_coords(bees->bees[info.bee].coords, best.direction);
	if (is_grid_wall(grid[temp_coord.row][temp_coord.col].cell))
	{
		best.action = GUARD;
		grid[temp_coord.row][temp_coord.col].cell = WALL_TARGET;
	}
	return (best);
}

static command_t	build_wall(t_cell_history grid[NUM_ROWS][NUM_COLS], t_bee bee)
{
	grid_cell_t	targets[8];
	grid_cell_t	action_targets[] = {EMPTY_ALEPH, FLOWER_ALEPH, TARGET_FLOWER, BEE_ENEMY_WITH_FLOWER, WALL_TARGET};
	action_t	actions[] = {BUILD, GUARD, GUARD, GUARD, GUARD};
	coords_t	temp_coords;

	for (int d = 0; d < 8; d++)
	{
		temp_coords = direction_to_coords(bee.coords, d);
		targets[d] = grid[temp_coords.row]
						[temp_coords.col].cell;
	}
	for (int i = 0; i < 5; i++)
	{
		for (int d = 0; d < 8; d++)
		{
			if (targets[d] == action_targets[i])
			{
				if(actions[i] == BUILD)
					{
						temp_coords = direction_to_coords(bee.coords, d);
						grid[temp_coords.row][temp_coords.col].cell = WALL_ALEPH;
					}
				return ((command_t){.action = actions[i], .direction = d});
			}
		}
	}
	return ((command_t){.action = BUILD, .direction = 0});
}

command_t	best_builder_action(agent_info_t info, \
	t_cell_history grid[NUM_ROWS][NUM_COLS], \
	t_bees *bees)
{
	dir_t	temp_dir;
	if (bees->bees[info.bee].target.row < 0)
		bees->bees[info.bee].target = get_target(info.player, bees->bees[info.bee]);
	if (distance_between_points(bees->bees[info.bee].coords, bees->bees[info.bee].target))
	{
		temp_dir = direction_from_coords(bees->bees[info.bee].coords, bees->bees[info.bee].target);
		if (distance_between_points(bees->bees[info.bee].coords, bees->bees[info.bee].target) == 1)
		{
			if (grid[bees->bees[info.bee].target.row][bees->bees[info.bee].target.col].cell == EMPTY_ALEPH \
				|| grid[bees->bees[info.bee].target.row][bees->bees[info.bee].target.col].cell == EMPTY_TARGET)
				return((command_t) {.action = MOVE, .direction = temp_dir});
			else if (is_grid_wall(grid[bees->bees[info.bee].target.row][bees->bees[info.bee].target.col].cell))
			{
				grid[bees->bees[info.bee].target.row][bees->bees[info.bee].target.col].cell = WALL_TARGET;
				return((command_t) {.action = GUARD, .direction = direction_from_coords(bees->bees[info.bee].coords, bees->bees[info.bee].target)});
			}
			else
				return(build_wall(grid, bees->bees[info.bee]));
		}
		return(get_best_move(grid, bees, info));
	}
	else
		return(build_wall(grid, bees->bees[info.bee]));
}
