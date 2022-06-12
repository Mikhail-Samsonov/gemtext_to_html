#ifndef GENERATOR_PROGRAM_OPTIONS_H
#define GENERATOR_PROGRAM_OPTIONS_H

#include <boost/program_options.hpp>
#include <iostream>
#include <stdexcept>

struct Context
{
	std::string src;
	std::string dst;
};

struct Help : public std::exception
{
};

struct Error : public std::exception
{
};

/**
 * Парсинг аргуметов командной строки
 * @param argc
 * @param argv
 * @return Структура с абсолютными путями src и dst
 */
Context parse_command_options(int argc, char** argv)
{
	namespace po = boost::program_options;

	po::options_description opt_desc("Allowed options");
	opt_desc.add_options()
			("help", "Print this message")
			("src", po::value<std::string>()->required(), "SourceDirectory")
			("dst", po::value<std::string>()->required(), "DestinationDirectory");

	po::variables_map var_map;
	try
	{
		auto parsed = po::command_line_parser(argc, argv)
				.options(opt_desc)
				.run();
		po::store(parsed, var_map);
		if (var_map.count("help") != 0)
		{
			std::cout << opt_desc << "\n";
			throw Help{};
		}
		po::notify(var_map);
	}
	catch (const po::error& error)
	{
		std::cerr << "Error while parsing command-line arguments: "
				  << error.what() << "\nPlease use --help to see help message\n";
		throw Error{};
	}

	std::string src = var_map["src"].as<std::string>();

	std::string dst = var_map["dst"].as<std::string>();

	return Context{ src, dst };
}

#endif //GENERATOR_PROGRAM_OPTIONS_H