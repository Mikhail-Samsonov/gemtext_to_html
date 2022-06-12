#ifndef GENERATOR_HEAD_H
#define GENERATOR_HEAD_H

#include "IBaseline.h"

/**
 * Конструкция gemtext: заголовок
 */
class Head : public IBaseline
{
public:

	explicit Head(std::string& line) : IBaseline(line)
	{
	}

	virtual std::string operator()() override;

	virtual Linetipe linetype() const override
	{
		return Linetipe::head;
	};

private:

};


#endif //GENERATOR_HEAD_H
