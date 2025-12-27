#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QVBoxLayout>
#include <QDebug>
#include <QFileDialog>
#include <QAction>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_graph(new ScenicAreaGraph())
    , m_mapWidget(new ScenicMapWidget(this))
{
    ui->setupUi(this);
    
    // 设置窗口标题
    setWindowTitle("西湖景区管理系统 - 图形化地图");
    
    // 将地图组件添加到布局中（替换占位widget）
    QVBoxLayout* mapLayout = qobject_cast<QVBoxLayout*>(ui->mapWidget->parentWidget()->layout());
    if (mapLayout) {
        int index = mapLayout->indexOf(ui->mapWidget);
        if (index >= 0) {
            mapLayout->removeWidget(ui->mapWidget);
            ui->mapWidget->setParent(nullptr);
            delete ui->mapWidget;
            mapLayout->insertWidget(index, m_mapWidget);
        } else {
            mapLayout->addWidget(m_mapWidget);
        }
    }
    
    // 设置地图数据
    m_mapWidget->setGraph(m_graph);
    
    // 连接地图信号
    connect(m_mapWidget, &ScenicMapWidget::spotClicked, this, &MainWindow::onSpotClicked);
    
    // 初始化下拉框
    initializeComboBoxes();

    // 在菜单栏添加“加载图文件”操作
    QAction* loadAct = new QAction(tr("加载图文件"), this);
    connect(loadAct, &QAction::triggered, this, &MainWindow::loadGraphFromFile);
    menuBar()->addAction(loadAct);
    
    // 初始显示
    ui->statusbar->showMessage("系统就绪 - 点击地图上的景点或使用右侧控制面板进行操作", 5000);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_graph;
}

void MainWindow::initializeComboBoxes()
{
    QVector<ScenicSpot> spots = m_graph->getSpots();
    ui->comboBoxPathStart->clear();
    ui->comboBoxPathEnd->clear();
    ui->comboBoxTourStart->clear();
    
    for (const ScenicSpot& spot : spots) {
        ui->comboBoxPathStart->addItem(spot.getName());
        ui->comboBoxPathEnd->addItem(spot.getName());
        ui->comboBoxTourStart->addItem(spot.getName());
    }
    
    if (!spots.isEmpty()) {
        ui->comboBoxPathStart->setCurrentIndex(0);
        ui->comboBoxPathEnd->setCurrentIndex(spots.size() > 1 ? 1 : 0);
        ui->comboBoxTourStart->setCurrentIndex(0);
    }
}

void MainWindow::on_pushButtonQueryPath_clicked()
{
    QString startSpot = ui->comboBoxPathStart->currentText();
    QString endSpot = ui->comboBoxPathEnd->currentText();
    if (startSpot.isEmpty() || endSpot.isEmpty()) {
        QMessageBox::warning(this, "提示", "请选择起点和终点景点");
        return;
    }
    if (startSpot == endSpot) {
        QMessageBox::information(this, "提示", "起点和终点不能相同");
        return;
    }
    QList<QString> path = m_graph->getShortestPath(startSpot, endSpot);
    int distance = m_graph->getShortestDistance(startSpot, endSpot);
    if (path.isEmpty()) {
        QMessageBox::information(this, "结果", QString("从【%1】到【%2】无路径可达").arg(startSpot).arg(endSpot));
        m_mapWidget->clearHighlight();
        ui->textEditResult->clear();
        return;
    }
    m_mapWidget->highlightPath(path);
    updateResultText("最短路径查询", startSpot, endSpot, path, distance);
    ui->statusbar->showMessage(QString("路径查询完成：%1 → %2，距离 %3米，途经 %4个景点")
                               .arg(startSpot).arg(endSpot).arg(distance).arg(path.size()), 5000);
}

