#include "scenicmapwidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QDebug>
#include <QtMath>
#include <algorithm>

ScenicMapWidget::ScenicMapWidget(QWidget *parent)
    : QWidget(parent)
    , m_graph(nullptr)
    , m_selectedSpot(-1)
    , m_isTourMode(false)
{
    setMinimumSize(800, 600);
    setMouseTracking(true);
}

void ScenicMapWidget::setGraph(ScenicAreaGraph* graph)
{
    m_graph = graph;
    if (m_graph) {
        calculatePositions();
    }
    update();
}

void ScenicMapWidget::calculatePositions()
{
    if (!m_graph) return;
    
    QVector<ScenicSpot> spots = m_graph->getSpots();
    int count = spots.size();
    if (count == 0) return;
    
    m_positions.clear();
    m_positions.resize(count);
    
    // 获取窗口尺寸
    double w = width();
    double h = height();
    
    // 定义每个景点的位置（相对位置，0.0-1.0之间）
    QMap<QString, QPointF> positionMap;
    
    // 根据用户指定的布局设置位置（9个景点）
    positionMap["断桥残雪"] = QPointF(0.5 * w, 0.08 * h);      // 顶部中间
    positionMap["平湖秋月"] = QPointF(0.45 * w, 0.35 * h);      // 断桥残雪下方
    positionMap["曲院风荷"] = QPointF(0.10 * w, 0.45 * h);     // 左侧
    positionMap["苏堤春晓"] = QPointF(0.32 * w, 0.53 * h);     // 中间偏下
    positionMap["花港观鱼"] = QPointF(0.61 * w, 0.80 * h);      // 左下
    positionMap["三潭印月"] = QPointF(0.75 * w, 0.3 * h);     // 右侧
    positionMap["柳浪闻莺"] = QPointF(0.7 * w, 0.60 * h);      // 右侧上方
    positionMap["雷峰夕照"] = QPointF(0.85 * w, 0.70 * h);     // 右下
    positionMap["南屏晚钟"] = QPointF(0.83 * w, 0.84 * h);     // 右下
    
    // 为每个景点设置位置
    for (int i = 0; i < count; i++) {
        QString name = spots[i].getName();
        if (positionMap.contains(name)) {
            m_positions[i] = positionMap[name];
        } else {
            // 如果未指定位置，使用默认位置（中心）
            m_positions[i] = QPointF(w / 2.0, h / 2.0);
        }
    }
}

void ScenicMapWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    calculatePositions();
    update();
}

QPoint ScenicMapWidget::getSpotPosition(int index) const
{
    if (index >= 0 && index < m_positions.size()) {
        return m_positions[index].toPoint();
    }
    return QPoint();
}

int ScenicMapWidget::getSpotAtPosition(const QPoint& pos) const
{
    for (int i = 0; i < m_positions.size(); i++) {
        QPoint spotPos = getSpotPosition(i);
        QPoint diff = pos - spotPos;
        int distance = qSqrt(diff.x() * diff.x() + diff.y() * diff.y());
        if (distance <= SPOT_RADIUS) {
            return i;
        }
    }
    return -1;
}

void ScenicMapWidget::highlightPath(const QList<QString>& path)
{
    m_currentPath = path;
    m_currentTitle = "最短路径";
    m_highlightedSpots.clear();
    m_highlightedEdges.clear();
    m_spotOrderMap.clear();
    m_edgeOrderMap.clear();
    m_isTourMode = false;
    
    if (!m_graph || path.isEmpty()) {
        update();
        return;
    }
    
    // 标记路径上的所有景点并记录顺序
    for (int i = 0; i < path.size(); i++) {
        int index = m_graph->getSpotIndex(path[i]);
        if (index >= 0) {
            m_highlightedSpots.insert(index);
            m_spotOrderMap[index] = i + 1;  // 从1开始编号
        }
    }
    
    // 标记路径上的所有边并记录顺序（保持方向）
    for (int i = 0; i < path.size() - 1; i++) {
        int from = m_graph->getSpotIndex(path[i]);
        int to = m_graph->getSpotIndex(path[i + 1]);
        if (from >= 0 && to >= 0) {
            QPair<int, int> edge = qMakePair(qMin(from, to), qMax(from, to));
            m_highlightedEdges.insert(edge);
            // 记录有方向的边顺序
            m_edgeOrderMap[qMakePair(from, to)] = i + 1;  // 边的顺序
        }
    }
    
    update();
}

void ScenicMapWidget::clearHighlight()
{
    m_currentPath.clear();
    m_currentTitle.clear();
    m_highlightedSpots.clear();
    m_highlightedEdges.clear();
    m_spotOrderMap.clear();
    m_edgeOrderMap.clear();
    m_isTourMode = false;
    update();
}

