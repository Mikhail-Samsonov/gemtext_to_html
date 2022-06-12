#ifndef GENERATOR_GMI_TO_HTML_H
#define GENERATOR_GMI_TO_HTML_H

#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>
#include <regex>

#include "Preformatted.h"
#include "Link.h"
#include "List.h"
#include "Head.h"
#include "Blockquotes.h"
#include "Text.h"

/**
 * Класс, который работает с файликами gemtext
 */
class Document
{
public:
	Document() = delete;

	explicit Document(std::filesystem::path file);

	/**
	 * Считать файл gemtext
	 */
	void read();

	/**
	 * Перевести gtmtext в html и создать в новой директории
	 * @param file
	 */
	void write_file(const std::filesystem::path& file);

private:

	/**
	 * Запуск интерпретатора
	 */
	void interpretate();

	/**
	 * итирование
	 * @param s строка, в которой находится цитата
	 */
	void iblockquotes(std::string_view s);

	/**
	 * Заголовок
	 * @param s строка, в которой находится заголовок
	 */
	void ihead(std::string_view s);

	/**
	 * Ссылка
	 * @param s строка, в которой находится ссылка
	 */
	void ilink(std::string_view s);

	/**
	 * GEM_TO_HTML: список
	 * @param v Документ
	 * @param pos Позиция, с которой начинается просмотр
	 * @return кол-во строк, которые надо пропустить, поскольку их обработали в другой процедуре
	 */
	std::size_t ilist(std::vector<std::unique_ptr<IBaseline>>& v, std::size_t pos);

	/**
	 * GEM_TO_HTML: предформатированный блок
	 * @param v Документ
	 * @param pos Позиция, с которой начинается просмотр
	 * @return кол-во строк, которые надо пропустить, поскольку их обработали в другой процедуре
	 */
	std::size_t ipreformatted(std::vector<std::unique_ptr<IBaseline>>& v, std::size_t pos);

	/**
	 * процедура перевода в htmp текстовой строки
	 * @param s исходныя строка gemtext
	 */
	void itext(std::string_view s);

	/**
	 * Записать html в файл
	 * @param s
	 */
	void print_file(const std::filesystem::path& s);

	std::vector<std::string> _html;

	std::filesystem::path _filepath = "";

	// представление документа как набора строк
	// под каждый вид строки есть свой объект
	std::vector<std::unique_ptr<IBaseline>> _document;

	// Regex рапознавания вида строк
	const std::regex _head{ "^(#{1,3})\\s*(.*)" };
	const std::regex _preformatted{ "^```(.*)?" };
	const std::regex _list{ "^\\*\\s*(.*)" };
	const std::regex _blockquotes{ "^>\\s*(.*)" };
	const std::regex _link{ "^=>\\s+(\\S+)\\s*(.*)" };

	void add_line(std::unique_ptr<IBaseline> line)
	{
		_document.emplace_back(std::move(line));
	}

	void do_read()
	{
		std::string line;
		std::ifstream file(_filepath);
		std::vector<std::string> vec;

		while (std::getline(file, line, '\n'))
		{
			// сверяем с regex
			// запихиваем соответствующий объект в document

			if (std::regex_match(line, _preformatted))
				add_line(std::make_unique<Preformatted>(line));
			else if (std::regex_match(line, _head))
				add_line(std::make_unique<Head>(line));
			else if (std::regex_match(line, _list))
				add_line(std::make_unique<List>(line));
			else if (std::regex_match(line, _blockquotes))
				add_line(std::make_unique<Blockquotes>(line));
			else if (std::regex_match(line, _link))
				add_line(std::make_unique<Link>(line));
			else
				add_line(std::make_unique<Text>(line));
		}
	}
};

#endif