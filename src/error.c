/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wngamkri <wngamkri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/27 08:55:34 by wngamkri          #+#    #+#             */
/*   Updated: 2026/10/27 09:11:11 by wngamkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	exit_error(t_ctx *ctx)
{
	ft_putendl_fd("Error", 2);
	stack_free(&ctx->a);
	stack_free(&ctx->b);
	exit(1);
}

// this file just say error and end the process
