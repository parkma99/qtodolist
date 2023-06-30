#ifndef _QITEM_H_
#define _QITEM_H_
#pragma execution_character_set("utf-8")

#include <QWidget>
#include "ui_qitem.h"
#include "sqldata.h"
#include <QDebug>

class QItem : public QWidget
{
	Q_OBJECT

public:
	QItem(QWidget *parent = Q_NULLPTR);
	~QItem();
	void initPriorityMap();	void initQItemUI(QMap<QString, QString> itemMap);	QMap<QString, QString> getItemMap();	int getItemId();	void setItemChecked(bool isChecked);	void deleteItem();
signals:
	void showItemSettingSignal(QItem* itemWidget);	void checkFinishChangedSignal(QString itemGroupName);	void currentItemDeleted(QString itemGroupName);
protected:
	virtual void mouseReleaseEvent(QMouseEvent* event);	virtual void enterEvent(QEvent* event);	virtual void leaveEvent(QEvent* event);
public slots:
	void checkFinishChanged(int index);
private:
	Ui::QItem ui;
	SqlData* m_database=nullptr;	QMap<QString, QString> m_itemMap;	QMap<QString, QString> m_priorityTextMap;	QMap<QString, QString> m_priorityColorMap;	QMap<QString, QString> m_repeatTextMap;	bool m_isCheckedFlag = false;};

#endif