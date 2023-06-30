#include "qitemlist.h"

QItemList::QItemList(QList<QMap<QString, QString>> itemList):
	m_list(itemList),
	m_size(itemList.size())
{
}

int QItemList::getSize() {
	return m_size;
}

QMap<QString, QString> QItemList::getItemData(int index)
{
	if (index >= 0 && index < m_size)
	{
		return m_list[index];
	}
	else
	{
		return QMap<QString, QString>();
	}
}

QItemList QItemList::getItemNotSetted()
{
	QList<QMap<QString, QString>> list;
	for (int i = 0; i < m_size; ++i)
	{
		if (m_list[i]["deadline"].isEmpty())
		{
			list.append(m_list[i]);
		}
	}
	return list;
}

QItemList QItemList::getItemInSevenDays()
{
	QList<QMap<QString, QString>> list;
	QDateTime overSevenDateTime = QDateTime::currentDateTime().addDays(7);;
	for (int i = 0; i < m_size; ++i)
	{
		if (m_list[i]["deadline"].isEmpty())
		{
			continue;
		}
		QString expireStr = m_list[i]["deadline"].split(" ")[0];
		QDateTime datetime = QDateTime::fromString(expireStr, "yyyy-MM-dd");
		if (overSevenDateTime >= datetime)		{
			list.append(m_list[i]);
		}
	}
	return list;
}

QItemList QItemList::getItemOverSevenDays()
{
	QList<QMap<QString, QString>> list;
	QDateTime overSevenDateTime = QDateTime::currentDateTime().addDays(7);;
	for (int i = 0; i < m_size; ++i)
	{
		if (m_list[i]["deadline"].isEmpty())
		{
			continue;
		}
		QString expireStr = m_list[i]["deadline"].split(" ")[0];
		QDateTime datetime = QDateTime::fromString(expireStr, "yyyy-MM-dd");
		if (overSevenDateTime < datetime)		{
			list.append(m_list[i]);
		}
	}
	return list;
}

QItemList QItemList::getItemNotFinished()
{
	QList<QMap<QString, QString>> list;
	for (int i = 0; i < m_size; ++i)
	{
		if (!QString::compare(m_list[i]["finish"], "0"))
		{
			list.append(m_list[i]);
		}
	}
	return list;
}

QItemList QItemList::getItemFinished()
{
	QList<QMap<QString, QString>> list;
	for (int i = 0; i < m_size; ++i)
	{
		if (!QString::compare(m_list[i]["finish"], "1"))
		{
			list.append(m_list[i]);
		}
	}
	return list;
}

bool compareByDeadline(QMap<QString, QString> itemA, QMap<QString, QString> itemB)
{
	if (itemA["deadline"].isEmpty() && itemB["deadline"].isEmpty())
	{
		return itemA["id"].toInt() > itemB["id"].toInt();
	}
	if (itemA["deadline"].isEmpty()&&!itemB["deadline"].isEmpty())
	{
		return false;
	}
	if (itemB["deadline"].isEmpty()&&!itemA["deadline"].isEmpty())
	{
		return true;
	}
	QString deadline = itemA["deadline"].split(" ")[0];
	QDateTime deadlineItemA = QDateTime::fromString(deadline, "yyyy-MM-dd");
	deadline = itemB["deadline"].split(" ")[0];
	QDateTime deadlineItemB = QDateTime::fromString(deadline, "yyyy-MM-dd");
	return deadlineItemA > deadlineItemB;
}

bool compareByCreatedTime(QMap<QString, QString> itemA, QMap<QString, QString> itemB)
{
	return itemA["id"].toInt() > itemB["id"].toInt();
}

bool compareByPriority(QMap<QString, QString> itemA, QMap<QString, QString> itemB)
{
	if (itemA["priority"].isEmpty() && itemB["priority"].isEmpty())
	{
		return itemA["id"].toInt() > itemB["id"].toInt();
	}
	if (itemA["priority"].isEmpty())
	{
		return false;
	}
	if (itemB["priority"].isEmpty())
	{
		return true;
	}
	if (itemA["priority"].toInt() == 0 && itemB["priority"].toInt() == 0)
	{
		return itemA["id"].toInt() > itemB["id"].toInt();
	}
	if (itemA["priority"].toInt() == 0 && itemB["priority"].toInt() != 0)
	{
		return false;
	}
	if (itemA["priority"].toInt() != 0 && itemB["priority"].toInt() == 0)
	{
		return true;

	}
	return itemA["priority"].toInt() < itemB["priority"].toInt();
}

void QItemList::sortByDifferentType(int type)
{
	if (m_size < 2)
	{
		return;
	}

	if (type == 0)
	{
		std::sort(m_list.begin(), m_list.end(), compareByDeadline);
	}
	else if (type == 1)
	{
		std::sort(m_list.begin(), m_list.end(), compareByCreatedTime);
	}
	else
	{
		std::sort(m_list.begin(), m_list.end(), compareByPriority);
	}
}