void MainWindow::on_pushButtonDFS_clicked()
{
    QString startSpot = ui->comboBoxTourStart->currentText();
    if (startSpot.isEmpty()) {
        QMessageBox::warning(this, "提示", "请选择起点景点");
        return;
    }
    QList<QString> tour = m_graph->dfsTour(startSpot);
    if (tour.isEmpty()) {
        QMessageBox::information(this, "结果", "遍历失败：起点景点不存在");
        return;
    }
    m_mapWidget->highlightTour(tour, "深度优先遍历(DFS)");
    QString resultHtml = QString("<h3 style='color: #2E5A88;'>深度优先遍历(DFS)</h3>");
    resultHtml += QString("<p><b>起点：</b><span style='color: #228B22;'>%1</span></p>").arg(startSpot);
    resultHtml += "<p><b>遍历顺序：</b></p><ol>";
    for (int i = 0; i < tour.size(); i++) resultHtml += QString("<li>%1. %2</li>").arg(i + 1).arg(tour[i]);
    resultHtml += QString("</ol><p><b>共访问 %1 个景点</b></p>").arg(tour.size());
    ui->textEditResult->setHtml(resultHtml);
    ui->statusbar->showMessage(QString("DFS遍历完成，共访问%1个景点").arg(tour.size()), 5000);
}

void MainWindow::on_pushButtonBFS_clicked()
{
    QString startSpot = ui->comboBoxTourStart->currentText();
    if (startSpot.isEmpty()) {
        QMessageBox::warning(this, "提示", "请选择起点景点");
        return;
    }
    QList<QString> tour = m_graph->bfsTour(startSpot);
    if (tour.isEmpty()) {
        QMessageBox::information(this, "结果", "遍历失败：起点景点不存在");
        return;
    }
    m_mapWidget->highlightTour(tour, "广度优先遍历(BFS)");
    QString resultHtml = QString("<h3 style='color: #2E5A88;'>广度优先遍历(BFS)</h3>");
    resultHtml += QString("<p><b>起点：</b><span style='color: #228B22;'>%1</span></p>").arg(startSpot);
    resultHtml += "<p><b>遍历顺序：</b></p><ol>";
    for (int i = 0; i < tour.size(); i++) resultHtml += QString("<li>%1. %2</li>").arg(i + 1).arg(tour[i]);
    resultHtml += QString("</ol><p><b>共访问 %1 个景点</b></p>").arg(tour.size());
    ui->textEditResult->setHtml(resultHtml);
    ui->statusbar->showMessage(QString("BFS遍历完成，共访问%1个景点").arg(tour.size()), 5000);
}

void MainWindow::on_pushButtonMST_clicked()
{
    QList<QPair<QString, QString>> mstEdges = m_graph->primAlgorithm();
    if (mstEdges.isEmpty()) {
        QMessageBox::information(this, "结果", "无法生成最小生成树");
        return;
    }
    m_mapWidget->highlightMST(mstEdges);
    QString mstHtml = QString("<h3 style='color: #2E5A88;'>最小生成树结果</h3>");
    mstHtml += "<p style='color: #666;'>连接以下景点可以覆盖所有景点且总距离最短</p>";
    mstHtml += "<table style='width: 100%; border-collapse: collapse;'>";
    mstHtml += "<tr style='background-color: #4A6FA5; color: white;'>";
    mstHtml += "<th style='padding: 5px; text-align: center;'>序号</th>";
    mstHtml += "<th style='padding: 5px; text-align: center;'>景点A</th>";
    mstHtml += "<th style='padding: 5px; text-align: center;'>景点B</th>";
    mstHtml += "<th style='padding: 5px; text-align: center;'>距离(米)</th>";
    mstHtml += "</tr>";
    int totalDistance = 0;
    for (int i = 0; i < mstEdges.size(); i++) {
        const auto& edge = mstEdges[i];
        int fromIdx = m_graph->getSpotIndex(edge.first);
        int toIdx = m_graph->getSpotIndex(edge.second);
        int distance = 0;
        if (fromIdx != -1 && toIdx != -1) distance = m_graph->getAdjMatrix()[fromIdx][toIdx];
        totalDistance += distance;
        QString rowColor = (i % 2 == 0) ? "#F8F9FA" : "#FFFFFF";
        mstHtml += QString("<tr style='background-color: %1;'>").arg(rowColor);
        mstHtml += QString("<td style='padding: 5px; text-align: center;'>%1</td>").arg(i + 1);
        mstHtml += QString("<td style='padding: 5px; text-align: center;'>%1</td>").arg(edge.first);
        mstHtml += QString("<td style='padding: 5px; text-align: center;'>%1</td>").arg(edge.second);
        mstHtml += QString("<td style='padding: 5px; text-align: center;'>%1</td>").arg(distance);
        mstHtml += "</tr>";
    }
    mstHtml += "</table>";
    mstHtml += QString("<p><b>边数量：</b>%1 条</p>").arg(mstEdges.size());
    mstHtml += QString("<p><b>顶点数量：</b>%1 个</p>").arg(m_graph->getSpots().size());
    mstHtml += QString("<p><b>总距离：</b><span style='color: #FF8C00;'>%1 米</span></p>").arg(totalDistance);
    ui->textEditResult->setHtml(mstHtml);
    ui->statusbar->showMessage(QString("最小生成树生成完成，共%1条边，总距离%2米").arg(mstEdges.size()).arg(totalDistance), 5000);
}

