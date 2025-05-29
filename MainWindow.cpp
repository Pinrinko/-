#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "roundrobin.h"
#include "priorityscheduler.h"
#include "multilevelrr.h"
#include "dynamicpriorityscheduler.h"
#include "variableroundrobin.h"
#include <QInputDialog>
#include <QList>
#include <QPair>
#include <QInputDialog>
#include <QVector>
#include "yhj.h"
#include <QMessageBox>
#include "FCFS.h"

#include "HRN.h"
#include "SJF.h"
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindowClass)
{
    ui->setupUi(this);
    ui->textEditHistory->setReadOnly(true);
   
    connect(ui->page0button1, &QPushButton::clicked, this, &MainWindow::page0button1);
    connect(ui->page0button2, &QPushButton::clicked, this, &MainWindow::page0button2);
    connect(ui->page0button3, &QPushButton::clicked, this, &MainWindow::page0button3);
    connect(ui->page0button4, &QPushButton::clicked, this, &MainWindow::page0button4);
    connect(ui->page0button5, &QPushButton::clicked, this, &MainWindow::page0button5);
    connect(ui->on_btnBackToMain_clicked, &QPushButton::clicked, this, &MainWindow::on_btnBackToMain_clicked);
    connect(ui->on_btnBackToMain_clicked_2, &QPushButton::clicked, this, &MainWindow::on_btnBackToMain_clicked);
    connect(ui->on_btnBackToMain_clicked_3, &QPushButton::clicked, this, &MainWindow::on_btnBackToMain_clicked);
    connect(ui->on_btnBackToMain_clicked_5, &QPushButton::clicked, this, &MainWindow::on_btnBackToMain_clicked);
    connect(ui->on_btnBackToMain_clicked_5, &QPushButton::clicked, this, &MainWindow::on_btnBackToMain_clicked);

    connect(ui->pushButtonYHJ, &QPushButton::clicked, this, &MainWindow::on_pushButtonYHJ_clicked);

    connect(ui->pushButton_1, &QPushButton::clicked, this, &MainWindow::runRoundRobin);

    /*connect(ui->pushButtonYHJRequest, &QPushButton::clicked, this, &MainWindow::on_pushButtonYHJRequest_clicked);*/
	/*connect(ui->pushButtonFCFS, &QPushButton::clicked, this, &MainWindow::on_pushButtonFCFS_clicked);*/   // 这行注释掉了，因为 pushButtonFCFS是原按钮名，自动绑定了
}

MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::on_btnBackToMain_clicked() { ui->sss->setCurrentIndex(0); }
void MainWindow::page0button1() { ui->sss->setCurrentIndex(1); }
void MainWindow::page0button2() { ui->sss->setCurrentIndex(2); }
void MainWindow::page0button3() { ui->sss->setCurrentIndex(3); }
void MainWindow::page0button4() { ui->sss->setCurrentIndex(4); }
void MainWindow::page0button5() { ui->sss->setCurrentIndex(5);
// 弹窗输入进程数和资源数
bool ok1, ok2;
int processCount = QInputDialog::getInt(this, tr("进程数"), tr("请输入进程数:"), 3, 1, 50, 1, &ok1);
if (!ok1) return;
int resourceCount = QInputDialog::getInt(this, tr("资源种类数"), tr("请输入资源种类数:"), 3, 1, 20, 1, &ok2);
if (!ok2) return;

// 初始化表格
initBankerTables(processCount, resourceCount);
}




void MainWindow::runRoundRobin() {
    bool ok;
    int count = QInputDialog::getInt(this, tr("进程数"), tr("请输入进程数:"), 5, 1, 100, 1, &ok);
    if (!ok) return;

    QList<QPair<QString, int>> plist;
    for (int i = 0; i < count; ++i) {
        bool ok1, ok2;
        QString name = QInputDialog::getText(this, tr("输入进程名"),
                                             tr("请输入第%1个进程名:").arg(i+1),
                                             QLineEdit::Normal, "", &ok1);
        if (!ok1 || name.isEmpty()) return;

        int ntime = QInputDialog::getInt(this, tr("输入运行时间"),
                                         tr("请输入%1的运行时间:").arg(name),
                                         1, 1, 100, 1, &ok2);
        if (!ok2) return;

        plist.append(qMakePair(name, ntime));
    }

    RoundRobin rr;
    rr.setProcessList(plist);
    QString result = rr.runSimulation();
    ui->textEdit->setText(result);
}

