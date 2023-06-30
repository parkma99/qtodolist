#ifndef _QITEMLIST_H_
#define _QITEMLIST_H_

#include<QList>
#include<QMap>
#include<QDateTime>
#include<QDebug>
#include <algorithm>



class QItemList
{
public:
	QItemList(QList<QMap<QString, QString>> itemList);


	int getSize();
	QMap<QString, QString> getItemData(int index);
	QItemList getItemNotSetted();	QItemList getItemInSevenDays();	QItemList getItemOverSevenDays();	QItemList getItemNotFinished();	QItemList getItemFinished();
	void sortByDifferentType(int type);
private:
	QList<QMap<QString, QString>> m_list;	int m_size;};

#endif
