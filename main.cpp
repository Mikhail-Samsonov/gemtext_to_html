#include <iostream>
#include "program_options.h"
#include "Controller.h"

int main(int argc, char* argv[])
{
	Context context;
	try
	{
		context = parse_command_options(argc, argv);

		Controller controller(context.src, context.dst);
		controller.do_work();
	}
	catch (const Help&)
	{
		return EXIT_SUCCESS;
	}
	catch (const Error&)
	{
		return EXIT_FAILURE;
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << '\n';
		return EXIT_FAILURE;
	}
}