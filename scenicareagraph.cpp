#include "scenicareagraph.h"
#include <QStack>
#include <QQueue>
#include <algorithm>
#include <QFile>
#include <QIODevice>
#include <QRegularExpression>
#include <QDebug>

ScenicAreaGraph::ScenicAreaGraph() {
    initializeWestLakeSpots();
    floydAlgorithm();
}

bool ScenicAreaGraph::loadFromFile(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "无法打开文件:" << filePath;
        return false;
    }

    QString content = file.readAll();
    file.close();

    // 提取所有整数（支持文件包含注释或非数字字符）
    QRegularExpression rx("(-?\\d+)");
    QRegularExpressionMatchIterator it = rx.globalMatch(content);
    QVector<int> nums;
    while (it.hasNext()) {
        auto m = it.next();
        nums.append(m.captured(1).toInt());
    }

    if (nums.isEmpty()) {
        qWarning() << "文件中未找到整数:" << filePath;
        return false;
    }

    int n = nums[0];
    if (n <= 0) {
        qWarning() << "节点数非法:" << n;
        return false;
    }

    if (nums.size() < 1 + n * n) {
        qWarning() << "数据不足，期望" << (1 + n * n) << "个整数，实际" << nums.size();
        return false;
    }

    // 重建图数据结构
    m_spots.clear();
    m_nameToIndex.clear();
    m_adjMatrix.clear();

    m_adjMatrix.resize(n);
    for (int i = 0; i < n; i++) {
        m_adjMatrix[i].resize(n);
        // 使用数字字符串作为景点名（1..n），id 也使用 i+1（与示意图一致）
        ScenicSpot spot(i + 1, QString::number(i + 1), "");
        m_spots.append(spot);
        m_nameToIndex[spot.getName()] = i;
    }

    const int NO_EDGE = 20000; // 约定值：表示无连接
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int val = nums[1 + i * n + j];
            if (val == NO_EDGE || val <= 0) {
                m_adjMatrix[i][j] = 0; // 无边或自环视为0（无连接）
            }
            else {
                m_adjMatrix[i][j] = val;
            }
        }
    }

    return true;
}

void ScenicAreaGraph::initializeWestLakeSpots() {
    // 清空现有数据
    m_spots.clear();
    m_nameToIndex.clear();
    
    // 西湖9个主要景点
    m_spots.append(ScenicSpot(1, "断桥残雪", "西湖十景之一，白堤的起点"));
    m_spots.append(ScenicSpot(2, "平湖秋月", "西湖十景之一，秋夜赏月最佳地点"));
    m_spots.append(ScenicSpot(3, "苏堤春晓", "西湖十景之一，贯穿西湖南北的堤坝"));
    m_spots.append(ScenicSpot(4, "曲院风荷", "西湖十景之一，以荷花景观著名"));
    m_spots.append(ScenicSpot(5, "雷峰夕照", "西湖十景之一，雷峰塔的日落景观"));
    m_spots.append(ScenicSpot(6, "南屏晚钟", "西湖十景之一，净慈寺的钟声"));
    m_spots.append(ScenicSpot(7, "柳浪闻莺", "西湖十景之一，以柳树和鸟鸣为特色"));
    m_spots.append(ScenicSpot(8, "三潭印月", "西湖十景之一，湖中的小瀛洲"));
    m_spots.append(ScenicSpot(9, "花港观鱼", "西湖十景之一，观鱼赏花的好去处"));
    
    // 初始化名称到索引的映射
    for (int i = 0; i < m_spots.size(); i++) {
        m_nameToIndex[m_spots[i].getName()] = i;
    }
    
    // 初始化邻接矩阵（9×9）
    int n = m_spots.size();
    m_adjMatrix.resize(n);
    for (int i = 0; i < n; i++) {
        m_adjMatrix[i].resize(n);
        for (int j = 0; j < n; j++) {
            m_adjMatrix[i][j] = 0;  // 默认无连接
        }
    }
    
    // 设置景点间的距离（单位：米）
    // 对称设置，假设距离是对称的
    // 索引: 0-断桥残雪, 1-平湖秋月, 2-苏堤春晓, 3-曲院风荷, 4-雷峰夕照, 5-南屏晚钟, 6-柳浪闻莺, 7-三潭印月, 8-花港观鱼
    m_adjMatrix[0][1] = 800;   m_adjMatrix[1][0] = 800;   // 断桥残雪 - 平湖秋月
    m_adjMatrix[0][2] = 1200;  m_adjMatrix[2][0] = 1200;  // 断桥残雪 - 苏堤春晓
    
    m_adjMatrix[1][2] = 600;   m_adjMatrix[2][1] = 600;   // 平湖秋月 - 苏堤春晓
    
    m_adjMatrix[2][3] = 900;   m_adjMatrix[3][2] = 900;   // 苏堤春晓 - 曲院风荷
    m_adjMatrix[2][4] = 2500;  m_adjMatrix[4][2] = 2500;  // 苏堤春晓 - 雷峰夕照
    m_adjMatrix[2][8] = 1700;  m_adjMatrix[8][2] = 1700;  // 苏堤春晓 - 花港观鱼
    
    m_adjMatrix[4][5] = 300;   m_adjMatrix[5][4] = 300;   // 雷峰夕照 - 南屏晚钟
    m_adjMatrix[4][6] = 500;   m_adjMatrix[6][4] = 500;   // 雷峰夕照 - 柳浪闻莺
    m_adjMatrix[4][7] = 1200;  m_adjMatrix[7][4] = 1200;  // 雷峰夕照 - 三潭印月
    m_adjMatrix[4][8] = 1200;   m_adjMatrix[8][4] = 1200;   // 雷峰夕照 - 花港观鱼
    
    m_adjMatrix[5][6] = 700;   m_adjMatrix[6][5] = 700;   // 南屏晚钟 - 柳浪闻莺
}

