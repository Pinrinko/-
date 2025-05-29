#pragma once
#pragma once
#ifndef DYNAMICPRIORITYSCHEDULER_H
#define DYNAMICPRIORITYSCHEDULER_H

#include <QString>
#include <QList>
#include <QPair>

struct DynamicPriorityPCB {
    QString name;
    char state;
    int super;
    int ntime;
    int rtime;
    DynamicPriorityPCB* link;
};

class DynamicPriorityScheduler {
public:
    DynamicPriorityScheduler();
    void setProcessList(const QList<QPair<QString, QPair<int, int>>>& plist); // name, (super, ntime)
    QString runSimulation();

private:
    DynamicPriorityPCB* ready;
    void sort(DynamicPriorityPCB* new_p);
    QString display(DynamicPriorityPCB* running_p);
    void clear();
};

#endif // DYNAMICPRIORITYSCHEDULER_H