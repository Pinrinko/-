#include "PageManager.h"
#include <QRandomGenerator>

PageManager::PageManager() {
    init();
}

void PageManager::init() {
    page_table.resize(NUM_PAGES);
    for (int i = 0; i < NUM_PAGES; ++i) {
        page_table[i].page_number = i;
        page_table[i].valid = false;
        page_table[i].frame = -1;
    }
    memory.clear();
    memory.resize(MEMORY_FRAMES, -1);
    memory_count = 0;
    fifo_index = 0;
}

void PageManager::reset() {
    init();
}

QVector<PageTableEntry> PageManager::getPageTable() const {
    return page_table;
}

void PageManager::accessAddress(int addr, QString& log) {
    if (addr < 0 || addr >= VIRTUAL_ADDRESS_SPACE) {
        log += QString("地址 %1 非法（超出虚拟地址空间范围 0-%2）\n").arg(addr).arg(VIRTUAL_ADDRESS_SPACE - 1);
        return;
    }
    int page_number = addr / PAGE_SIZE;
    log += QString("访问地址: %1 (页号 %2)\n").arg(addr).arg(page_number);

    if (page_table[page_number].valid) {
        log += QString("页 %1 已在主存中，帧号为 %2\n").arg(page_number).arg(page_table[page_number].frame);
    }
    else {
        log += QString("页 %1 不在主存中，需要调入\n").arg(page_number);
        if (memory_count < MEMORY_FRAMES) {
            int frame = memory_count;
            memory[memory_count++] = page_number;
            page_table[page_number].valid = true;
            page_table[page_number].frame = frame;
        }
        else {
            int victim = memory[fifo_index];
            log += QString("主存已满，淘汰页 %1（帧号 %2）\n").arg(victim).arg(page_table[victim].frame);
            page_table[victim].valid = false;

            memory[fifo_index] = page_number;
            page_table[page_number].valid = true;
            page_table[page_number].frame = fifo_index;

            fifo_index = (fifo_index + 1) % MEMORY_FRAMES;
        }
    }
}

void PageManager::generateAddressStream(QVector<int>& addr_stream, int len) {
    addr_stream.resize(len);
    for (int i = 0; i < len; ++i) {
        int r = QRandomGenerator::global()->bounded(100);
        if (r < 50 && i > 0) {
            addr_stream[i] = addr_stream[i - 1] + PAGE_SIZE / 4;
        }
        else if (r < 75) {
            addr_stream[i] = QRandomGenerator::global()->bounded(VIRTUAL_ADDRESS_SPACE / 2);
        }
        else {
            addr_stream[i] = (VIRTUAL_ADDRESS_SPACE / 2) + QRandomGenerator::global()->bounded(VIRTUAL_ADDRESS_SPACE / 2);
        }
        if (addr_stream[i] >= VIRTUAL_ADDRESS_SPACE)
            addr_stream[i] %= VIRTUAL_ADDRESS_SPACE;
    }
}