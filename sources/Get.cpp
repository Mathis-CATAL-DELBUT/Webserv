/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Get.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcatal-d <mcatal-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 15:33:41 by tedelin           #+#    #+#             */
/*   Updated: 2023/09/25 17:58:51 by mcatal-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Get.hpp"

Get::Get(Parsing* config, Request* request) : Response(config, request) {
	name = "GET";
	std::string file_path = config->getRoot() + request->getValue("File");
	checkFile(file_path);
	setBody(file_path);
}

Get::~Get() {}