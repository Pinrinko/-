#pragma once
#pragma once
#ifndef PRIORITYSCHEDULER_H
#define PRIORITYSCHEDULER_H

#include <QString>
#include <QList>
#include <QPair>

struct PriorityPCB {
    QString name;
    char state;
    int super;     // ��̬���ȼ�
    int ntime;     // ������ʱ��
    int rtime;     // ������ʱ��
    PriorityPCB* link;
};

class PriorityScheduler {
public:
    PriorityScheduler();
    void setProcessList(const QList<QPair<QString, QPair<int, int>>>& plist); // name, (super, ntime)
    QString runSimulation();

private:
    PriorityPCB* ready;
    void sort(PriorityPCB* new_p);
    QString display(PriorityPCB* running_p);
    void clear();
};

#endif // PRIORITYSCHEDULER_H