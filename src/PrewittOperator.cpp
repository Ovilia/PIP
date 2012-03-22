#include "Operator.h"
#include "PrewittOperator.h"

const int PrewittOperator::X_KERNERL[KERNERL_LENGTH] = {-1, -1, -1,
                                                        0, 0, 0,
                                                        1, 1, 1};

const int PrewittOperator::Y_KERNERL[KERNERL_LENGTH] = {-1, 0, 1,
                                                        -1, 0, 1,
                                                        -1, 0, 1};
PrewittOperator::PrewittOperator(QImage* image,
                                 ImagePolicy::BorderPolicy policy) :
    Operator(image, KERNERL_LENGTH, X_KERNERL, Y_KERNERL, policy)
{
}

PrewittOperator::~PrewittOperator()
{

}
