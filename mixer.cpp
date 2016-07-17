#include <list>

#include "mixer.h"

mixer::mixer( unsigned long long mem ) {
    //models.push_front( new vertical_markov_predictor( mem/10 ) );
    models.push_front( new markov_predictor( mem ) );
}

float mixer::get_prediction() {
    /*if ( (*models.rbegin())->get_prediction() > 0.95f || (*models.rbegin())->get_prediction() < 0.05f && (*models.rbegin())->get_likelihood() > 4 ) {
        return (*models.rbegin())->get_prediction();
    }
    else {*/
        // ToDo: fix the mixer
        return (*models.begin())->get_prediction();
    //}
}

void mixer::model( unsigned int bit ) {
    for ( std::list< predictor* >::iterator i = models.begin(); i != models.end(); ++i ) {
        (*i)->model(bit);
    }
}
