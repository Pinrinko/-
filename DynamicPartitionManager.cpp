#include "DynamicPartitionManager.h"

DynamicPartitionManager::DynamicPartitionManager() {}

void DynamicPartitionManager::init(int total_size) {
    blocks.clear();
    MemoryBlock block;
    block.start = 0;
    block.size = total_size;
    block.is_allocated = false;
    block.job_name = "";
    blocks.append(block);
}

void DynamicPartitionManager::reset(int total_size) {
    init(total_size);
}

QVector<MemoryBlock> DynamicPartitionManager::getBlocks() const {
    return blocks;
}

void DynamicPartitionManager::mergeFreeBlocks() {
    for (int i = 0; i < blocks.size() - 1;) {
        if (!blocks[i].is_allocated && !blocks[i + 1].is_allocated) {
            blocks[i].size += blocks[i + 1].size;
            blocks.remove(i + 1);
        }
        else {
            ++i;
        }
    }
}

bool DynamicPartitionManager::allocateJob(const QString& jobName, int jobSize, AllocationAlgorithm algo, QString& msg) {
    int chosen_index = -1;
    int best_diff = INT_MAX, worst_size = -1;

    for (int i = 0; i < blocks.size(); ++i) {
        if (!blocks[i].is_allocated && blocks[i].size >= jobSize) {
            int diff = blocks[i].size - jobSize;
            if (algo == AllocationAlgorithm::FirstFit) {
                chosen_index = i;
                break;
            }
            else if (algo == AllocationAlgorithm::BestFit) {
                if (diff < best_diff) {
                    best_diff = diff;
                    chosen_index = i;
                }
            }
            else if (algo == AllocationAlgorithm::WorstFit) {
                if (blocks[i].size > worst_size) {
                    worst_size = blocks[i].size;
                    chosen_index = i;
                }
            }
        }
    }

    if (chosen_index == -1) {
        msg = QString("无足够空间为作业 %1 分配内存。").arg(jobName);
        return false;
    }

    auto& block = blocks[chosen_index];
    if (block.size == jobSize) {
        block.is_allocated = true;
        block.job_name = jobName;
    }
    else {
        MemoryBlock newBlock = block;
        newBlock.size = jobSize;
        newBlock.is_allocated = true;
        newBlock.job_name = jobName;

        block.start += jobSize;
        block.size -= jobSize;

        blocks.insert(chosen_index, newBlock);
    }
    msg = QString("作业 %1 分配成功。").arg(jobName);
    return true;
}

bool DynamicPartitionManager::releaseJob(const QString& jobName, QString& msg) {
    bool found = false;
    for (auto& block : blocks) {
        if (block.is_allocated && block.job_name == jobName) {
            block.is_allocated = false;
            block.job_name = "";
            found = true;
            msg = QString("作业 %1 已释放。").arg(jobName);
            break;
        }
    }
    if (!found) {
        msg = QString("未找到作业 %1。").arg(jobName);
        return false;
    }
    mergeFreeBlocks();
    return true;
}