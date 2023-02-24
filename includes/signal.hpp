/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleferra <aleferra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 12:51:28 by lfilloux          #+#    #+#             */
/*   Updated: 2023/02/23 16:42:16 by aleferra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNAL_HPP
# define SIGNAL_HPP

#include <iostream>
#include <string>
#include "serv.hpp"

void setSignal(void);
void sigNcHandler(int sig, siginfo_t *info, void *context);

#endif