void MainWindow::initBankerTables(int processCount, int resourceCount)
{
    ui->tabMAX->setRowCount(processCount);
    ui->tabMAX->setColumnCount(resourceCount);
    ui->tabALLOCATION->setRowCount(processCount);
    ui->tabALLOCATION->setColumnCount(resourceCount);
    ui->tabAV->setRowCount(1);
    ui->tabAV->setColumnCount(resourceCount);

    // 允许编辑
    ui->tabMAX->setEditTriggers(QAbstractItemView::AllEditTriggers);
    ui->tabALLOCATION->setEditTriggers(QAbstractItemView::AllEditTriggers);
    ui->tabAV->setEditTriggers(QAbstractItemView::AllEditTriggers);
}

void MainWindow::readBankerTables(QVector<QVector<int>>& max, QVector<QVector<int>>& allocation, QVector<int>& available)
{
    int m = ui->tabMAX->rowCount();
    int n = ui->tabMAX->columnCount();
    max.resize(m);
    allocation.resize(m);
    for (int i = 0; i < m; ++i) {
        max[i].resize(n);
        allocation[i].resize(n);
        for (int j = 0; j < n; ++j) {
            max[i][j] = ui->tabMAX->item(i, j) ? ui->tabMAX->item(i, j)->text().toInt() : 0;
            allocation[i][j] = ui->tabALLOCATION->item(i, j) ? ui->tabALLOCATION->item(i, j)->text().toInt() : 0;
        }
    }
    available.resize(n);
    for (int j = 0; j < n; ++j) {
        available[j] = ui->tabAV->item(0, j) ? ui->tabAV->item(0, j)->text().toInt() : 0;
    }
}


void MainWindow::on_pushButton_2_clicked() {
    bool ok;
    int count = QInputDialog::getInt(this, tr("进程数"), tr("请输入进程数:"), 5, 1, 100, 1, &ok);
    if (!ok) return;

    QList<QPair<QString, QPair<int, int>>> plist;
    for (int i = 0; i < count; ++i) {
        bool ok1, ok2, ok3;
        QString name = QInputDialog::getText(this, tr("输入进程名"),
            tr("请输入第%1个进程名:").arg(i + 1),
            QLineEdit::Normal, "", &ok1);
        if (!ok1 || name.isEmpty()) return;

        int super = QInputDialog::getInt(this, tr("输入优先级"),
            tr("请输入%1的优先级:").arg(name),
            1, -100, 100, 1, &ok2);
        if (!ok2) return;

        int ntime = QInputDialog::getInt(this, tr("输入运行时间"),
            tr("请输入%1的运行时间:").arg(name),
            1, 1, 100, 1, &ok3);
        if (!ok3) return;

        plist.append(qMakePair(name, qMakePair(super, ntime)));
    }

    PriorityScheduler scheduler;
    scheduler.setProcessList(plist);
    QString result = scheduler.runSimulation();
    ui->textEdit->setText(result);
}


void MainWindow::on_pushButton_3_clicked() {
    bool ok;
    int count = QInputDialog::getInt(this, tr("进程数"), tr("请输入进程数:"), 5, 1, 100, 1, &ok);
    if (!ok) return;

    QList<QPair<QString, QPair<int, int>>> plist;
    for (int i = 0; i < count; ++i) {
        bool ok1, ok2, ok3;
        QString name = QInputDialog::getText(this, tr("输入进程名"),
            tr("请输入第%1个进程名:").arg(i + 1),
            QLineEdit::Normal, "", &ok1);
        if (!ok1 || name.isEmpty()) return;

        int super = QInputDialog::getInt(this, tr("输入优先级"),
            tr("请输入%1的优先级:").arg(name),
            1, -100, 100, 1, &ok2);
        if (!ok2) return;

        int ntime = QInputDialog::getInt(this, tr("输入运行时间"),
            tr("请输入%1的运行时间:").arg(name),
            1, 1, 100, 1, &ok3);
        if (!ok3) return;

        plist.append(qMakePair(name, qMakePair(super, ntime)));
    }

    DynamicPriorityScheduler scheduler;
    scheduler.setProcessList(plist);
    QString result = scheduler.runSimulation();
    ui->textEdit->setText(result);
}

