#include "sqldata.h"
#include <qfile.h>
QSqlDatabase SqlData::m_db = QSqlDatabase::addDatabase("QSQLITE");

SqlData::SqlData() {
    QString db_file = "todo.db";
    bool isFirstOpen = !QFile::exists(db_file);
    m_db.setDatabaseName(db_file);
    if (!m_db.open())
    {
        qDebug() << __FUNCTION__ << m_db.lastError().text();
    }
    if (isFirstOpen) {
        QSqlQuery query(m_db);
        query.exec("CREATE TABLE todo (id INTEGER PRIMARY KEY, name TEXT NOT NULL, priority INTEGER DEFAULT (0), deadline TEXT, repeat INTEGER DEFAULT (0), remind TEXT, groupName TEXT NOT NULL, describe TEXT, location TEXT, notes TEXT, finish INTEGER DEFAULT (0));");
        query.exec("CREATE TABLE groupList (id INTEGER PRIMARY KEY, groupName TEXT NOT NULL);");
    }
}

bool SqlData::itemExists(QString eventName, QString eventGroupName)
{
	QSqlQuery query(m_db);
	query.prepare("SELECT * FROM todo WHERE name=:name and groupName=:groupName");
	query.bindValue(":name", eventName);
	query.bindValue(":groupName", eventGroupName);
	query.exec();
	if (query.next())
	{
		return true;
	}
	return false;

}

bool SqlData::addItem(QString eventName, QString eventGroupName)
{
	QSqlQuery query(m_db);
	query.prepare("INSERT INTO todo (name,groupName) VALUES (:name,:groupName)");
	query.bindValue(":name", eventName);
	query.bindValue(":groupName", eventGroupName);
    return query.exec();
}
bool SqlData::deleteItem(int eventId) 
{
	QSqlQuery query(m_db);
	query.prepare("DELETE FROM todo WHERE id=:id");
	query.bindValue(":id", eventId);
	return query.exec();
}

bool SqlData::updateItemFinished(int eventId, int finished)
{
	QSqlQuery query(m_db);
	query.prepare("UPDATE  todo  SET finish =:finish WHERE id =:id ");
	query.bindValue(":finish", finished);
	query.bindValue(":id", eventId);
	return query.exec();
}

bool SqlData::updateItemPriority(int eventId, int priority)
{
	QSqlQuery query(m_db);
	query.prepare("UPDATE  todo  SET priority =:priority WHERE id =:id ");
	query.bindValue(":priority", priority);
	query.bindValue(":id", eventId);
	return query.exec();
}

bool SqlData::updateDeadline(int eventId, QString deadline)
{
	QSqlQuery query(m_db);
	query.prepare("UPDATE  todo  SET deadline =:deadline WHERE id =:id ");
	query.bindValue(":deadline", deadline);
	query.bindValue(":id", eventId);
	return query.exec();
}

bool SqlData::updateRepeatTime(int eventId, int repeat)
{
	QSqlQuery query(m_db);
	query.prepare("UPDATE  todo  SET repeat =:repeat WHERE id =:id ");
	query.bindValue(":repeat", repeat);
	query.bindValue(":id", eventId);
	return query.exec();
}

bool SqlData::updateRemindTime(int eventId, QString remind)
{
	QSqlQuery query(m_db);
	query.prepare("UPDATE  todo  SET remind =:remind WHERE id =:id ");
	query.bindValue(":remind", remind);
	query.bindValue(":id", eventId);
	return query.exec();
}

bool SqlData::updateDescribe(int eventId, QString describe)
{
	QSqlQuery query(m_db);
	query.prepare("UPDATE  todo  SET describe =:describe WHERE id =:id ");
	query.bindValue(":describe", describe);
	query.bindValue(":id", eventId);
	return query.exec();
}

bool SqlData::updateLocation(int eventId, QString location)
{
	QSqlQuery query(m_db);
	query.prepare("UPDATE  todo  SET location =:location WHERE id =:id ");
	query.bindValue(":location", location);
	query.bindValue(":id", eventId);
	return query.exec();
}

bool SqlData::updateOtherNote(int eventId, QString notes)
{
	QSqlQuery query(m_db);
	query.prepare("UPDATE  todo  SET notes =:notes WHERE id =:id ");
	query.bindValue(":notes", notes);
	query.bindValue(":id", eventId);
	return query.exec();
}

bool SqlData::groupExists(QString groupName)
{
	QSqlQuery query(m_db);
	query.prepare("SELECT * FROM groupList WHERE groupName=:groupName");
	query.bindValue(":groupName", groupName);
	query.exec();
	if (query.next())
	{
		return true;
	}
	return false;
}

bool SqlData::addGroup(QString groupName)
{
	QSqlQuery query(m_db);
	query.prepare("INSERT INTO groupList (groupName) VALUES (:groupName)");
	query.bindValue(":groupName", groupName);
	return query.exec();
}

QList<QString> SqlData::getGroups() 
{
	QSqlQuery query(m_db);
	query.prepare("SELECT * FROM groupList ORDER BY id DESC");
	query.exec();
	QList<QString> list;
	while (query.next())
	{
		list.append(query.value("groupName").toString());
	}
	return list;
}

int SqlData::getNumNotFinishedByCurrentGroup(QString groupName)
{
	QSqlQuery query(m_db);
	query.prepare("SELECT * FROM todo WHERE groupName=:groupName and finish=0");
	query.bindValue(":groupName", groupName);
	query.exec();
	int count = 0;
	while (query.next())
	{
		++count;
	}
	return count;
}

QItemList SqlData::getItemsFromToDo(QSqlQuery query)
{
	QList<QMap<QString, QString>> list;
	while (query.next())
	{
		QMap<QString, QString> map;
		map["id"] = QString::number(query.value(0).toInt());
		map["name"] = query.value(1).toString();
		map["priority"] = QString::number(query.value(2).toInt());
		map["deadline"] = query.value(3).toString();
		map["repeat"] = QString::number(query.value(4).toInt());
		map["remind"] = query.value(5).toString();
		map["groupName"] = query.value(6).toString();
		map["describe"] = query.value(7).toString();
		map["location"] = query.value(8).toString();
		map["notes"] = query.value(9).toString();
		map["finish"] = query.value(10).toString();
		list.append(map);
	}
	QItemList itemList(list);
	return itemList;
}

QItemList SqlData::getItemsNotFinished()
{
	QSqlQuery query(m_db);
	query.prepare("SELECT * FROM todo WHERE finish=0 ORDER BY id DESC");
    query.exec();
    return getItemsFromToDo(std::move(query));
}

QItemList SqlData::getItemsByGroup(QString groupName)
{
	QSqlQuery query(m_db);
	query.prepare("SELECT * FROM todo WHERE groupName=:groupName ORDER BY id DESC");
	query.bindValue(":groupName", groupName);
	query.exec();
    return getItemsFromToDo(std::move(query));
}
