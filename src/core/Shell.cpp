/*
 * Shell.cpp
 *
 *  Created on: 8 mars 2017
 *      Author: rubisoft
 */

#include <iostream>
#include <core/Shell.h>



void Shell::setCmd(const std::string& name, FuncShell function) {
	m_cmds[name] = function;
	std::cout << "Add function '" << name << "'" << std::endl;
}

void Shell::parse(const std::string& cmd, std::ostream& output) {
	std::size_t i=0;
	std::vector<std::string> accu;

	bool escape=false,
		 escape_space=false;

	// Explode string
	for(std::size_t j=0; j<cmd.size(); ++j) {
		if (cmd[j] == ' ' && !escape_space) {
			if (i==j) {
				++i; escape = false; continue;
			}
			accu.push_back(cmd.substr(i,j-i));
			i = j+1;
		}
		else if (cmd[j] == '\'' && !escape) {
			if (escape_space) {
				accu.push_back(cmd.substr(i+1, j-i-1));
				escape_space = false;
				i = j+1;
			} else escape_space = true;
		}

		escape = false;
		if (cmd[j] == '\\') escape = true;
	}
	if (i!=cmd.size())
		accu.push_back(cmd.substr(i,cmd.size()-i));

	// Parse content
	auto func = m_cmds.find(accu[0]);
	if(func != m_cmds.end())
		func->second(accu, output);
	else
		output << "Command not found : '" << accu[0] << "'" << std::endl;
}


