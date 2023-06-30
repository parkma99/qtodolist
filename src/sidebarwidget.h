#ifndef _SIDEBRAWIDGET_H_
#define _SIDEBRAWIDGET_H_
#pragma execution_character_set("utf-8")

#include <QWidget>
#include "ui_sidebarwidget.h"
#include<QDebug>
#include <QDialog>
#include<QCheckBox>

#include"qitem.h"
#include"sqldata.h"
#include"timedialog.h"

class SideBarWidget : public QWidget
{
	Q_OBJECT

public:
	SideBarWidget(QWidget *parent = Q_NULLPTR);
	~SideBarWidget();
signals:
	void checkFinishedChangedSignal(QString groupName);
	 
public slots:
	void initSideBarUI(QItem* qitem);
	void checkFinishChanged(int index);	void comboPriorityChanged(int index);	void btnDeadlineClicked();	void btnRemindClicked();	void btnDescribeClicked();	void btnDescribeConfirmClicked();	void btnLocationClicked();	void btnLocationConfirmClicked();	void btnNotesClicked();	void btnNotesConfirmClicked();
private:
	Ui::SideBarWidget ui;
	SqlData* m_database = nullptr;	QItem* m_qitem= nullptr;	QMap<QString, QString> m_itemMap;};

#endif