#ifndef PREWITTOPERATOR_H
#define PREWITTOPERATOR_H

#include <QImage>

#include "ImagePolicy.h"
#include "Operator.h"

class PrewittOperator : Operator
{
public:
    PrewittOperator(QImage* image,
                    ImagePolicy::BorderPolicy policy = ImagePolicy::NEAREST);
    virtual ~PrewittOperator();

private:
    static const int KERNERL_LENGTH = 9;
    static const int X_KERNERL[KERNERL_LENGTH];
    static const int Y_KERNERL[KERNERL_LENGTH];
};

#endif // PREWITTOPERATOR_H