void ScenicMapWidget::highlightTour(const QList<QString>& tour, const QString& title)
{
    m_currentPath = tour;
    m_currentTitle = title;
    m_highlightedSpots.clear();
    m_highlightedEdges.clear();
    m_spotOrderMap.clear();
    m_edgeOrderMap.clear();
    m_isTourMode = true;  // 设置为遍历模式
    
    if (!m_graph || tour.isEmpty()) {
        update();
        return;
    }
    
    // 标记遍历路径上的所有景点并记录顺序
    for (int i = 0; i < tour.size(); i++) {
        int index = m_graph->getSpotIndex(tour[i]);
        if (index >= 0) {
            m_highlightedSpots.insert(index);
            m_spotOrderMap[index] = i + 1;  // 从1开始编号
        }
    }
    
    // 标记遍历路径上的所有边（按顺序）并记录顺序
    for (int i = 0; i < tour.size() - 1; i++) {
        int from = m_graph->getSpotIndex(tour[i]);
        int to = m_graph->getSpotIndex(tour[i + 1]);
        if (from >= 0 && to >= 0) {
            QPair<int, int> edge = qMakePair(qMin(from, to), qMax(from, to));
            m_highlightedEdges.insert(edge);
            // 记录边的方向顺序（用于显示箭头）
            m_edgeOrderMap[qMakePair(from, to)] = i + 1;
        }
    }
    
    update();
}

void ScenicMapWidget::highlightMST(const QList<QPair<QString, QString>>& mstEdges)
{
    m_currentPath.clear();
    m_currentTitle = "最小生成树";
    m_highlightedSpots.clear();
    m_highlightedEdges.clear();
    
    if (!m_graph) {
        update();
        return;
    }
    
    // 标记MST中的所有边和景点
    for (const auto& edge : mstEdges) {
        int from = m_graph->getSpotIndex(edge.first);
        int to = m_graph->getSpotIndex(edge.second);
        if (from >= 0 && to >= 0) {
            m_highlightedSpots.insert(from);
            m_highlightedSpots.insert(to);
            m_highlightedEdges.insert(qMakePair(qMin(from, to), qMax(from, to)));
        }
    }
    
    update();
}

void ScenicMapWidget::drawEdge(QPainter& painter, int from, int to, bool isHighlighted, bool showArrow, int edgeOrder)
{
    if (from < 0 || to < 0 || from >= m_positions.size() || to >= m_positions.size()) {
        return;
    }
    
    QPointF start = m_positions[from];
    QPointF end = m_positions[to];
    
    // 计算边的方向向量
    QPointF dir = end - start;
    double length = qSqrt(dir.x() * dir.x() + dir.y() * dir.y());
    if (length < 1e-6) return;
    
    // 归一化方向向量
    dir /= length;
    
    // 计算起点和终点（考虑节点半径）
    QPointF adjustedStart = start + dir * SPOT_RADIUS;
    QPointF adjustedEnd = end - dir * SPOT_RADIUS;
    
    // 设置画笔
    if (isHighlighted) {
        painter.setPen(QPen(QColor(255, 100, 100), 4, Qt::SolidLine, Qt::RoundCap));
    } else {
        painter.setPen(QPen(QColor(150, 150, 150), 2, Qt::SolidLine, Qt::RoundCap));
    }
    
    painter.drawLine(adjustedStart, adjustedEnd);
    
    // 如果显示箭头（遍历模式）
    if (showArrow && isHighlighted) {
        // 在边的中点附近绘制箭头
        QPointF arrowPos = adjustedStart + (adjustedEnd - adjustedStart) * 0.7;
        drawArrow(painter, adjustedStart, adjustedEnd);
        
        // 如果有顺序号，显示在箭头附近
        if (edgeOrder > 0) {
            QPointF orderPos = adjustedStart + (adjustedEnd - adjustedStart) * 0.5;
            QRectF orderRect(orderPos.x() - 15, orderPos.y() - 10, 30, 20);
            painter.setBrush(QColor(255, 200, 100, 230));
            painter.setPen(QPen(QColor(200, 100, 0), 2));
            painter.drawRoundedRect(orderRect, 10, 10);
            painter.setPen(QColor(200, 50, 0));
            painter.setFont(QFont("Arial", 10, QFont::Bold));
            painter.drawText(orderRect, Qt::AlignCenter, QString::number(edgeOrder));
        }
    }
    
    // 如果有图数据，显示距离（非高亮边）
    if (m_graph && !isHighlighted) {
        QVector<QVector<int>> matrix = m_graph->getAdjMatrix();
        if (from < matrix.size() && to < matrix[from].size() && matrix[from][to] > 0) {
            int distance = matrix[from][to];
            QPointF mid = (adjustedStart + adjustedEnd) / 2.0;
            
            // 绘制距离标签背景
            QRectF labelRect(mid.x() - 25, mid.y() - 10, 50, 20);
            painter.setBrush(QColor(255, 255, 255, 200));
            painter.setPen(QPen(QColor(100, 100, 100), 1));
            painter.drawRoundedRect(labelRect, 3, 3);
            
            // 绘制距离文字
            painter.setPen(QColor(80, 80, 80));
            painter.setFont(QFont("Arial", 8));
            painter.drawText(labelRect, Qt::AlignCenter, QString::number(distance));
        }
    }
}

