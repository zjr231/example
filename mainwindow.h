#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "scenicareagraph.h"
#include "scenicmapwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // 按钮点击槽函数
    void on_pushButtonDFS_clicked();
    void on_pushButtonBFS_clicked();
    void on_pushButtonMST_clicked();
    void on_pushButtonQueryPath_clicked();
    void on_pushButtonClear_clicked();
    
    // 地图交互
    void onSpotClicked(const QString& spotName);
    
    // 初始化方法
    void initializeComboBoxes();

    // 新增：从外部文件加载图
    void loadGraphFromFile();

private:
    Ui::MainWindow *ui;
    ScenicAreaGraph *m_graph;
    ScenicMapWidget *m_mapWidget;
    
    // 辅助方法
    void updateResultText(const QString& title, 
                         const QString& start, 
                         const QString& end,
                         const QList<QString>& path,
                         int distance = -1);
};

#endif // MAINWINDOW_H
