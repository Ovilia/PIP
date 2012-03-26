#include "Operator.h"
#include "RobertsOperator.h"

const int RobertsOperator::X_KERNERL[KERNERL_LENGTH] = {0, 0, 0,
                                                        0, -1, 0,
                                                        0, 0, 1};

const int RobertsOperator::Y_KERNERL[KERNERL_LENGTH] = {0, 0, 0,
                                                        0, 0, -1,
                                                        0, 1, 0};

RobertsOperator::RobertsOperator(QImage* image, const bool isColored,
                                 ImagePolicy::BorderPolicy policy) :
    Operator(image, KERNERL_RADIO, X_KERNERL, Y_KERNERL, isColored, policy)
{
}

RobertsOperator::~RobertsOperator()
{

}
