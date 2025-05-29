#include "variableroundrobin.h"

VariableRoundRobin::VariableRoundRobin()
    : ready(nullptr)
{
}

void VariableRoundRobin::clear() {
    while (ready) {
        VariableRRPCB* p = ready;
        ready = p->link;
        delete p;
    }
}

void VariableRoundRobin::enqueue(VariableRRPCB* p) {
    if (!ready) {
        ready = p;
        p->link = nullptr;
    }
    else {
        VariableRRPCB* q = ready;
        while (q->link)
            q = q->link;
        q->link = p;
        p->link = nullptr;
    }
}

void VariableRoundRobin::setProcessList(const QList<std::tuple<QString, int, int>>& plist) {
    clear();
    for (const auto& tuple : plist) {
        VariableRRPCB* p = new VariableRRPCB;
        p->name = std::get<0>(tuple);
        p->ntime = std::get<1>(tuple);
        p->timeslice = std::get<2>(tuple);
        p->rtime = 0;
        p->state = 'W';
        p->link = nullptr;
        enqueue(p);
    }
}

QString VariableRoundRobin::display() {
    QString res;
    res += "就绪队列:\n";
    res += QString("| %1 | %2 | %3 | %4 | %5 |\n")
        .arg("进程名", -8)
        .arg("状态", -8)
        .arg("总时间", -10)
        .arg("已运行", -10)
        .arg("时间片", -10);
    VariableRRPCB* p = ready;
    while (p) {
        res += QString("| %1 | %2 | %3 | %4 | %5 |\n")
            .arg(p->name, -8)
            .arg(QChar(p->state), -8)
            .arg(p->ntime, -10)
            .arg(p->rtime, -10)
            .arg(p->timeslice, -10);
        p = p->link;
    }
    return res;
}

QString VariableRoundRobin::runSimulation() {
    QString output;
    output += QStringLiteral("初始化完成，开始可变时间片轮转调度...\n");
    output += display();

    while (ready) {
        VariableRRPCB* p = ready;
        ready = p->link;
        p->link = nullptr;

        p->state = 'R';
        output += QString("\n运行进程: %1\n").arg(p->name);

        int time_to_run = (p->ntime - p->rtime) > p->timeslice ? p->timeslice : (p->ntime - p->rtime);
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
            enqueue(p);
        }

        output += display();
    }
    output += QStringLiteral("\n所有进程已完成.\n");
    return output;
}