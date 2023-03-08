#ifndef SHOWSTATISTICALESTIMATE_H
#define SHOWSTATISTICALESTIMATE_H

class ShowStatisticaleStimate
{

public:

    virtual ~ShowStatisticaleStimate() = default;
    virtual void ShowStimate(double mean, double meanValue, double variance, double osv, double osv_to_meanValue) = 0;

};

#endif // SHOWSTATISTICALESTIMATE_H
