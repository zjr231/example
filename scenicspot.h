#ifndef SCENICSPOT_H
#define SCENICSPOT_H

#include <QString>

class ScenicSpot
{
public:
    ScenicSpot();
    ScenicSpot(int id, const QString& name, const QString& description = "");
    
    // Getter方法
    int getId() const { return m_id; }
    QString getName() const { return m_name; }
    QString getDescription() const { return m_description; }
    
    // Setter方法
    void setId(int id) { m_id = id; }
    void setName(const QString& name) { m_name = name; }
    void setDescription(const QString& description) { m_description = description; }
    
private:
    int m_id;
    QString m_name;
    QString m_description;
};

#endif // SCENICSPOT_H

