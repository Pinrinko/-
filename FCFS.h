#pragma once
#pragma once
#include <QString>
#include <QList>

struct Job {
    QString name;
    int arrive_time;
    int run_time;
    int start_time;
    int finish_time;
    QChar state; // 'W', 'R', 'F'
};

class FCFS {
public:
    FCFS();
    void clear();
    void addJob(const QString& name, int arrive_time, int run_time);
    QString displayQueue() const;
    QString schedule();

private:
    QList<Job> jobQueue;
};