void ScenicAreaGraph::floydAlgorithm() {
    int n = m_spots.size();
    
    // 初始化距离矩阵和路径矩阵
    m_distMatrix.resize(n);
    m_nextMatrix.resize(n);
    
    for (int i = 0; i < n; i++) {
        m_distMatrix[i].resize(n);
        m_nextMatrix[i].resize(n);
        
        for (int j = 0; j < n; j++) {
            if (i == j) {
                m_distMatrix[i][j] = 0;
                m_nextMatrix[i][j] = j;
            } else if (m_adjMatrix[i][j] > 0) {
                m_distMatrix[i][j] = m_adjMatrix[i][j];
                m_nextMatrix[i][j] = j;
            } else {
                m_distMatrix[i][j] = INT_MAX / 2;  // 避免溢出
                m_nextMatrix[i][j] = -1;
            }
        }
    }
    
    // Floyd算法核心
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            if (m_distMatrix[i][k] == INT_MAX / 2) continue;
            for (int j = 0; j < n; j++) {
                if (m_distMatrix[k][j] == INT_MAX / 2) continue;
                
                int newDist = m_distMatrix[i][k] + m_distMatrix[k][j];
                if (newDist < m_distMatrix[i][j]) {
                    m_distMatrix[i][j] = newDist;
                    m_nextMatrix[i][j] = m_nextMatrix[i][k];
                }
            }
        }
    }
}

int ScenicAreaGraph::getSpotIndex(const QString& name) const {
    if (m_nameToIndex.contains(name)) {
        return m_nameToIndex[name];
    }
    return -1;
}

QString ScenicAreaGraph::getSpotName(int index) const {
    if (index >= 0 && index < m_spots.size()) {
        return m_spots[index].getName();
    }
    return "";
}

QList<QString> ScenicAreaGraph::dfsTour(const QString& startSpot) {
    QList<QString> result;
    int startIndex = getSpotIndex(startSpot);
    if (startIndex == -1) return result;
    
    QVector<bool> visited(m_spots.size(), false);
    QStack<int> stack;
    stack.push(startIndex);
    
    while (!stack.empty()) {
        int current = stack.pop();
        
        if (!visited[current]) {
            visited[current] = true;
            result.append(m_spots[current].getName());
            
            // 收集所有未访问的相邻节点
            QList<QPair<QString, int>> neighbors;
            for (int i = 0; i < m_spots.size(); i++) {
                if (m_adjMatrix[current][i] > 0 && !visited[i]) {
                    neighbors.append(qMakePair(m_spots[i].getName(), i));
                }
            }
            
            // 按名称排序后反向压栈
            std::sort(neighbors.begin(), neighbors.end(), 
                [](const QPair<QString, int>& a, const QPair<QString, int>& b) {
                    return a.first > b.first;
                });
            
            for (const auto& neighbor : neighbors) {
                stack.push(neighbor.second);
            }
        }
    }
    
    return result;
}

