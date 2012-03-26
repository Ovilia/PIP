#include "Operator.h"
#include "PrewittOperator.h"

const int PrewittOperator::X_KERNERL[KERNERL_LENGTH] = {-1, -1, -1,
                                                        0, 0, 0,
                                                        1, 1, 1};

const int PrewittOperator::Y_KERNERL[KERNERL_LENGTH] = {-1, 0, 1,
                                                        -1, 0, 1,
                                                        -1, 0, 1};
PrewittOperator::PrewittOperator(QImage* image, const bool isColored,
                                 ImagePolicy::BorderPolicy policy) :
    Operator(image, KERNERL_RADIO, X_KERNERL, Y_KERNERL, isColored, policy)
{
}

PrewittOperator::~PrewittOperator()
{

}
