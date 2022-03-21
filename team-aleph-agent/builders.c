/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builders.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsamoilo <nsamoilo@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 17:28:30 by jraivio           #+#    #+#             */
/*   Updated: 2022/03/21 20:55:51 by nsamoilo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "team-aleph.h"

static coords_t		get_target(int player, t_bees bees)
{
	int builder_count;
	int			i;
	coords_t	target;

	i = 0;
	builder_count = 0;
	while (i < 5)
		if (bees.bees[i++].role == BUILDER)
			builder_count++;
	target = hive_coords(player ? 0 : 1);
	target.row += builder_offsets[builder_count - 1].row;
	target.col += builder_offsets[builder_count - 1].col/* * (player ? 1 : -1)*/;
	return (target);
}

static command_t	get_best_move(t_cell_history grid[NUM_ROWS][NUM_COLS], t_bee bee)
{
	coords_t	temp_coord;
	command_t	best;
	int			best_distance;
	int			temp_distance;
	bool		is_wall;

	is_wall = false;
	best_distance = NUM_COLS;
	for (int d = 0; d < 8; d++)
	{
		temp_coord = direction_to_coords(bee.coords, d);
		if (temp_coord.row < 0 || temp_coord.row >= NUM_ROWS ||
			temp_coord.col < 0 || temp_coord.col >= NUM_COLS)
			continue ;
		temp_distance = distance_between_points(temp_coord, bee.target);
		if (grid[temp_coord.row][temp_coord.col].cell != EMPTY_ALEPH/* || 
			is_grid_wall(grid[temp_coord.row][temp_coord.col].cell)*/)
			continue ;
		if (temp_distance < best_distance ||
			(temp_distance == best_distance && 
			is_wall))
		{
			best_distance = temp_distance;
			best.action = MOVE;
			best.direction = d;
			if (is_grid_wall(grid[temp_coord.row][temp_coord.col].cell))
				is_wall = true;
			else
				is_wall = false;
		}
	}
	if (is_wall)
		best.action = GUARD;
	return (best);
}

static command_t	build_wall(t_cell_history grid[NUM_ROWS][NUM_COLS], t_bee bee)
{
	grid_cell_t	targets[8];
	grid_cell_t	action_targets[] = {EMPTY_ALEPH, FLOWER_ALEPH, BEE_ENEMY_WITH_FLOWER};
	action_t	actions[] = {BUILD, GUARD, GUARD};

	for (int d = 0; d < 8; d++)
	{
		targets[d] = grid[direction_to_coords(bee.coords, d).row]
						[direction_to_coords(bee.coords, d).col].cell;
	}
	for (int i = 0; i < 3; i++)
	{
		for (int d = 0; d < 8; d++)
			{
			if (targets[d] == action_targets[i])
				return ((command_t){.action = actions[i], .direction = d});
			}
	}
	return ((command_t){.action = BUILD, .direction = 0});
}

command_t	best_builder_action(agent_info_t info, \
	t_cell_history grid[NUM_ROWS][NUM_COLS], \
	t_bees *bees)
{
	if (bees->bees[info.bee].target.row < 0)
		bees->bees[info.bee].target = get_target(info.player, *bees);
	if (!(bees->bees[info.bee].target.row == bees->bees[info.bee].coords.row &&
		bees->bees[info.bee].target.col == bees->bees[info.bee].coords.col))
		return(get_best_move(grid, bees->bees[info.bee]));
	else
		return(build_wall(grid, bees->bees[info.bee]));
}
