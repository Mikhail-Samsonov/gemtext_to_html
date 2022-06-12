#ifndef GENERATOR_PREFORMATTED_H
#define GENERATOR_PREFORMATTED_H

#include "IBaseline.h"

/**
 * Конструкция gemtext: предформатированный участок
 */
class Preformatted : public IBaseline
{
public:

	explicit Preformatted(std::string& line) : IBaseline(line)
	{
	}

	virtual std::string operator()() override;

	virtual Linetipe linetype() const override
	{
		return Linetipe::preformatted;
	};

private:

};


#endif //GENERATOR_PREFORMATTED_H
