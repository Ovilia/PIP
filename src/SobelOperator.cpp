#include "Operator.h"
#include "SobelOperator.h"

const int SobelOperator::X_KERNERL[KERNERL_LENGTH] = {-1, -2, -1,
                                                      0, 0, 0,
                                                      1, 2, 1};

const int SobelOperator::Y_KERNERL[KERNERL_LENGTH] = {-1, 0, 1,
                                                      -2, 0, 2,
                                                      -1, 0, -1};

SobelOperator::SobelOperator(QImage* image, const bool isColored,
                             ImagePolicy::BorderPolicy policy) :
    Operator(image, KERNERL_RADIO, X_KERNERL, Y_KERNERL, isColored, policy)
{

}

SobelOperator::~SobelOperator()
{

}
