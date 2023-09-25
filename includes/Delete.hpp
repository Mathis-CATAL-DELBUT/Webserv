/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Delete.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tedelin <tedelin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 15:33:39 by tedelin           #+#    #+#             */
/*   Updated: 2023/09/24 15:31:22 by tedelin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DELETE_HPP
#define DELETE_HPP
#include "Response.hpp"

class Delete : public Response {
	public:
		Delete(Parsing* i_config, Request* i_request);
		~Delete();

		void	delete_file(const std::string& file_path);
		
	private:
		Delete();
	
};

#endif