#ifndef GENERATOR_TEXT_H
#define GENERATOR_TEXT_H

#include "IBaseline.h"

/**
 * Конструкция gemtext: Блок текста
 */
class Text : public IBaseline
{
public:
	explicit Text(std::string& line) : IBaseline(line)
	{
	}

	virtual std::string operator()() override;

	virtual Linetipe linetype() const override
	{
		return Linetipe::text;
	};

private:
};


#endif //GENERATOR_TEXT_H
