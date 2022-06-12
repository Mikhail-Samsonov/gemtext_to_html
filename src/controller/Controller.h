#ifndef GENERATOR_CONTROLLER_H
#define GENERATOR_CONTROLLER_H

#include <iostream>
#include "ThreadPool.h"
#include <filesystem>
#include <exception>
#include <memory>

class Controller
{
public:
	Controller(std::string_view src_dir, std::string_view dst_dir);

	/**
	 * Копирует файлики
	 * Запускает конвертер gmi_to_html
	 */
	void do_work();

private:
	/**
	 * Проверяет, что директория Source существует.
	 * Если Source Нет - кидает ошибку runtime_error.
	 * Если Destination нет - ничего не делает
	 * Если Destination есть - удаляет ее рекурсивно.
	 */
	void check_input() const;

	/**
	 * Путь к исходной папке
	 */
	std::filesystem::path _src_dir;

	/**
	 * Путь к папке назначения
	 */
	std::filesystem::path _dst_dir;
};

#endif //GENERATOR_CONTROLLER_H