void MainWindow::on_pushButton_4_clicked() {
    bool ok;
    int count = QInputDialog::getInt(this, tr("进程数"), tr("请输入进程数:"), 5, 1, 100, 1, &ok);
    if (!ok) return;

    QList<std::tuple<QString, int, int>> plist;
    for (int i = 0; i < count; ++i) {
        bool ok1, ok2, ok3;
        QString name = QInputDialog::getText(this, tr("输入进程名"),
            tr("请输入第%1个进程名:").arg(i + 1),
            QLineEdit::Normal, "", &ok1);
        if (!ok1 || name.isEmpty()) return;

        int ntime = QInputDialog::getInt(this, tr("输入运行时间"),
            tr("请输入%1的运行时间:").arg(name),
            1, 1, 100, 1, &ok2);
        if (!ok2) return;

        int timeslice = QInputDialog::getInt(this, tr("输入时间片"),
            tr("请输入%1的时间片:").arg(name),
            1, 1, 100, 1, &ok3);
        if (!ok3) return;

        plist.append(std::make_tuple(name, ntime, timeslice));
    }

    VariableRoundRobin scheduler;
    scheduler.setProcessList(plist);
    QString result = scheduler.runSimulation();
    ui->textEdit->setText(result);
}



void MainWindow::on_pushButton_5_clicked() {
    bool ok;
    int count = QInputDialog::getInt(this, tr("进程数"), tr("请输入进程数:"), 5, 1, 100, 1, &ok);
    if (!ok) return;

    QList<std::tuple<QString, int, int>> plist;
    for (int i = 0; i < count; ++i) {
        bool ok1, ok2, ok3;
        QString name = QInputDialog::getText(this, tr("输入进程名"),
            tr("请输入第%1个进程名:").arg(i + 1),
            QLineEdit::Normal, "", &ok1);
        if (!ok1 || name.isEmpty()) return;

        int ntime = QInputDialog::getInt(this, tr("输入运行时间"),
            tr("请输入%1的运行时间:").arg(name),
            1, 1, 100, 1, &ok2);
        if (!ok2) return;

        int level = QInputDialog::getInt(this, tr("输入优先级"),
            tr("请输入%1的优先级 (0高, 1低):").arg(name),
            0, 0, 1, 1, &ok3);
        if (!ok3) return;

        plist.append(std::make_tuple(name, ntime, level));
    }

    MultiLevelRR scheduler;
    scheduler.setProcessList(plist);
    QString result = scheduler.runSimulation();
    ui->textEdit->setText(result);
}
YHJ yhj;
void MainWindow::on_pushButtonYHJ_clicked()
{
    QVector<QVector<int>> max, allocation;
    QVector<int> available;
    readBankerTables(max, allocation, available);

    int m = max.size();
    int n = m > 0 ? max[0].size() : 0;
    yhj.setProcessResource(m, n, max, allocation, available);

    QString output = yhj.showStatus();
    output += yhj.getSafeSequence();
    ui->textEdit_YHJ->setText(output);

    // 不再弹出“是否继续请求”
}
void MainWindow::on_pushButtonYHJRequest_clicked()
{
    int m = yhj.getProcessCount();
    int n = yhj.getResourceCount();
    QString output;

    bool ok;
    int pid = QInputDialog::getInt(this, tr("请求进程号"), tr("请输入请求资源的进程号(0~%1):").arg(m - 1), 0, 0, m - 1, 1, &ok);
    if (!ok) return;

    QString reqStr = QInputDialog::getText(this, tr("请求资源"), tr("请输入进程%1的各资源请求数（用空格分隔）:").arg(pid), QLineEdit::Normal, "", &ok);
    if (!ok) return;

    QStringList reqList = reqStr.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    if (reqList.size() != n) {
        QMessageBox::warning(this, tr("输入错误"), tr("资源数目不匹配！"));
        return;
    }
    QVector<int> req(n);
    for (int j = 0; j < n; ++j) req[j] = reqList[j].toInt();

    output += "\n" + yhj.requestResource(pid, req);
    output += yhj.showStatus();

    // 刷新表格内容
    const auto& alloc = yhj.getAllocation();
    const auto& avail = yhj.getAvailable();
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < n; ++j)
            ui->tabALLOCATION->setItem(i, j, new QTableWidgetItem(QString::number(alloc[i][j])));
    for (int j = 0; j < n; ++j)
        ui->tabAV->setItem(0, j, new QTableWidgetItem(QString::number(avail[j])));

    ui->textEdit_YHJ->setText(output);
}


