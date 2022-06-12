#ifndef GENERATOR_LINK_H
#define GENERATOR_LINK_H

#include "IBaseline.h"

/**
 * Конструкция gemtext: ссылка
 */
class Link : public IBaseline
{
public:

	explicit Link(std::string& line) : IBaseline(line)
	{
	}

	virtual std::string operator()() override;

	virtual Linetipe linetype() const override
	{
		return Linetipe::link;
	};

private:

};


#endif //GENERATOR_LINK_H
