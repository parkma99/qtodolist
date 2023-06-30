#include "sidebarwidget.h"

SideBarWidget::SideBarWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	m_database = new SqlData;
	(void)connect(ui.checkFinished, &QCheckBox::stateChanged, this, &SideBarWidget::checkFinishChanged);
	(void)connect(ui.comboPriority, static_cast<void(QComboBox::*)(int)>(&QComboBox::activated), this, &SideBarWidget::comboPriorityChanged);
	(void)connect(ui.btnDeadline, &QPushButton::clicked, this, &SideBarWidget::btnDeadlineClicked);
	(void)connect(ui.btnRemind, &QPushButton::clicked, this, &SideBarWidget::btnRemindClicked);
	(void)connect(ui.btnDescribe, &QPushButton::clicked, this, &SideBarWidget::btnDescribeClicked);
	(void)connect(ui.btnDescribeConfirm, &QPushButton::clicked, this, &SideBarWidget::btnDescribeConfirmClicked);
	(void)connect(ui.btnLocation, &QPushButton::clicked, this, &SideBarWidget::btnLocationClicked);
	(void)connect(ui.btnLocationConfirm, &QPushButton::clicked, this, &SideBarWidget::btnLocationConfirmClicked);
	(void)connect(ui.btnNote, &QPushButton::clicked, this, &SideBarWidget::btnNotesClicked);
	(void)connect(ui.btnNoteConfirm, &QPushButton::clicked, this, &SideBarWidget::btnNotesConfirmClicked);

}

SideBarWidget::~SideBarWidget()
{
}

void SideBarWidget::initSideBarUI(QItem* qitem)
{
	m_qitem = qitem;
	m_itemMap = qitem->getItemMap();
	ui.checkFinished->setChecked(m_itemMap["finish"].toInt());
	ui.labelName->setText(m_itemMap["name"]);
	ui.comboPriority->setCurrentIndex(m_itemMap["priority"].toInt());
	ui.lineDeadline->setText(m_itemMap["deadline"]);
	ui.lineRemind->setText(m_itemMap["remind"]);
	ui.textDescribe->setText(m_itemMap["describe"]);
	ui.btnDescribeConfirm->hide();
	ui.lineLocation->setText(m_itemMap["location"]);
	ui.btnLocationConfirm->hide();
	ui.textNotes->setText(m_itemMap["notes"]);
	ui.btnNoteConfirm->hide();
}

void SideBarWidget::checkFinishChanged(int index)
{
	index = index == 0 ? 0 : 1;	if (index == m_itemMap["finish"].toInt())
	{
		return;
	}
	m_itemMap["finish"] = QString::number(index);
	m_database->updateItemFinished(m_itemMap["id"].toInt(), index);
	emit checkFinishedChangedSignal(m_itemMap["groupName"]);
}

void SideBarWidget::comboPriorityChanged(int index)
{
	m_itemMap["priority"] = QString::number(index);
	m_database->updateItemPriority(m_itemMap["id"].toInt(), index);	m_qitem->initQItemUI(m_itemMap);}

void SideBarWidget::btnDeadlineClicked()
{
	TimeDialog dialog(this);	dialog.setRepeatCalendar(m_itemMap["repeat"]);	int ret = dialog.exec();
	if (ret == TimeDialog::Accepted)
	{
		m_itemMap["deadline"] = dialog.getDeadline();		m_itemMap["repeat"] = dialog.getRepeat();		m_database->updateDeadline(m_itemMap["id"].toInt(), m_itemMap["deadline"]);		m_database->updateRepeatTime(m_itemMap["id"].toInt(), m_itemMap["repeat"].toInt());		m_qitem->initQItemUI(m_itemMap);		ui.lineDeadline->setText(m_itemMap["deadline"]);
		emit checkFinishedChangedSignal(m_itemMap["groupName"]);
	}
}

void SideBarWidget::btnRemindClicked()
{
	TimeDialog dialog(this);
	dialog.setRemindCalendar();	int ret = dialog.exec();
	if (ret == TimeDialog::Accepted)
	{
		m_itemMap["remind"] = dialog.getRemind(); 		m_database->updateRemindTime(m_itemMap["id"].toInt(), m_itemMap["remind"]);		m_qitem->initQItemUI(m_itemMap);		ui.lineRemind->setText(m_itemMap["remind"]);
	}
}

void SideBarWidget::btnDescribeClicked()
{
	ui.btnDescribeConfirm->show();
	ui.textDescribe->setEnabled(true);
	ui.textDescribe->setFocus();
}

void SideBarWidget::btnDescribeConfirmClicked()
{
	m_itemMap["describe"] = ui.textDescribe->toPlainText();	m_database->updateDescribe(m_itemMap["id"].toInt(), m_itemMap["describe"]);
	ui.btnDescribeConfirm->hide();	ui.textDescribe->setEnabled(false);
}

void SideBarWidget::btnLocationClicked()
{
	ui.btnLocationConfirm->show();
	ui.lineLocation->setEnabled(true);
	ui.lineLocation->setFocus();
}

void SideBarWidget::btnLocationConfirmClicked()
{
	m_itemMap["location"] = ui.lineLocation->text();	m_database->updateLocation(m_itemMap["id"].toInt(), m_itemMap["location"]);
	ui.btnLocationConfirm->hide();	ui.lineLocation->setEnabled(false);
}

void SideBarWidget::btnNotesClicked()
{
	ui.btnNoteConfirm->show();
	ui.textNotes->setEnabled(true);
	ui.textNotes->setFocus();
}

void SideBarWidget::btnNotesConfirmClicked()
{
	m_itemMap["notes"] = ui.textNotes->toPlainText();	m_database->updateOtherNote(m_itemMap["id"].toInt(), m_itemMap["notes"]);
	ui.btnNoteConfirm->hide();	ui.textNotes->setEnabled(false);
}