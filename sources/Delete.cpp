/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Delete.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tedelin <tedelin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 15:33:37 by tedelin           #+#    #+#             */
/*   Updated: 2023/09/24 15:31:13 by tedelin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Delete.hpp"

Delete::Delete(Parsing* config, Request* request) : Response(config, request) {
	name = "DELETE";
	std::string file_path = config->getRoot() + request->getValue("File");
	checkFile(file_path);
	delete_file(file_path);
	
}

void	Delete::delete_file(const std::string& file_path) {
	if (status == 200) {
		remove(file_path.c_str());
	}
}

Delete::~Delete() {}