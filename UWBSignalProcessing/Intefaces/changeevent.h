#ifndef CHANGEEVENT_H
#define CHANGEEVENT_H


class ChangeEvent
{
public:
    virtual ~ChangeEvent() = default;
    virtual void Change() = 0;
};


#endif // CHANGEEVENT_H
