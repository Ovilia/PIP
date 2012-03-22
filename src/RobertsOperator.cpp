#include "Operator.h"
#include "RobertsOperator.h"

const int RobertsOperator::X_KERNERL[KERNERL_LENGTH] = {0, 0, 0,
                                                        0, -1, 0,
                                                        0, 0, 1};

const int RobertsOperator::Y_KERNERL[KERNERL_LENGTH] = {-1, 0, 0,
                                                        0, 0, -1,
                                                        0, 1, 0};

RobertsOperator::RobertsOperator(QImage* image,
                                 ImagePolicy::BorderPolicy policy) :
    Operator(image, KERNERL_LENGTH, X_KERNERL, Y_KERNERL, policy)
{
}

RobertsOperator::~RobertsOperator()
{

}