void MainWindow::on_pushButtonClear_clicked()
{
    m_mapWidget->clearHighlight();
    ui->textEditResult->clear();
    ui->statusbar->showMessage("已清除所有高亮显示", 2000);
}

void MainWindow::onSpotClicked(const QString& spotName)
{
    int index = ui->comboBoxPathStart->findText(spotName);
    if (index >= 0) ui->comboBoxPathStart->setCurrentIndex(index);
    index = ui->comboBoxTourStart->findText(spotName);
    if (index >= 0) ui->comboBoxTourStart->setCurrentIndex(index);
    ui->statusbar->showMessage(QString("已选择景点：%1").arg(spotName), 2000);
}

// 实现：加载外部图文件（确保此实现与 header 中声明完全一致）
void MainWindow::loadGraphFromFile()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("选择图数据文件"), QString(), tr("文本文件 (*.txt);;所有文件 (*)"));
    if (filePath.isEmpty()) return;

    if (m_graph->loadFromFile(filePath)) {
        m_graph->floydAlgorithm();
        m_mapWidget->setGraph(m_graph);
        initializeComboBoxes();
        ui->statusbar->showMessage(QString("已加载图文件：%1").arg(filePath), 5000);
    } else {
        QMessageBox::warning(this, tr("加载失败"), tr("无法解析文件，请检查文件格式（首行节点数，随后 n×n 的邻接矩阵，使用 20000 表示无连接）。"));
    }
}

void MainWindow::updateResultText(const QString& title,
                                  const QString& start,
                                  const QString& end,
                                  const QList<QString>& path,
                                  int distance)
{
    QString resultHtml;
    resultHtml += QString("<h3 style='color: #2E5A88;'>%1</h3>").arg(title);
    resultHtml += "<div style='background-color: #F0F8FF; padding: 8px; border-radius: 3px; margin: 5px 0;'>";
    resultHtml += QString("<p><b>起点：</b><span style='color: #228B22;'>%1</span></p>").arg(start);
    if (!end.isEmpty()) resultHtml += QString("<p><b>终点：</b><span style='color: #DC143C;'>%1</span></p>").arg(end);
    if (distance != -1) resultHtml += QString("<p><b>最短距离：</b><span style='color: #FF8C00; font-size: 14px; font-weight: bold;'>%1 米</span></p>").arg(distance);
    resultHtml += "</div>";
    resultHtml += "<p><b>路径顺序：</b></p><ol style='font-size: 12px;'>";
    for (int i = 0; i < path.size(); i++) {
        QString style = (i == 0) ? "color: #228B22; font-weight: bold;" : ((i == path.size() - 1 && !end.isEmpty()) ? "color: #DC143C; font-weight: bold;" : "color: #333;");
        resultHtml += QString("<li style='%1'>%2. %3</li>").arg(style).arg(i + 1).arg(path[i]);
    }
    resultHtml += "</ol>";
    resultHtml += QString("<p><b>统计：</b>共经过 <b>%1</b> 个景点</p>").arg(path.size());
    ui->textEditResult->setHtml(resultHtml);
}
