#include "multilevelrr.h"

MultiLevelRR::MultiLevelRR() {
    for (int i = 0; i < LEVELS; ++i) queues[i] = nullptr;
    timeslice[0] = 2; // 高优先级
    timeslice[1] = 4; // 低优先级
}

void MultiLevelRR::clear() {
    for (int i = 0; i < LEVELS; ++i) {
        while (queues[i]) {
            MultiLevelPCB* p = queues[i];
            queues[i] = p->link;
            delete p;
        }
    }
}

void MultiLevelRR::enqueue(int level, MultiLevelPCB* p) {
    if (!queues[level]) {
        queues[level] = p;
        p->link = nullptr;
    }
    else {
        MultiLevelPCB* q = queues[level];
        while (q->link)
            q = q->link;
        q->link = p;
        p->link = nullptr;
    }
}

void MultiLevelRR::setProcessList(const QList<std::tuple<QString, int, int>>& plist) {
    clear();
    for (const auto& tuple : plist) {
        MultiLevelPCB* p = new MultiLevelPCB;
        p->name = std::get<0>(tuple);
        p->ntime = std::get<1>(tuple);
        p->level = std::get<2>(tuple);
        if (p->level < 0 || p->level >= LEVELS) p->level = 1;
        p->rtime = 0;
        p->state = 'W';
        p->link = nullptr;
        enqueue(p->level, p);
    }
}

QString MultiLevelRR::display() {
    QString res;
    for (int i = 0; i < LEVELS; ++i) {
        res += QString("\n队列 Level %1:\n").arg(i);
        res += QString("| %1 | %2 | %3 | %4 |\n")
            .arg("进程名", -8)
            .arg("状态", -8)
            .arg("总时间", -10)
            .arg("已运行", -10);
        MultiLevelPCB* p = queues[i];
        while (p) {
            res += QString("| %1 | %2 | %3 | %4 |\n")
                .arg(p->name, -8)
                .arg(QChar(p->state), -8)
                .arg(p->ntime, -10)
                .arg(p->rtime, -10);
            p = p->link;
        }
    }
    return res;
}

bool MultiLevelRR::hasProcess() {
    for (int i = 0; i < LEVELS; ++i)
        if (queues[i]) return true;
    return false;
}

QString MultiLevelRR::runSimulation() {
    QString output;
    output += QStringLiteral("初始化完成，开始多级队列轮转调度...\n");
    output += display();

    while (hasProcess()) {
        for (int i = 0; i < LEVELS; ++i) {
            if (!queues[i]) continue;

            MultiLevelPCB* p = queues[i];
            queues[i] = p->link;
            p->link = nullptr;

            p->state = 'R';
            output += QString("\n运行进程: %1 (Level %2)\n").arg(p->name).arg(i);

            int time_to_run = (p->ntime - p->rtime) > timeslice[i] ? timeslice[i] : (p->ntime - p->rtime);
            p->rtime += time_to_run;

            output += QString("进程 %1 运行了 %2 单位，总运行时间 %3 / %4\n")
                .arg(p->name)
                .arg(time_to_run)
                .arg(p->rtime)
                .arg(p->ntime);

            if (p->rtime >= p->ntime) {
                p->state = 'F';
                output += QString("进程 %1 已完成.\n").arg(p->name);
                delete p;
            }
            else {
                p->state = 'W';
                enqueue(i, p);
            }

            output += display();
            break; // 一次只运行一个进程
        }
    }
    output += QStringLiteral("\n所有进程已完成.\n");
    return output;
}