#include "qitem.h"

QItem::QItem(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	m_database = new SqlData;
	initPriorityMap();
	(void)connect(ui.checkFinished, &QCheckBox::stateChanged, this, &QItem::checkFinishChanged);
	(void)connect(ui.btnDelete, &QPushButton::clicked, this, &QItem::deleteItem);
}

QItem::~QItem()
{
}

void QItem::initQItemUI(QMap<QString, QString> itemMap)
{
	m_itemMap = itemMap;	ui.checkFinished->setChecked(m_itemMap["finish"].toInt());
		if (!QString::compare(itemMap["priority"], "0"))
	{
		ui.labelPriority->hide();
	}
	else
	{
		ui.labelPriority->show();
		ui.labelPriority->setStyleSheet(m_priorityColorMap[itemMap["priority"]]);
		ui.labelPriority->setText(m_priorityTextMap[itemMap["priority"]]);
	}

	ui.labelName->setText(itemMap["name"]);	ui.labelRepeat->setText(m_repeatTextMap[itemMap["repeat"]] + "	|");	ui.labelGroupName->setText(itemMap["groupName"]);
		if (itemMap["deadline"].isEmpty())
	{
		ui.labelDeadline->setText(QString("无到期时间") + "	|");
	}
	else
	{
		ui.labelDeadline->setText(itemMap["deadline"] + "	|");
	}

		if (itemMap["remind"].isEmpty())
	{
		ui.labelRemind->setText(QString("无提醒时间") + "	|");
	}
	else
	{
		ui.labelRemind->setText(itemMap["remind"] + "	|");
	}
	
 }

void QItem::initPriorityMap()
{
	m_priorityTextMap["0"] = "!!!";
	m_priorityTextMap["1"] = "!!!";
	m_priorityTextMap["2"] = "!!";
	m_priorityTextMap["3"] = "!";

	m_priorityColorMap["0"] = "color:gray;";
	m_priorityColorMap["1"] = "color:red;";
	m_priorityColorMap["2"] = "color:orange;";
	m_priorityColorMap["3"] = "color:blue;";

	m_repeatTextMap["0"] = "不重复";
	m_repeatTextMap["1"] = "每天重复";
	m_repeatTextMap["2"] = "每周重复";
	m_repeatTextMap["3"] = "每月重复";
}


QMap<QString, QString> QItem::getItemMap()
{
	return m_itemMap;
}

int QItem::getItemId()
{
	return m_itemMap["id"].toInt();
}

void QItem::setItemChecked(bool isChecked)
{
	m_isCheckedFlag = isChecked;
		if (isChecked)
	{
        QPalette palette;
        palette.setColor(QPalette::ColorRole::Window, QColor(78, 78, 78));
		this->setAutoFillBackground(true);
		this->setPalette(palette);
	}
	else
	{
		this->setAutoFillBackground(false);
	}
}

void QItem::mouseReleaseEvent(QMouseEvent* event)
{
	emit showItemSettingSignal(this);}

void QItem::enterEvent(QEvent* event)
{
	if (m_isCheckedFlag)
	{
		return;
	}
		QPalette palette;
    palette.setColor(QPalette::ColorRole::Window, QColor(207, 207, 207));
	this->setAutoFillBackground(true);
	this->setPalette(palette);
}

void QItem::leaveEvent(QEvent* event)
{
	if (m_isCheckedFlag)
	{
		return;
	}
		this->setAutoFillBackground(false);
}

void QItem::checkFinishChanged(int index)
{
	index = index == 0 ? 0 : 1;	if (index == m_itemMap["finish"].toInt())
	{
		return;
	}
	m_itemMap["finish"] = QString::number(index);
	m_database->updateItemFinished(m_itemMap["id"].toInt(), index);	emit checkFinishChangedSignal(m_itemMap["groupName"]);}

void QItem::deleteItem()
{
	int itemId = m_itemMap["id"].toInt();
	m_database->deleteItem(itemId);
	emit currentItemDeleted(m_itemMap["groupName"]);
}
