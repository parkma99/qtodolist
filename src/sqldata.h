#ifndef _SQLDATA_H_
#define _SQLDATA_H_

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
#include<QList>
#include<QMap>
#include<QString>
#include"qitemlist.h"


class SqlData
{
public:
	SqlData();

	bool itemExists(QString eventName, QString eventGroupName);	bool addItem(QString eventName, QString eventGroupName);	bool deleteItem(int eventId);
	bool updateItemFinished(int eventId, int finished);	bool updateItemPriority(int eventId, int priority);	bool updateDeadline(int eventId, QString deadline);	bool updateRepeatTime(int eventId, int repeat);	bool updateRemindTime(int eventId, QString remind);	bool updateDescribe(int eventId, QString describe);	bool updateLocation(int eventId, QString location);	bool updateOtherNote(int eventId, QString notes);
	QItemList getItemsFromToDo(QSqlQuery query);	QItemList getItemsNotFinished();	QItemList getItemsByGroup(QString groupName);
	bool groupExists(QString groupName);	bool addGroup(QString groupName);	QList<QString> getGroups();	int getNumNotFinishedByCurrentGroup(QString groupName);
private:
	static QSqlDatabase m_db;
};

#endif