FCFS fcfs;
// 假设在 MainWindow 类中添加如下槽函数
void MainWindow::on_pushButtonFCFS_clicked()
{
    qDebug() << "FCFS clicked";
    fcfs.clear();
    bool ok;
    int jobCount = QInputDialog::getInt(this, tr("作业数"), tr("请输入作业数:"), 3, 1, 50, 1, &ok);
    if (!ok) return;
    for (int i = 0; i < jobCount; ++i) {
        QString name = QInputDialog::getText(this, tr("作业名"), tr("请输入第%1个作业名:").arg(i + 1), QLineEdit::Normal, "", &ok);
        if (!ok || name.isEmpty()) return;
        int arrive = QInputDialog::getInt(this, tr("提交时间"), tr("请输入%1的提交时间:").arg(name), 0, 0, 10000, 1, &ok);
        if (!ok) return;
        int run = QInputDialog::getInt(this, tr("运行时间"), tr("请输入%1的运行时间:").arg(name), 1, 1, 10000, 1, &ok);
        if (!ok) return;
        fcfs.addJob(name, arrive, run);
    }
    QString output = fcfs.displayQueue();
    output += "\n";
    output += fcfs.schedule();
    ui->textEdit_JOB->setText(output); // 假设你有一个 QTextEdit 用于显示结果
}

SJF sjf;

void MainWindow::on_pushButtonSJF_clicked()
{
    sjf.clear();
    bool ok;
    int jobCount = QInputDialog::getInt(this, tr("作业数"), tr("请输入作业数:"), 3, 1, 50, 1, &ok);
    if (!ok) return;
    for (int i = 0; i < jobCount; ++i) {
        QString name = QInputDialog::getText(this, tr("作业名"), tr("请输入第%1个作业名:").arg(i + 1), QLineEdit::Normal, "", &ok);
        if (!ok || name.isEmpty()) return;
        int arrive = QInputDialog::getInt(this, tr("到达时间"), tr("请输入%1的到达时间:").arg(name), 0, 0, 10000, 1, &ok);
        if (!ok) return;
        int run = QInputDialog::getInt(this, tr("运行时间"), tr("请输入%1的运行时间:").arg(name), 1, 1, 10000, 1, &ok);
        if (!ok) return;
        sjf.addJob(name, arrive, run);
    }
    QString output = sjf.schedule();
    ui->textEdit_JOB->setText(output); // 假设你有一个 QTextEdit 用于显示结果
}

HRN hrn;

void MainWindow::on_pushButtonHRN_clicked()
{
    hrn.clear();
    bool ok;
    int jobCount = QInputDialog::getInt(this, tr("作业数"), tr("请输入作业数:"), 3, 1, 50, 1, &ok);
    if (!ok) return;
    for (int i = 0; i < jobCount; ++i) {
        QString name = QInputDialog::getText(this, tr("作业名"), tr("请输入第%1个作业名:").arg(i + 1), QLineEdit::Normal, "", &ok);
        if (!ok || name.isEmpty()) return;
        int arrive = QInputDialog::getInt(this, tr("到达时间"), tr("请输入%1的到达时间:").arg(name), 0, 0, 10000, 1, &ok);
        if (!ok) return;
        int run = QInputDialog::getInt(this, tr("运行时间"), tr("请输入%1的运行时间:").arg(name), 1, 1, 10000, 1, &ok);
        if (!ok) return;
        hrn.addJob(name, arrive, run);
    }
    QString output = hrn.schedule();
    ui->textEdit_JOB->setText(output); // 假设你有一个 QTextEdit 用于显示结果
}

