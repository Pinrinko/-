#include "FCFS.h"
#include <algorithm>

FCFS::FCFS() {}

void FCFS::clear() {
    jobQueue.clear();
}

void FCFS::addJob(const QString& name, int arrive_time, int run_time) {
    Job job;
    job.name = name;
    job.arrive_time = arrive_time;
    job.run_time = run_time;
    job.state = 'W';
    job.start_time = 0;
    job.finish_time = 0;
    // 按提交时间插入
    int idx = 0;
    while (idx < jobQueue.size() && jobQueue[idx].arrive_time <= arrive_time)
        ++idx;
    jobQueue.insert(idx, job);
}

QString FCFS::displayQueue() const {
    QString res;
    res += "作业名\t提交时间\t运行时间\t状态\n";
    for (const auto& job : jobQueue) {
        res += QString("%1\t%2\t%3\t%4\n")
            .arg(job.name)
            .arg(job.arrive_time)
            .arg(job.run_time)
            .arg(job.state);
    }
    return res;
}

QString FCFS::schedule() {
    if (jobQueue.isEmpty()) return "无作业可调度。\n";
    int current_time = 0;
    float total_turnaround = 0.0;
    float total_weighted_turnaround = 0.0;
    QString res;
    res += "开始调度：\n";
    for (auto& job : jobQueue) {
        job.state = 'R';
        job.start_time = qMax(current_time, job.arrive_time);
        job.finish_time = job.start_time + job.run_time;
        job.state = 'F';

        int turnaround_time = job.finish_time - job.arrive_time;
        float weighted_turnaround = (float)turnaround_time / job.run_time;

        total_turnaround += turnaround_time;
        total_weighted_turnaround += weighted_turnaround;

        res += QString("作业 %1 开始于 %2，完成于 %3，周转时间: %4，带权周转时间: %5\n")
            .arg(job.name)
            .arg(job.start_time)
            .arg(job.finish_time)
            .arg(turnaround_time)
            .arg(weighted_turnaround, 0, 'f', 2);

        current_time = job.finish_time;
    }
    res += QString("全部作业调度完成。\n平均周转时间: %1\n带权平均周转时间: %2\n")
        .arg(total_turnaround / jobQueue.size(), 0, 'f', 2)
        .arg(total_weighted_turnaround / jobQueue.size(), 0, 'f', 2);
    return res;
}