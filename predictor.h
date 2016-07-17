#ifndef PREDICTOR_H_INCLUDED
#define PREDICTOR_H_INCLUDED

/*
* Predictor class
* A Base class. Derive this to make a predictor that can be plugged into the mixer
* The predictor should return a value of the probability the next bit is 1
*/
class predictor {
public:
	virtual float get_prediction()=0; // Should be between 0 and 1
	virtual float get_likelihood()=0; // Used to weight models
	virtual void model( unsigned int bit )=0; // Called when each bit is read
};

#endif // PREDICTOR_H_INCLUDED
