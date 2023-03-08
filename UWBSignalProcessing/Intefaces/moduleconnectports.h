#ifndef MODULECONNECTPORTS_H
#define MODULECONNECTPORTS_H

class IPort;
class OPort
{
public:
    virtual ~OPort() = default;
    virtual bool Connect(IPort* newFront) = 0;
    virtual void Disconnect() = 0;
};

class IPort
{
public:
    virtual ~IPort() = default;
    virtual bool Connect(OPort* newBack) = 0;
    virtual void Disconnect() = 0;
};

#endif // MODULECONNECTPORTS_H
