#pragma once
#pragma once
#include <QVector>
#include <QString>

struct PageTableEntry {
    int page_number;
    bool valid;
    int frame;
};

class PageManager {
public:
    PageManager();
    void init();
    void reset();
    void accessAddress(int addr, QString& log);
    void generateAddressStream(QVector<int>& addr_stream, int len = 30);
    QVector<PageTableEntry> getPageTable() const;

    static constexpr int PAGE_SIZE = 1024;
    static constexpr int VIRTUAL_ADDRESS_SPACE = 32768;
    static constexpr int NUM_PAGES = VIRTUAL_ADDRESS_SPACE / PAGE_SIZE;
    static constexpr int MEMORY_FRAMES = 5;
    static constexpr int ADDRESS_STREAM_LEN = 30;

private:
    QVector<PageTableEntry> page_table;
    QVector<int> memory; // 当前主存中的页号
    int memory_count;
    int fifo_index;
};