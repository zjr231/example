#ifndef SCENICMAPWIDGET_H
#define SCENICMAPWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QPoint>
#include <QList>
#include <QString>
#include <QSet>
#include <QMap>
#include "scenicareagraph.h"

class ScenicMapWidget : public QWidget
{
    Q_OBJECT

signals:
    void spotClicked(const QString& spotName);

public:
    explicit ScenicMapWidget(QWidget *parent = nullptr);
    
    // 设置图数据
    void setGraph(ScenicAreaGraph* graph);
    
    // 高亮路径
    void highlightPath(const QList<QString>& path);
    void clearHighlight();
    
    // 高亮遍历路径
    void highlightTour(const QList<QString>& tour, const QString& title = "");
    
    // 高亮最小生成树
    void highlightMST(const QList<QPair<QString, QString>>& mstEdges);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    // 计算景点位置（圆形布局）
    void calculatePositions();
    
    // 获取景点在屏幕上的位置
    QPoint getSpotPosition(int index) const;
    
    // 检查鼠标是否点击在景点上
    int getSpotAtPosition(const QPoint& pos) const;
    
    // 绘制景点
    void drawSpot(QPainter& painter, int index, bool isHighlighted = false, int orderNumber = -1);
    
    // 绘制边
    void drawEdge(QPainter& painter, int from, int to, bool isHighlighted = false, bool showArrow = false, int edgeOrder = -1);
    
    // 绘制标签
    void drawLabel(QPainter& painter, int index, const QPointF& center);
    
    // 绘制箭头
    void drawArrow(QPainter& painter, const QPointF& start, const QPointF& end);

private:
    ScenicAreaGraph* m_graph;
    QVector<QPointF> m_positions;  // 景点位置
    QSet<QPair<int, int>> m_highlightedEdges;  // 高亮的边
    QSet<int> m_highlightedSpots;  // 高亮的景点
    QList<QString> m_currentPath;  // 当前显示的路径
    QString m_currentTitle;  // 当前显示的标题
    int m_selectedSpot;  // 选中的景点索引
    QMap<int, int> m_spotOrderMap;  // 景点索引到顺序号的映射（用于遍历显示）
    QMap<QPair<int, int>, int> m_edgeOrderMap;  // 边到顺序号的映射
    bool m_isTourMode;  // 是否为遍历模式
    static const int SPOT_RADIUS = 20;  // 景点半径
    static const int LABEL_OFFSET = 5;  // 标签偏移
};

#endif // SCENICMAPWIDGET_H