#include "FixedPartitionManager.h"
#include "FixedPartitionManager.h"
FixedPartitionManager partitionMgr;

void MainWindow::on_pushButtonFixedPartitionAlloc_clicked() {
    // 弹出操作选择菜单
    QStringList actions;
    actions << "分配作业" << "回收作业" << "重置分区" << "查看分区状态";
    bool ok = false;
    QString action = QInputDialog::getItem(this, tr("固定分区管理"), tr("请选择操作："), actions, 0, false, &ok);
    if (!ok) return;

    if (action == "分配作业") {
        QString jobName = QInputDialog::getText(this, tr("作业名"), tr("请输入作业名:"), QLineEdit::Normal, "", &ok);
        if (!ok || jobName.isEmpty()) return;
        int jobSize = QInputDialog::getInt(this, tr("作业大小"), tr("请输入作业大小:"), 100, 1, 10000, 1, &ok);
        if (!ok) return;
        QString msg;
        partitionMgr.allocateJob(jobName, jobSize, msg);
        QMessageBox::information(this, tr("分配结果"), msg);
    }
    else if (action == "回收作业") {
        QString jobName = QInputDialog::getText(this, tr("作业名"), tr("请输入要释放的作业名:"), QLineEdit::Normal, "", &ok);
        if (!ok || jobName.isEmpty()) return;
        QString msg;
        partitionMgr.releaseJob(jobName, msg);
        QMessageBox::information(this, tr("回收结果"), msg);
    }
    else if (action == "重置分区") {
        partitionMgr.reset();
        QMessageBox::information(this, tr("重置分区"), tr("分区已重置为初始状态。"));
    }
    // "查看分区状态" 和其他操作都刷新表格
     // 固定分区表格初始化（只需设置一次）
    ui->tableWidgetfenqu->setColumnCount(4);
    ui->tableWidgetfenqu->setHorizontalHeaderLabels(QStringList() << "分区号" << "大小" << "状态" << "作业名");

    updatePartitionTable();
}

void MainWindow::updatePartitionTable() {
    const auto& parts = partitionMgr.getPartitions();

    ui->tableWidgetfenqu->setRowCount(parts.size());
    for (int i = 0; i < parts.size(); ++i) {
        ui->tableWidgetfenqu->setItem(i, 0, new QTableWidgetItem(QString::number(parts[i].id)));
        ui->tableWidgetfenqu->setItem(i, 1, new QTableWidgetItem(QString::number(parts[i].size)));
        ui->tableWidgetfenqu->setItem(i, 2, new QTableWidgetItem(parts[i].is_allocated ? "已分配" : "空闲"));
        ui->tableWidgetfenqu->setItem(i, 3, new QTableWidgetItem(parts[i].is_allocated ? parts[i].job_name : "-"));
    }
}

#include "DynamicPartitionManager.h"
DynamicPartitionManager dynamicPartitionMgr;
int dynamicPartitionTotalSize = 1024; // 可根据需要初始化

