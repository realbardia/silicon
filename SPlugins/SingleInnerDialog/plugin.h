#ifndef PLUGIN_H
#define PLUGIN_H

#include <SPlugin>

class DManager;
class SingleInnerDialog : public SPlugin
{
public:
    SingleInnerDialog();
    ~SingleInnerDialog();

protected:
    bool startEvent();
    bool stopEvent();

private:
    DManager *m;
};

#endif // PLUGIN_H