QList<QString> ScenicAreaGraph::bfsTour(const QString& startSpot) {
    QList<QString> result;
    int startIndex = getSpotIndex(startSpot);
    if (startIndex == -1) return result;
    
    QVector<bool> visited(m_spots.size(), false);
    QQueue<int> queue;
    
    queue.enqueue(startIndex);
    visited[startIndex] = true;
    
    while (!queue.empty()) {
        int current = queue.dequeue();
        result.append(m_spots[current].getName());
        
        // 收集所有未访问的相邻节点
        QList<QPair<QString, int>> neighbors;
        for (int i = 0; i < m_spots.size(); i++) {
            if (m_adjMatrix[current][i] > 0 && !visited[i]) {
                neighbors.append(qMakePair(m_spots[i].getName(), i));
            }
        }
        
        // 按名称排序
        std::sort(neighbors.begin(), neighbors.end(), 
            [](const QPair<QString, int>& a, const QPair<QString, int>& b) {
                return a.first < b.first;
            });
        
        for (const auto& neighbor : neighbors) {
            if (!visited[neighbor.second]) {
                queue.enqueue(neighbor.second);
                visited[neighbor.second] = true;
            }
        }
    }
    
    return result;
}

QList<QString> ScenicAreaGraph::getShortestPath(const QString& start, const QString& end) {
    QList<QString> path;
    int startIndex = getSpotIndex(start);
    int endIndex = getSpotIndex(end);
    
    if (startIndex == -1 || endIndex == -1) return path;
    if (m_distMatrix[startIndex][endIndex] >= INT_MAX / 2) return path;
    
    // 重建路径
    int current = startIndex;
    while (current != endIndex) {
        path.append(m_spots[current].getName());
        current = m_nextMatrix[current][endIndex];
        if (current == -1) {
            path.clear();
            return path;
        }
    }
    path.append(m_spots[endIndex].getName());
    
    return path;
}

int ScenicAreaGraph::getShortestDistance(const QString& start, const QString& end) {
    int startIndex = getSpotIndex(start);
    int endIndex = getSpotIndex(end);
    
    if (startIndex == -1 || endIndex == -1) return -1;
    if (m_distMatrix[startIndex][endIndex] >= INT_MAX / 2) return -1;
    
    return m_distMatrix[startIndex][endIndex];
}

QList<QPair<QString, QString>> ScenicAreaGraph::primAlgorithm() {
    QList<QPair<QString, QString>> mstEdges;
    int n = m_spots.size();
    
    if (n == 0) return mstEdges;
    
    QVector<bool> visited(n, false);
    QVector<int> minDist(n, INT_MAX);
    QVector<int> parent(n, -1);
    
    minDist[0] = 0;
    
    for (int i = 0; i < n; i++) {
        // 找到未访问的最小距离顶点
        int minIndex = -1;
        int minValue = INT_MAX;
        
        for (int j = 0; j < n; j++) {
            if (!visited[j] && minDist[j] < minValue) {
                minValue = minDist[j];
                minIndex = j;
            }
        }
        
        if (minIndex == -1) break;
        
        visited[minIndex] = true;
        
        // 添加边到MST（排除根节点）
        if (parent[minIndex] != -1) {
            mstEdges.append(qMakePair(
                m_spots[parent[minIndex]].getName(),
                m_spots[minIndex].getName()
            ));
        }
        
        // 更新相邻顶点的最小距离
        for (int j = 0; j < n; j++) {
            if (!visited[j] && m_adjMatrix[minIndex][j] > 0 && 
                m_adjMatrix[minIndex][j] < minDist[j]) {
                minDist[j] = m_adjMatrix[minIndex][j];
                parent[j] = minIndex;
            }
        }
    }
    
    return mstEdges;
}

