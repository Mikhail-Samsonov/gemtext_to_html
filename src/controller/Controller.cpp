#include "Controller.h"
#include "gmi_to_html.h"
#include "ThreadPool.h"

namespace fs = std::filesystem;

void Controller::check_input() const
{
	if (!fs::is_directory(_src_dir))
		throw std::runtime_error("Invalid source directory");

	if (fs::exists(_dst_dir))
	{
		fs::remove_all(_dst_dir);
		fs::create_directory(_dst_dir);
	}

	if (!fs::exists(_dst_dir))
		fs::create_directory(_dst_dir);
}

Controller::Controller(std::string_view src_dir, std::string_view dst_dir)
		: _src_dir(std::move(src_dir)), _dst_dir(std::move(dst_dir))
{
}

void Controller::do_work()
{
	check_input();

	for (auto const& dir_entry: fs::recursive_directory_iterator(_src_dir))
	{
		fs::path src_relative = fs::relative(dir_entry.path(), _src_dir);
		fs::path dst_absolete = _dst_dir / src_relative;

		// если директория - создать
		if (fs::is_directory(dir_entry.path()))
			fs::create_directory(dst_absolete);

		// просто файл
		if (fs::is_regular_file(dir_entry.path()))
		{
			// не .gmi - копировать
			if (dir_entry.path().extension() != ".gmi")
			{
				auto job = [=]()
				{
					fs::copy_file(dir_entry.path(), dst_absolete);
				};

				// не ждем пока скопируется
				// отсоединяем поток
				std::thread(job).detach();

				continue;
			}

			// .gmi - распарсить и сохранить
			ThreadPool workers{std::thread::hardware_concurrency()};

			auto job = [=]()
			{
				Document document{ dir_entry.path() };
				document.read();
				document.write_file(dst_absolete);
			};

			workers.enqueue(job);
		}
	}
};
