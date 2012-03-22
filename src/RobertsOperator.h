#ifndef ROBERTSOPERATOR_H
#define ROBERTSOPERATOR_H

#include <QImage>

#include "ImagePolicy.h"
#include "Operator.h"

class RobertsOperator : public Operator
{
public:
    RobertsOperator(QImage* image,
                    ImagePolicy::BorderPolicy policy = ImagePolicy::NEAREST);
    virtual ~RobertsOperator();

private:
    static const int KERNERL_LENGTH = 9;
    static const int X_KERNERL[KERNERL_LENGTH];
    static const int Y_KERNERL[KERNERL_LENGTH];

};

#endif // ROBERTSOPERATOR_H
