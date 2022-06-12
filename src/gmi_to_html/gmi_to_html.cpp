#include "gmi_to_html.h"

Document::Document(std::filesystem::path file)
		: _filepath{ std::move(file) }
{
}

void Document::read()
{
	do_read();
}

void Document::print_file(const std::filesystem::path& s)
{
	std::ofstream ofs(s);

	for (const auto& i: _html)
	{
		ofs << i << '\n';
	}

	ofs.close();
}

void Document::write_file(const std::filesystem::path& file)
{
	if (_document.empty())
	{
		// просто создать файл
		print_file(file);
	}
	else
	{
		// интерпретировать и создать файл
		interpretate();
		print_file(file);
	}
}

void Document::interpretate()
{
	std::size_t i = 0;
	Linetipe next, prev, current = Linetipe::null;

	do
	{
		if (i + 1 < _document.size())
			next = _document[i + 1]->linetype();

		current = _document[i]->linetype();


		if (current == Linetipe::head)
		{
			ihead(_document[i]->data);
		}
		else if (current == Linetipe::list)
		{
			_html.push_back("<ul>");
			if (next != Linetipe::null)
			{
				i = i + ilist(_document, i);
			}
			_html.push_back("</ul>");
		}
		else if (current == Linetipe::text)
		{
			itext(_document[i]->data);
		}
		else if (current == Linetipe::link)
		{
			ilink(_document[i]->data);
		}
		else if (current == Linetipe::preformatted)
		{
			_html.push_back("<pre>");
			if (next != Linetipe::null)
			{
				i = i + ipreformatted(_document, i);
			}
			_html.push_back("</pre>");
		}
		else if (current == Linetipe::blockquotes)
		{
			iblockquotes(_document[i]->data);
		}
		else
			throw std::logic_error("undefined Linetype option");

		prev = current;
		i++;
	} while (i < _document.size());

	_html.push_back("<br/>");
}

void Document::ihead(std::string_view s)
{
	// h1,h2,h3 смотрим по числу # в начале строки
	int count = 1;
	for (int i = 1; (i < 3) && (i < std::strlen(s.data())); i++)
	{
		if (s[i] != '#')
			break;
		count++;
	}

	const char* tag;
	const char* indent;

	switch (count)
	{
	case 1:
		tag = "h1";
		indent = "";
		break;
	case 2:
		tag = "h2";
		indent = " ";
		break;
	case 3:
		tag = "h3";
		indent = "  ";
		break;
	};

	std::string str;

	str.append(indent);
	str.append("<");
	str.append(tag);
	str.append(">");

	str.append(s, count, s.length() - count);

	str.append("</");
	str.append(tag);
	str.append(">");

	_html.push_back(str);
}

void Document::ilink(std::string_view s)
{
	std::string str;

	// с 3 позиции ищем первый пробел ( ну или конец строки, если пробела нет )
	std::string link;
	std::string description;


	std:size_t pos = s.find_first_of(" ",3);

	if(pos != std::string::npos)
	{
		link = s.substr(3, pos - 3);
		description = s.substr(pos + 1, s.length() - pos);
	}  else
	{
		link = s;
		description = "";
	}

	str.append("<a href=");
	str.append(link);
	str.append(">");
	str.append(description);
	str.append("</a>");

	_html.push_back(str);
}

void Document::itext(std::string_view s)
{
	std::string str;

	str.append("<p>");
	str.append(s);
	str.append("</p>");
	_html.push_back(str);

}

std::size_t Document::ilist(std::vector<std::unique_ptr<IBaseline>>& v, std::size_t pos)
{
	int i = 0;
	while ((pos + i < v.size()) && (v[pos + i]->linetype() != Linetipe::list) )
	{
		std::string str;
		str.append("  <li>");
		str.append(v[pos + i]->data, 1, v[pos + i]->data.length() - 1);
		str.append("</li>");
		_html.push_back(str);
		i++;
	}

	return i;
}

std::size_t Document::ipreformatted(std::vector<std::unique_ptr<IBaseline>>& v, std::size_t pos)
{
	int i = 1;
	while ((pos + i < v.size()) && (v[pos + i]->linetype() != Linetipe::preformatted) )
	{
		std::string str;
		str.append("  ");
		str.append(v[pos + i]->data);
		_html.push_back(str);
		i++;
	}

	return i;
}

void Document::iblockquotes(std::string_view s)
{
	std::string str;

	str.append("<blockquote>");
	str.append(s, 1, s.length() - 1);
	str.append("</blockquote>");
	_html.push_back(str);
}