#ifndef PREWITTOPERATOR_H
#define PREWITTOPERATOR_H

#include <QImage>

#include "ImagePolicy.h"
#include "Operator.h"

class PrewittOperator : public Operator
{
public:
    PrewittOperator(QImage* image, const bool isColored,
                    ImagePolicy::BorderPolicy policy = ImagePolicy::NEAREST);
    virtual ~PrewittOperator();

private:
    static const int KERNERL_RADIO = 1;
    static const int KERNERL_LENGTH =
            (2 * KERNERL_RADIO + 1) * (2 * KERNERL_RADIO + 1);
    static const int X_KERNERL[KERNERL_LENGTH];
    static const int Y_KERNERL[KERNERL_LENGTH];
};

#endif // PREWITTOPERATOR_H
