/*
 * Shell.h
 *
 *  Created on: 8 mars 2017
 *      Author: rubisoft
 */

#ifndef CORE_SHELL_H_
#define CORE_SHELL_H_

#include <map>
#include <vector>
#include <string>
#include <functional>


using ConstStrVect = const std::vector<std::string> ;
using OutputStream = std::ostream;
// Macro usefull for some calls of setCmd
#define member(member, memberOf) std::bind(member,memberOf, std::placeholders::_1, std::placeholders::_2)

class Shell {


	// use void instead of bool, exceptions are handling the error management stuff
	using FuncShell 	= std::function<void(const std::vector<std::string>&, std::ostream&)>;

	public:

	using Params = std::vector<std::string>;

	/*
	 * setCmd add a function to the command list.
	 *
	 * You can use it this way with non member functions (c functions):
	 *     setCmd("foo", foofunction);
	 * with foofunction declared this way:
	 *     void foofunction(const std::vector<std::string>& args, std::ostream& output)
	 *
	 * You can use it with anonymous functions this way:
	 *     setCmd("ping", [](const std::vector<std::string>&, std::ostream& output) {
	 *         output << "pong";
	 *     });
	 *
	 * You also may add a member function to the command list using the member() macro.
	 * Example beloww assumes that class Benchmark has a exit_from_shell method and that "shell"
	 * object has been created in a scope belonging to a Benchmark object. exit_from_shell must
	 * return void and expect two arguments: const std::vector<std::string>& and std::ostream&
	 *     shell.setCmd("exit", member(&Benchmark::exit_from_shell, this));
	 */
	void setCmd(const std::string& name, FuncShell function);
	void exit_from_shell(const std::vector<std::string>& args, std::ostream& output);
	void parse(const std::string& cmd, std::ostream& output);



	private:

	std::map<std::string, FuncShell> m_cmds;

};

#endif /* CORE_SHELL_H_ */
