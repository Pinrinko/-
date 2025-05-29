#pragma once
#ifndef MULTILEVELRR_H
#define MULTILEVELRR_H

#include <QString>
#include <QList>
#include <tuple>

struct MultiLevelPCB {
    QString name;
    int ntime;
    int rtime;
    char state;
    int level;
    MultiLevelPCB* link;
};

class MultiLevelRR {
public:
    MultiLevelRR();
    void setProcessList(const QList<std::tuple<QString, int, int>>& plist); // name, ntime, level
    QString runSimulation();

private:
    static const int LEVELS = 2;
    MultiLevelPCB* queues[LEVELS];
    int timeslice[LEVELS];
    void enqueue(int level, MultiLevelPCB* p);
    QString display();
    bool hasProcess();
    void clear();
};

#endif // MULTILEVELRR_H#pragma once
