#ifndef SWI_UTILS
#define SWI_UTILS

#include "SWIUtils.h"

QString SWIUtils::FormatDownQuantity(int quantity)
{
    QString result;

    if(quantity > pow(10, 7))
    {
        quantity /= pow(10, 6);
        QTextStream(&result) << quantity << " m";
    }
    else if (quantity > pow(10 ,4))
    {
        quantity /= pow(10, 3);
        QTextStream(&result) << quantity << " k";
    }
    else
    {
        result = QString::number(quantity);
    }

    return result;
}

#endif // SWI_UTILS
