#include "FixedPartitionManager.h"

FixedPartitionManager::FixedPartitionManager() {
    reset();
}

void FixedPartitionManager::reset() {
    partitions.clear();
    partitions.append({0, 100, false, ""});
    partitions.append({1, 200, false, ""});
    partitions.append({2, 300, false, ""});
    partitions.append({3, 400, false, ""});
    partitions.append({4, 500, false, ""});
}

bool FixedPartitionManager::allocateJob(const QString& jobName, int jobSize, QString& msg) {
    for (auto& part : partitions) {
        if (!part.is_allocated && part.size >= jobSize) {
            part.is_allocated = true;
            part.job_name = jobName;
            msg = QString("作业 %1 分配到分区 %2（大小 %3）").arg(jobName).arg(part.id).arg(part.size);
            return true;
        }
    }
    msg = QString("没有可用的足够大的分区，作业 %1 分配失败。").arg(jobName);
    return false;
}

bool FixedPartitionManager::releaseJob(const QString& jobName, QString& msg) {
    for (auto& part : partitions) {
        if (part.is_allocated && part.job_name == jobName) {
            part.is_allocated = false;
            part.job_name.clear();
            msg = QString("作业 %1 所在的分区 %2 已回收。").arg(jobName).arg(part.id);
            return true;
        }
    }
    msg = QString("未找到作业 %1。").arg(jobName);
    return false;
}

QVector<Partition> FixedPartitionManager::getPartitions() const {
    return partitions;
}