/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Get.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tedelin <tedelin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 15:33:41 by tedelin           #+#    #+#             */
/*   Updated: 2023/10/10 14:35:38 by tedelin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Get.hpp"

Get::Get(Parsing* config, Request* request) : Response(config, request) {
	name = "GET";
	std::string file_path = config->getRoot() + request->getValue("File");
	if (checkDirectory(file_path) == false) {
		checkFile(file_path);
		setBody(file_path);
	}
}

Get::~Get() {}