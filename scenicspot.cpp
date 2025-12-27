#include "scenicspot.h"

ScenicSpot::ScenicSpot() : m_id(0) {}

ScenicSpot::ScenicSpot(int id, const QString& name, const QString& description)
    : m_id(id), m_name(name), m_description(description) {}

