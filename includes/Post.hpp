/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Post.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tedelin <tedelin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 15:33:52 by tedelin           #+#    #+#             */
/*   Updated: 2023/10/04 11:36:31 by tedelin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POST_HPP
#define POST_HPP
#include "Response.hpp"

class Post : public Response {
	public:
		Post(Parsing* i_config, Request* i_request);
		~Post();

		void	createFile();
		
	private:
		Post();
		
};

#endif