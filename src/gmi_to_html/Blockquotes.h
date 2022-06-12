#ifndef GENERATOR_BLOCKQUOTES_H
#define GENERATOR_BLOCKQUOTES_H

#include "IBaseline.h"

/**
 * Конструкция gemtext: Цитирование
 */
class Blockquotes : public IBaseline
{
public:

	explicit Blockquotes(std::string& line) : IBaseline(line)
	{
	};

	virtual std::string operator()() override;

	virtual Linetipe linetype() const override
	{
		return Linetipe::blockquotes;
	};

private:

};


#endif //GENERATOR_BLOCKQUOTES_H
