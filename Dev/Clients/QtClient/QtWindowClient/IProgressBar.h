#ifndef IPROGRESSBAR_H
#define IPROGRESSBAR_H

class IProgressBar
{
public:
    virtual void SetProgress(float value) = 0;
};

#endif // IPROGRESSBAR_H
