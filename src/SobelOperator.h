#ifndef SOBELOPERATOR_H
#define SOBELOPERATOR_H

#include <QImage>

#include "ImagePolicy.h"
#include "Operator.h"

class SobelOperator : public Operator
{
public:
    SobelOperator(QImage* image,
                  ImagePolicy::BorderPolicy policy = ImagePolicy::NEAREST);
    virtual ~SobelOperator();

private:
    static const int KERNERL_LENGTH = 9;
    static const int X_KERNERL[KERNERL_LENGTH];
    static const int Y_KERNERL[KERNERL_LENGTH];

};

#endif // SOBELOPERATOR_H