void MainWindow::on_pushButtonDynamicPartition_clicked() {
    // 第一次使用时初始化
    static bool initialized = false;
    if (!initialized) {
        bool ok;
        int size = QInputDialog::getInt(this, tr("初始化内存"), tr("请输入总内存大小:"), 1024, 1, 100000, 1, &ok);
        if (!ok) return;
        dynamicPartitionTotalSize = size;
        dynamicPartitionMgr.init(dynamicPartitionTotalSize);
        initialized = true;
        updateDynamicPartitionTable();
    }

    QStringList actions;
    actions << "分配作业" << "回收作业" << "查看内存状态" << "重置内存";
    bool ok = false;
    QString action = QInputDialog::getItem(this, tr("动态分区管理"), tr("请选择操作："), actions, 0, false, &ok);
    if (!ok) return;

    if (action == "分配作业") {
        // 选择算法
        QStringList algos;
        algos << "首次适应" << "最佳适应" << "最坏适应";
        QString algoStr = QInputDialog::getItem(this, tr("分配算法"), tr("请选择分配算法："), algos, 0, false, &ok);
        if (!ok) return;
        AllocationAlgorithm algo = AllocationAlgorithm::FirstFit;
        if (algoStr == "首次适应") algo = AllocationAlgorithm::FirstFit;
        else if (algoStr == "最佳适应") algo = AllocationAlgorithm::BestFit;
        else if (algoStr == "最坏适应") algo = AllocationAlgorithm::WorstFit;

        QString jobName = QInputDialog::getText(this, tr("作业名"), tr("请输入作业名:"), QLineEdit::Normal, "", &ok);
        if (!ok || jobName.isEmpty()) return;
        int jobSize = QInputDialog::getInt(this, tr("作业大小"), tr("请输入作业大小:"), 100, 1, dynamicPartitionTotalSize, 1, &ok);
        if (!ok) return;
        QString msg;
        dynamicPartitionMgr.allocateJob(jobName, jobSize, algo, msg);
        QMessageBox::information(this, tr("分配结果"), msg);
    }
    else if (action == "回收作业") {
        QString jobName = QInputDialog::getText(this, tr("作业名"), tr("请输入要释放的作业名:"), QLineEdit::Normal, "", &ok);
        if (!ok || jobName.isEmpty()) return;
        QString msg;
        dynamicPartitionMgr.releaseJob(jobName, msg);
        QMessageBox::information(this, tr("回收结果"), msg);
    }
    else if (action == "重置内存") {
        dynamicPartitionMgr.reset(dynamicPartitionTotalSize);
        QMessageBox::information(this, tr("重置内存"), tr("内存已重置为初始状态。"));
    }
    // 查看内存状态和其他操作都刷新表格
    ui->tableWidgetfenqu->setColumnCount(4);
    ui->tableWidgetfenqu->setHorizontalHeaderLabels(QStringList() << "起始地址" << "大小" << "状态" << "作业名");
    updateDynamicPartitionTable();
}

void MainWindow::updateDynamicPartitionTable() {
    const auto& blocks = dynamicPartitionMgr.getBlocks();
    ui->tableWidgetfenqu->setRowCount(blocks.size());
    for (int i = 0; i < blocks.size(); ++i) {
        ui->tableWidgetfenqu->setItem(i, 0, new QTableWidgetItem(QString::number(blocks[i].start)));
        ui->tableWidgetfenqu->setItem(i, 1, new QTableWidgetItem(QString::number(blocks[i].size)));
        ui->tableWidgetfenqu->setItem(i, 2, new QTableWidgetItem(blocks[i].is_allocated ? "已分配" : "空闲"));
        ui->tableWidgetfenqu->setItem(i, 3, new QTableWidgetItem(blocks[i].is_allocated ? blocks[i].job_name : "-"));
    }
}


#include "PageManager.h"
PageManager pageMgr;

