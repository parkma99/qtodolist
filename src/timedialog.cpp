#include "timedialog.h"

TimeDialog::TimeDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	(void)connect(ui.btnCancel, &QPushButton::clicked, this, &TimeDialog::btnCancelClicked);
	(void)connect(ui.btnConfirm, &QPushButton::clicked, this, &TimeDialog::btnConfirmClicked);
}

TimeDialog::~TimeDialog()
{
}

QString TimeDialog::getDeadline()
{
	return m_deadline;
}

QString TimeDialog::getRepeat()
{
	return m_repeat;
}

QString TimeDialog::getRemind()
{
	return m_remind;
}

void TimeDialog::setRepeatCalendar(QString repeat)
{
	ui.timeRemind->hide();
	ui.comboRepeat->show();
	ui.comboRepeat->setCurrentIndex(repeat.toInt());
}

void TimeDialog::setRemindCalendar()
{
	ui.comboRepeat->hide();
	ui.timeRemind->show();
}

void TimeDialog::btnCancelClicked()
{
	done(Rejected);
}

void TimeDialog::btnConfirmClicked()
{
	m_deadline = ui.calendarWidget->selectedDate().toString("yyyy-MM-dd ddd");
	m_repeat = QString::number(ui.comboRepeat->currentIndex());
	m_remind = ui.calendarWidget->selectedDate().toString("yyyy-MM-dd ");
	m_remind = m_remind + ui.timeRemind->time().toString("hh:mm");
	done(Accepted);
}