/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Get.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tedelin <tedelin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 15:33:43 by tedelin           #+#    #+#             */
/*   Updated: 2023/09/22 14:25:32 by tedelin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_HPP
#define GET_HPP
#include "Response.hpp"

class Get : public Response {
	public:
		Get(Parsing* i_config, Request* i_request);
		~Get();
		
	private:
		Get();

};

#endif