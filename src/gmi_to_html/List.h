#ifndef GENERATOR_LIST_H
#define GENERATOR_LIST_H

#include "IBaseline.h"

/**
 * Конструкция gemtext: список
 */
class List : public IBaseline
{
public:

	explicit List(std::string& line) : IBaseline(line)
	{
	}

	virtual std::string operator()() override;

	virtual Linetipe linetype() const override
	{
		return Linetipe::list;
	};

private:

};

#endif //GENERATOR_LIST_H
