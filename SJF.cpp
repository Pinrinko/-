#include "SJF.h"
#include <algorithm>

SJF::SJF() {}

void SJF::clear() {
    jobs.clear();
}

void SJF::addJob(const QString& name, int arrive_time, int run_time) {
    SJFJob job;
    job.name = name;
    job.arrive_time = arrive_time;
    job.run_time = run_time;
    job.is_finished = false;
    job.start_time = 0;
    job.finish_time = 0;
    jobs.append(job);
}

QString SJF::schedule() {
    if (jobs.isEmpty()) return "无作业可调度。\n";
    QList<SJFJob> results = jobs;
    int job_count = results.size();
    int current_time = 0;

    for (int i = 0; i < job_count; ++i) {
        int min_index = -1, min_run = INT_MAX;
        for (int j = 0; j < job_count; ++j) {
            if (!results[j].is_finished && results[j].arrive_time <= current_time) {
                if (results[j].run_time < min_run) {
                    min_run = results[j].run_time;
                    min_index = j;
                }
            }
        }
        if (min_index == -1) {
            int min_arrive = INT_MAX;
            for (int j = 0; j < job_count; ++j) {
                if (!results[j].is_finished && results[j].arrive_time < min_arrive) {
                    min_arrive = results[j].arrive_time;
                    min_index = j;
                }
            }
            current_time = results[min_index].arrive_time;
        }

        results[min_index].start_time = current_time;
        results[min_index].finish_time = current_time + results[min_index].run_time;
        results[min_index].is_finished = true;
        current_time = results[min_index].finish_time;
    }

    float total_turnaround = 0.0, total_weighted = 0.0;
    QString res;
    res += "作业\t开始\t完成\t周转时间\t带权周转时间\n";
    for (const auto& job : results) {
        int turnaround = job.finish_time - job.arrive_time;
        float weighted = (float)turnaround / job.run_time;
        res += QString("%1\t%2\t%3\t%4\t\t%5\n")
            .arg(job.name)
            .arg(job.start_time)
            .arg(job.finish_time)
            .arg(turnaround)
            .arg(weighted, 0, 'f', 2);
        total_turnaround += turnaround;
        total_weighted += weighted;
    }
    res += QString("平均周转时间: %1\n带权平均周转时间: %2\n")
        .arg(total_turnaround / job_count, 0, 'f', 2)
        .arg(total_weighted / job_count, 0, 'f', 2);
    return res;
}