void MainWindow::on_pushButtonPage_clicked() {
    // 设置页表表头
    ui->tableWidgetfenqu->setColumnCount(3);
    ui->tableWidgetfenqu->setHorizontalHeaderLabels(QStringList() << "页号" << "Valid" << "帧号");

    QStringList actions;
    actions << "自动生成地址流访问" << "人工输入地址访问" << "显示当前页表" << "重置页表";
    bool ok = false;
    QString action = QInputDialog::getItem(this, tr("请求分页管理"), tr("请选择操作："), actions, 0, false, &ok);
    if (!ok) return;

    QString log;
    if (action == "自动生成地址流访问") {
        int count = QInputDialog::getInt(this, tr("地址流长度"), tr("请输入要生成的地址数量："), 30, 1, 1000, 1, &ok);
        if (!ok) return;
        QVector<int> addr_stream;
        pageMgr.generateAddressStream(addr_stream, count);
        for (int addr : addr_stream) {
            pageMgr.accessAddress(addr, log);
        }
        QMessageBox::information(this, tr("访问日志"), log);
    }
    else if (action == "人工输入地址访问") {
        while (true) {
            int addr = QInputDialog::getInt(this, tr("输入虚拟地址"), tr("请输入虚拟地址（0~%1，-1返回）:").arg(PageManager::VIRTUAL_ADDRESS_SPACE - 1), 0, -1, PageManager::VIRTUAL_ADDRESS_SPACE - 1, 1, &ok);
            if (!ok || addr == -1) break;
            pageMgr.accessAddress(addr, log);
        }
        QMessageBox::information(this, tr("访问日志"), log);
    }
    else if (action == "重置页表") {
        pageMgr.reset();
        QMessageBox::information(this, tr("重置"), tr("页表已重置。"));
    }
    // 显示页表
    updatePageTable();
}

void MainWindow::updatePageTable() {
    const auto& table = pageMgr.getPageTable();
    // 只显示 valid==true 的页
    int row = 0;
    ui->tableWidgetfenqu->setRowCount(PageManager::NUM_PAGES);
    for (int i = 0; i < table.size(); ++i) {
        if (table[i].valid) {
            ui->tableWidgetfenqu->setItem(row, 0, new QTableWidgetItem(QString::number(table[i].page_number)));
            ui->tableWidgetfenqu->setItem(row, 1, new QTableWidgetItem("1"));
            ui->tableWidgetfenqu->setItem(row, 2, new QTableWidgetItem(QString::number(table[i].frame)));
            ++row;
        }
    }
    ui->tableWidgetfenqu->setRowCount(row); // 只显示有效页
}

#include "SegmentPageManager.h"
SegmentPageManager segPageMgr;

void MainWindow::on_pushButtonSegmentPage_clicked() {
    bool ok1, ok2, ok3;
    int segment = QInputDialog::getInt(this, tr("段号"), tr("请输入段号(0~2)："), 0, 0, 2, 1, &ok1);
    if (!ok1) return;
    int page = QInputDialog::getInt(this, tr("页号"), tr("请输入页号(0~4)："), 0, 0, 4, 1, &ok2);
    if (!ok2) return;
    int offset = QInputDialog::getInt(this, tr("偏移"), tr("请输入页内偏移(0~1023)："), 0, 0, 1023, 1, &ok3);
    if (!ok3) return;

    QString result = segPageMgr.translateAddress(segment, page, offset);
    QMessageBox::information(this, tr("段页式地址转换"), result);

    // 可选：在tableWidgetfenqu中展示段表和页表
    // 例如：显示段表
    ui->tableWidgetfenqu->setColumnCount(2);
    ui->tableWidgetfenqu->setHorizontalHeaderLabels(QStringList() << "base" << "limit");
    ui->tableWidgetfenqu->setRowCount(segPageMgr.getSegmentTable().size());
    auto segs = segPageMgr.getSegmentTable();
    for (int i = 0; i < segs.size(); ++i) {
        ui->tableWidgetfenqu->setItem(i, 0, new QTableWidgetItem(QString::number(segs[i].base)));
        ui->tableWidgetfenqu->setItem(i, 1, new QTableWidgetItem(QString::number(segs[i].limit)));
    }
    // 或者显示某个段的页表
}

#include "FileSystem.h"
FileSystem fs;

