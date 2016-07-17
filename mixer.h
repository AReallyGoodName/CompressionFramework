#ifndef MIXER_H_INCLUDED
#define MIXER_H_INCLUDED

#include <list>
#include "predictor.h"
#include "markov_predictor.h"

class mixer {
	std::list< predictor* > models;
public:
	mixer( unsigned long long mem );

	float get_prediction();

	void model( unsigned int bit );
};

#endif // MIXER_H_INCLUDED
