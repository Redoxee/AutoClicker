#ifndef SWI_UTILS
#define SWI_UTILS

#include "SWIUtils.h"
#include <QStringBuilder>


QString SWIUtils::FormatDownQuantity(int64_t quantity)
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

QString SWIUtils::FormatScore(int64_t score)
{
    int scoreDecumulator = score;
    QString scoreMessage = QString();
    int magnitude = 1000;
    for(int i = 0; i < 4; ++i)
    {
        int current = scoreDecumulator % magnitude;
        if(scoreDecumulator >= magnitude)
        {
            scoreMessage = QString("%1").arg(current, 3, 10, QChar('0')) % scoreMessage;
        }
        else
        {
            scoreMessage = QString::number(current) % scoreMessage;
        }

        scoreDecumulator /= 1000;
        if(scoreDecumulator > 0)
        {
            scoreMessage = "," %scoreMessage;
        }
        else
        {
            break;
        }
    }

    return scoreMessage;
}

#endif // SWI_UTILS