void ScenicMapWidget::drawSpot(QPainter& painter, int index, bool isHighlighted, int orderNumber)
{
    if (index < 0 || index >= m_positions.size()) return;
    
    QPointF center = m_positions[index];
    
    // 绘制外圈（高亮时）
    if (isHighlighted) {
        painter.setBrush(QColor(255, 200, 200, 100));
        painter.setPen(QPen(QColor(255, 100, 100), 3));
        painter.drawEllipse(center, SPOT_RADIUS + 5, SPOT_RADIUS + 5);
    }
    
    // 绘制景点节点
    if (isHighlighted) {
        painter.setBrush(QColor(255, 100, 100));
        painter.setPen(QPen(QColor(200, 50, 50), 2));
    } else if (index == m_selectedSpot) {
        painter.setBrush(QColor(100, 150, 255));
        painter.setPen(QPen(QColor(50, 100, 200), 2));
    } else {
        painter.setBrush(QColor(100, 150, 255));
        painter.setPen(QPen(QColor(50, 100, 200), 2));
    }
    
    painter.drawEllipse(center, SPOT_RADIUS, SPOT_RADIUS);
    
    // 绘制景点编号或顺序号
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 10, QFont::Bold));
    QString text;
    if (orderNumber > 0 && isHighlighted) {
        // 如果有顺序号，显示顺序号
        text = QString::number(orderNumber);
    } else {
        // 否则显示景点索引
        text = QString::number(index + 1);
    }
    painter.drawText(QRectF(center.x() - SPOT_RADIUS, center.y() - SPOT_RADIUS, 
                           SPOT_RADIUS * 2, SPOT_RADIUS * 2),
                    Qt::AlignCenter, text);
    
    // 如果是遍历模式且有顺序号，在节点右上角绘制顺序标签
    if (orderNumber > 0 && isHighlighted && m_isTourMode) {
        QPointF orderPos = center + QPointF(0, 0);
        QRectF orderRect(orderPos.x(), orderPos.y(), 15, 15);

        painter.setBrush(QColor(255, 215, 0, 230));
        painter.setPen(QPen(QColor(200, 150, 0), 2));
        painter.drawRoundedRect(orderRect, 9, 9); // 圆角适配尺寸
        painter.setPen(QColor(150, 100, 0));
        painter.setFont(QFont("Arial", 9, QFont::Bold)); // 字体适配
        painter.drawText(orderRect, Qt::AlignCenter, QString::number(orderNumber));
    }
}

void ScenicMapWidget::drawArrow(QPainter& painter, const QPointF& start, const QPointF& end)
{
    // 计算箭头位置（在终点前一点）
    QPointF dir = end - start;
    double length = qSqrt(dir.x() * dir.x() + dir.y() * dir.y());
    if (length < 1e-6) return;
    
    dir /= length;
    
    // 箭头位置（距离终点15像素）
    QPointF arrowPos = end - dir * 15;
    
    // 计算箭头的两个边
    double angle = qAtan2(dir.y(), dir.x());
    double arrowLength = 12;
    double arrowAngle = M_PI / 6;  // 30度
    
    QPointF arrow1 = arrowPos + QPointF(
        -arrowLength * qCos(angle - arrowAngle),
        -arrowLength * qSin(angle - arrowAngle)
    );
    QPointF arrow2 = arrowPos + QPointF(
        -arrowLength * qCos(angle + arrowAngle),
        -arrowLength * qSin(angle + arrowAngle)
    );
    
    // 绘制箭头
    painter.setBrush(QColor(255, 100, 100));
    painter.setPen(QPen(QColor(200, 50, 50), 2));
    QPolygonF arrow;
    arrow << arrowPos << arrow1 << arrow2;
    painter.drawPolygon(arrow);
}

