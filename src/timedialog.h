#ifndef _TIMEDIALOG_H_
#define _TIMEDIALOG_H_
#pragma execution_character_set("utf-8")

#include <QDialog>
#include "ui_timedialog.h"
#include<QDebug>

class TimeDialog : public QDialog
{
	Q_OBJECT

public:
	TimeDialog(QWidget *parent = Q_NULLPTR);
	~TimeDialog();

	QString getDeadline();
	QString getRepeat();
	QString getRemind();
	void setRepeatCalendar(QString repeat);
	void setRemindCalendar();

public slots:
	void btnCancelClicked();
	void btnConfirmClicked();


private:
	Ui::TimeDialog ui;
	QString m_deadline;
	QString m_repeat;
	QString m_remind;
};
#endif