void MainWindow::on_pushButtonfile_clicked() {
    QStringList actions;
    actions << "创建文件" << "打开文件" << "关闭文件" << "删除文件"
        << "读取文件" << "写入文件" << "切换权限" << "重命名文件" << "文件列表";
    bool ok = false;
    QString action = QInputDialog::getItem(this, tr("文件系统"), tr("请选择操作："), actions, 0, false, &ok);
    if (!ok) return;

    QString info;
    if (action == "创建文件") {
        QString name = QInputDialog::getText(this, tr("创建文件"), tr("请输入文件名："), QLineEdit::Normal, "", &ok);
        if (!ok || name.isEmpty()) return;
        info = fs.createFile(name);
    }
    else if (action == "打开文件") {
        QString name = QInputDialog::getText(this, tr("打开文件"), tr("请输入文件名："), QLineEdit::Normal, "", &ok);
        if (!ok || name.isEmpty()) return;
        info = fs.openFile(name);
    }
    else if (action == "关闭文件") {
        info = fs.closeFile();
    }
    else if (action == "删除文件") {
        QString name = QInputDialog::getText(this, tr("删除文件"), tr("请输入文件名："), QLineEdit::Normal, "", &ok);
        if (!ok || name.isEmpty()) return;
        info = fs.deleteFile(name);
    }
    else if (action == "读取文件") {
        info = fs.readFile();
    }
    else if (action == "写入文件") {
        if (fs.getActiveFileIndex() == -1) {
            info = "无活动文件！请先打开文件。";
        }
        else {
            QString content = QInputDialog::getText(this, tr("写入文件"), tr("请输入写入内容："), QLineEdit::Normal, "", &ok);
            if (!ok) return;
            info = fs.writeFile(content);
        }
    }
    else if (action == "切换权限") {
        info = fs.changePermission();
    }
    else if (action == "重命名文件") {
        QString oldName = QInputDialog::getText(this, tr("重命名文件"), tr("请输入旧文件名："), QLineEdit::Normal, "", &ok);
        if (!ok || oldName.isEmpty()) return;
        QString newName = QInputDialog::getText(this, tr("重命名文件"), tr("请输入新文件名："), QLineEdit::Normal, "", &ok);
        if (!ok || newName.isEmpty()) return;
        info = fs.renameFile(oldName, newName);
    }
    else if (action == "文件列表") {
        info = fs.listFiles();
    }
    ui->textEditfile->setText(info);
}

#include "FileTree.h"
FileTree fileTree;

void MainWindow::on_pushButtonfileCommand_clicked()
{
    qDebug() << "button clicked";
    on_lineEditCmd_returnPressed();
}
void MainWindow::updateTreeWidgetFile() {
    ui->treeWidgetfile->clear();
    std::function<void(FileNode*, QTreeWidgetItem*)> addNode =
        [&](FileNode* node, QTreeWidgetItem* parentItem) {
        QTreeWidgetItem* item = new QTreeWidgetItem();
        item->setText(0, node->name + (node->type == NodeType::Directory ? "/" : ""));
        if (parentItem)
            parentItem->addChild(item);
        else
            ui->treeWidgetfile->addTopLevelItem(item);
        for (auto child : node->children)
            addNode(child, item);
        };
    addNode(fileTree.getRoot(), nullptr);
    ui->treeWidgetfile->expandAll();
}

void MainWindow::on_lineEditCmd_returnPressed()
{
    QString cmd = ui->lineEditCmd->text().trimmed();
    if (cmd.isEmpty()) return;

    // 1. 显示命令到历史区
    ui->textEditHistory->appendPlainText("$ " + cmd);

    // 2. 处理命令
    QString result;
    if (cmd.startsWith("mkdir ")) {
        result = fileTree.mkdir(cmd.mid(6).trimmed());
    }
    else if (cmd.startsWith("touch ")) {
        result = fileTree.touch(cmd.mid(6).trimmed());
    }
    else if (cmd == "ls") {
        result = fileTree.ls();
    }
    else if (cmd.startsWith("cd ")) {
        result = fileTree.cd(cmd.mid(3).trimmed());
    }
    else if (cmd.startsWith("rm ")) {
        result = fileTree.rm(cmd.mid(3).trimmed());
    }
    else if (cmd == "tree") {
        result = fileTree.tree();
        updateTreeWidgetFile();
    }
    else if (cmd == "pwd") {
        result = fileTree.pwd();
    }
    else {
        result = "未知命令";
    }

    // 3. 显示结果到历史区
    ui->textEditHistory->appendPlainText(result);

    // 4. 清空输入框，准备下一个命令
    ui->lineEditCmd->clear();
}