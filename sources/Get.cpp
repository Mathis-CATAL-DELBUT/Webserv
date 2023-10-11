/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Get.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tedelin <tedelin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 15:33:41 by tedelin           #+#    #+#             */
/*   Updated: 2023/10/11 16:21:02 by tedelin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Get.hpp"

Get::Get(Parsing* config, Request* request) : Response(config, request) {
	name = "GET";
	std::string file_path = config->getRoot() + request->getValue("File");
	if (checkDirectory(request->getValue("File")) == false) {
		checkFile(file_path);
		setBody(file_path);
	}
}

Get::~Get() {}