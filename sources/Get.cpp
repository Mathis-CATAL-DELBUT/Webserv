/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Get.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcatal-d <mcatal-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 15:33:41 by tedelin           #+#    #+#             */
/*   Updated: 2023/10/11 15:22:21 by mcatal-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Get.hpp"

Get::Get(Parsing* config, Request* request) : Response(config, request) {
	name = "GET";
	std::string path = request->getValue("File");
	std::string file_path = config->getRoot() + path;
	if (checkDirectory(path) == false) {
		std::cout << "file_path: " << config->getRoot() + path << std::endl;
		checkFile(config->getRoot() + path);
		setBody(config->getRoot() + path);
	}
}

Get::~Get() {}