void ScenicMapWidget::drawLabel(QPainter& painter, int index, const QPointF& center)
{
    if (!m_graph || index < 0 || index >= m_positions.size()) return;
    
    QVector<ScenicSpot> spots = m_graph->getSpots();
    if (index >= spots.size()) return;
    
    QString name = spots[index].getName();
    
    // 计算标签位置（在节点上方）
    QPointF labelPos = center - QPointF(0, SPOT_RADIUS + LABEL_OFFSET);
    
    // 绘制标签背景
    QFontMetrics fm(painter.font());
    QRect textRect = fm.boundingRect(name);
    QRectF labelRect(labelPos.x() - textRect.width() / 2.0 - 5,
                    labelPos.y() - textRect.height() / 2.0 - 2,
                    textRect.width() + 10,
                    textRect.height() + 4);
    
    painter.setBrush(QColor(255, 255, 255, 230));
    painter.setPen(QPen(QColor(100, 100, 100), 1));
    painter.drawRoundedRect(labelRect, 5, 5);
    
    // 绘制标签文字
    painter.setPen(QColor(50, 50, 50));
    painter.setFont(QFont("Microsoft YaHei", 9));
    painter.drawText(labelRect, Qt::AlignCenter, name);
}

void ScenicMapWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // 绘制背景
    painter.fillRect(rect(), QColor(245, 248, 250));
    
    if (!m_graph) {
        painter.setPen(QColor(150, 150, 150));
        painter.setFont(QFont("Arial", 14));
        painter.drawText(rect(), Qt::AlignCenter, "请加载景区数据");
        return;
    }
    
    QVector<ScenicSpot> spots = m_graph->getSpots();
    QVector<QVector<int>> matrix = m_graph->getAdjMatrix();
    
    if (spots.isEmpty()) return;
    
    // 绘制所有边（非高亮的）
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = i + 1; j < matrix[i].size(); j++) {
            if (matrix[i][j] > 0) {
                QPair<int, int> edge = qMakePair(i, j);
                bool isHighlighted = m_highlightedEdges.contains(edge);
                if (!isHighlighted) {
                    drawEdge(painter, i, j, false);
                }
            }
        }
    }
    
    // 绘制高亮的边（带箭头和顺序号）
    for (const QPair<int, int>& edge : m_highlightedEdges) {
        bool showArrow = m_isTourMode;  // 遍历模式显示箭头
        int edgeOrder = -1;
        
        // 尝试查找边的顺序（可能是有方向的）
        if (m_edgeOrderMap.contains(edge)) {
            edgeOrder = m_edgeOrderMap[edge];
        } else {
            // 尝试反向查找
            QPair<int, int> reverseEdge = qMakePair(edge.second, edge.first);
            if (m_edgeOrderMap.contains(reverseEdge)) {
                edgeOrder = m_edgeOrderMap[reverseEdge];
            } else {
                // 尝试查找有方向的边
                QPair<int, int> dirEdge1 = qMakePair(edge.first, edge.second);
                QPair<int, int> dirEdge2 = qMakePair(edge.second, edge.first);
                if (m_edgeOrderMap.contains(dirEdge1)) {
                    edgeOrder = m_edgeOrderMap[dirEdge1];
                } else if (m_edgeOrderMap.contains(dirEdge2)) {
                    edgeOrder = m_edgeOrderMap[dirEdge2];
                }
            }
        }
        
        // 确定边的方向（用于绘制箭头）
        bool drawFromTo = true;
        QPair<int, int> dirEdge = qMakePair(edge.first, edge.second);
        if (!m_edgeOrderMap.contains(dirEdge)) {
            dirEdge = qMakePair(edge.second, edge.first);
            if (m_edgeOrderMap.contains(dirEdge)) {
                drawFromTo = false;  // 需要反向绘制
            }
        }
        
        if (drawFromTo) {
            drawEdge(painter, edge.first, edge.second, true, showArrow, edgeOrder);
        } else {
            drawEdge(painter, edge.second, edge.first, true, showArrow, edgeOrder);
        }
    }
    
    // 绘制所有景点（带顺序号）
    for (int i = 0; i < spots.size(); i++) {
        bool isHighlighted = m_highlightedSpots.contains(i);
        int orderNumber = m_spotOrderMap.value(i, -1);
        drawSpot(painter, i, isHighlighted, orderNumber);
    }
    
    // 绘制标签
    for (int i = 0; i < spots.size(); i++) {
        drawLabel(painter, i, m_positions[i]);
    }
    
    // 绘制当前标题（如果有）
    if (!m_currentTitle.isEmpty()) {
        painter.setPen(QColor(50, 50, 50));
        painter.setFont(QFont("Microsoft YaHei", 12, QFont::Bold));
        QRect titleRect(10, 10, width() - 20, 30);
        painter.fillRect(titleRect, QColor(255, 255, 255, 200));
        painter.drawText(titleRect, Qt::AlignCenter, m_currentTitle);
    }
}

void ScenicMapWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        int spotIndex = getSpotAtPosition(event->pos());
        if (spotIndex >= 0) {
            m_selectedSpot = spotIndex;
            update();
            
            if (m_graph) {
                QVector<ScenicSpot> spots = m_graph->getSpots();
                if (spotIndex < spots.size()) {
                    emit spotClicked(spots[spotIndex].getName());
                }
            }
        }
    }
    QWidget::mousePressEvent(event);
}

