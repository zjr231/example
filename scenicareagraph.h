#ifndef SCENICAREAGRAPH_H
#define SCENICAREAGRAPH_H

#include "scenicspot.h"
#include <QVector>
#include <QMap>
#include <QList>
#include <QPair>
#include <climits>

class ScenicAreaGraph
{
public:
    ScenicAreaGraph();
    
    // 初始化数据
    void initializeWestLakeSpots();

    // 从文件加载图数据（返回是否成功）
    bool loadFromFile(const QString& filePath);

    
    // 核心算法
    void floydAlgorithm();  // Floyd算法计算最短路径
    QList<QString> dfsTour(const QString& startSpot);
    QList<QString> bfsTour(const QString& startSpot);
    QList<QString> getShortestPath(const QString& start, const QString& end);
    int getShortestDistance(const QString& start, const QString& end);
    QList<QPair<QString, QString>> primAlgorithm();  // 最小生成树
    
    // 辅助方法
    int getSpotIndex(const QString& name) const;
    QVector<ScenicSpot> getSpots() const { return m_spots; }
    QVector<QVector<int>> getAdjMatrix() const { return m_adjMatrix; }
    QString getSpotName(int index) const;
    
private:
    QVector<ScenicSpot> m_spots;           // 景点列表
    QMap<QString, int> m_nameToIndex;      // 名称到索引的映射
    QVector<QVector<int>> m_adjMatrix;     // 邻接矩阵
    QVector<QVector<int>> m_distMatrix;    // 最短距离矩阵
    QVector<QVector<int>> m_nextMatrix;    // 最短路径下一节点
};

#endif // SCENICAREAGRAPH_H

