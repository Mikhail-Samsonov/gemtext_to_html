#ifndef GENERATOR_IBASELINE_H
#define GENERATOR_IBASELINE_H

#include <iostream>

/**
 * Виды конструкций языка
 */
enum class Linetipe
{
	blockquotes,
	head,
	link,
	list,
	preformatted,
	text,
	null
};

class IBaseline
{
public:

	IBaseline() = default;

	explicit IBaseline(std::string& line);

	virtual std::string operator()() = 0;

	virtual ~IBaseline() = default;

	virtual Linetipe linetype() const = 0;

	// тут хранится строка gem файла
	std::string data = "";

protected:

};

#endif //GENERATOR_IBASELINE_H
