#include "qtodolist.h"

QToDoList::QToDoList(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    m_sqldata = new SqlData;
    m_showStateList << 1 << 0 << 0 << 0;
    initUI();
}

void QToDoList::initUI()
{
    (void)connect(ui.btnRefresh, &QPushButton::clicked, this, &QToDoList::btnRefreshClicked);
    (void)connect(ui.comboBoxSort, static_cast<void(QComboBox::*)(int)>(&QComboBox::activated), this, &QToDoList::comboSortChanged);
    (void)connect(ui.lineAddItem, &QLineEdit::returnPressed, this, &QToDoList::lineAddItemEntered);
    (void)connect(this, &QToDoList::showItemSideBarWidget, ui.widget, &SideBarWidget::initSideBarUI);
    (void)connect(ui.widget, &SideBarWidget::checkFinishedChangedSignal, this, &QToDoList::checkFinishChanged);

    initMenuButton();
    ui.comboBoxSort->hide();
    refreshItemList();
}

QPushButton* QToDoList::initMenuButton()
{
    m_menuBtnList.clear();    QWidget* widget = new QWidget(this);    QVBoxLayout* layout = new QVBoxLayout(widget);    QPushButton* btn;    QPushButton* btnFirstGroup;

    btn = new QPushButton(this);
    btn->setText("我的首页");    (void)connect(btn, &QPushButton::clicked, this, &QToDoList::menuBtnClicked);
    layout->addWidget(btn);    m_menuBtnCur = btn;    btnFirstGroup = btn;
    m_groupName = "我的待办";
    m_menuBtnList.append(btn);
    btn = new QPushButton(this);    int itemNum = m_sqldata->getNumNotFinishedByCurrentGroup("我的待办");    btn->setText(QString("我的待办") + "(" + QString::number(itemNum) + ")");    (void)connect(btn, &QPushButton::clicked, this, &QToDoList::menuBtnClicked);
    layout->addWidget(btn);    m_menuBtnList.append(btn);
        QFrame* line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    layout->addWidget(line);
    QList<QString> groupList = m_sqldata->getGroups();    for (int i = 0; i < groupList.size(); ++i)
    {
        int itemNum = m_sqldata->getNumNotFinishedByCurrentGroup(groupList[i]);        btn = new QPushButton(this);        btn->setText(groupList[i] + "(" + QString::number(itemNum) + ")");        (void)connect(btn, &QPushButton::clicked, this, &QToDoList::menuBtnClicked);
        layout->addWidget(btn);        m_menuBtnList.append(btn);        if (i == 0)
        {
            btnFirstGroup = btn;
        }
    }

    btn = new QPushButton(this);    btn->setText("添加分组");    (void)connect(btn, &QPushButton::clicked , this, &QToDoList::btnAddGroupClicked);
    layout->addWidget(btn);
    layout->addStretch();    ui.scrollAreaMenus->setWidget(widget);
    return btnFirstGroup;}

void QToDoList::btnAddGroupClicked()
{
    bool isOK;
    QString groupName = QInputDialog::getText(
        this,
        QString("新建分组"),
        QString("分组名"),
        QLineEdit::Normal,
        QString(""),
        &isOK).simplified();
        if (isOK && !groupName.isEmpty())
    {
        if (m_sqldata->groupExists(groupName))
        {
            QMessageBox::warning(this, "错误", "该分组已存在！");
            return;
        }
        addGroupNameButton(groupName);    }
}

void QToDoList::btnRefreshClicked()
{
    m_showStateList.clear();
    m_showStateList << 1 << 0 << 0 << 0;
    refreshItemList();}

void QToDoList::comboSortChanged(int index)
{
    m_sortType = index;    refreshItemList();}

void QToDoList::lineAddItemEntered()
{
    QString itemName = ui.lineAddItem->text().simplified();
    if (!QString::compare(itemName, ""))
    {        return;
    }
    if (!m_sqldata->addItem(itemName, m_groupName))
    {
        QMessageBox::warning(this, "错误", "新建失败！");
        return;
    }
    refreshMenuBtn(m_groupName);
    refreshItemList();
    ui.lineAddItem->clear();}

void QToDoList::showItemSetting(QItem* itemWidget)
{
    if (itemWidget->getItemId() == m_itemId)
    {
        return;
    }
    if (m_itemId != -1)
    {
        m_QItemMap[m_itemId]->setItemChecked(false);    }
    m_itemId = itemWidget->getItemId();
    itemWidget->setItemChecked(true);    ui.widget->show();    emit showItemSideBarWidget(itemWidget);}

void QToDoList::checkFinishChanged(QString itemGroupName)
{
    refreshMenuBtn(itemGroupName);
    refreshItemList();
}

void QToDoList::ItemDelete(QString itemGroupName)
{
    refreshMenuBtn(itemGroupName);
    refreshItemList();
}

void QToDoList::btnShowNotSettedClicked()
{
    m_showStateList[0] = m_showStateList[0] == 0;
    refreshItemList();
}

void QToDoList::btnShowItemsInSevenDaysClicked()
{
    m_showStateList[1] = m_showStateList[1] == 0;
    refreshItemList();
}

void QToDoList::btnShowItemsOverSevenDaysClicked()
{
    m_showStateList[2] = m_showStateList[2] == 0;
    refreshItemList();
}

void QToDoList::btnShowFinishClicked()
{
    m_showStateList[3] = m_showStateList[3] == 0;
    refreshItemList();
}

void QToDoList::clearItemList()
{
    m_QItemMap.clear();
    if (m_layout)
    {
        delete m_layout;
    }
    if (m_widget)
    {
        delete m_widget;
    }
}

void QToDoList::refreshItemList()
{
    clearItemList();
    m_widget = new QWidget(this);
    m_layout = new QVBoxLayout(m_widget);
        if (m_menuBtnCur->text() == "我的首页")
    {
        refreshHomeItemList();
    }
    else if (m_menuBtnCur->text() == "我的待办")
    {
        refreshGroupItemList("我的待办");
    }
    else
    {
        refreshGroupItemList(m_groupName);
    }
}


void QToDoList::refreshHomeItemList()
{
    QItemList itemList = m_sqldata->getItemsNotFinished();    QItemList itemListNotSetted = itemList.getItemNotSetted();    QItemList itemListInSevenDays = itemList.getItemInSevenDays();    QItemList itemListOverSevenDays = itemList.getItemOverSevenDays();
    if (itemListNotSetted.getSize() > 0)
    {
        QPushButton* btnShow = new QPushButton(m_widget);        btnShow->setStyleSheet("QPushButton {text-align : left;background-color:rgba(0,0,0,0);}");
        (void)connect(btnShow, &QPushButton::clicked, this, &QToDoList::btnShowNotSettedClicked);        addButtonToLayout(btnShow, false);        if (m_showStateList[0] == 1)
        {
            btnShow->setText(QString("﹀未安排"));
            addWidgetToLayout(itemListNotSetted);
        }
        else if (m_showStateList[0] == 0 && itemListNotSetted.getSize() > 0)
        {
            btnShow->setText(QString("︿未安排"));
        }
    }

    if (itemListInSevenDays.getSize() > 0)
    {
        QPushButton* btnShow = new QPushButton(m_widget);        btnShow->setStyleSheet("QPushButton {text-align : left;background-color:rgba(0,0,0,0);}");
        (void)connect(btnShow, &QPushButton::clicked, this, &QToDoList::btnShowItemsInSevenDaysClicked);
        addButtonToLayout(btnShow, false);;        if (m_showStateList[1] == 1)
        {
            btnShow->setText(QString("﹀未来七天"));
            addWidgetToLayout(itemListInSevenDays);
        }
        else
        {
            btnShow->setText(QString("︿未来七天"));
        }
    }


    if (itemListOverSevenDays.getSize() > 0)
    {
        QPushButton* btnShow = new QPushButton(m_widget);        btnShow->setStyleSheet("QPushButton {text-align : left;background-color:rgba(0,0,0,0);}");
        (void)connect(btnShow, &QPushButton::clicked, this, &QToDoList::btnShowItemsOverSevenDaysClicked);
        addButtonToLayout(btnShow, false);;        if (m_showStateList[2] == 1)
        {
            btnShow->setText(QString("﹀以后"));
            addWidgetToLayout(itemListOverSevenDays);
        }
        else
        {
            btnShow->setText(QString("︿以后"));
        }
    }

    m_layout->addStretch();    ui.scrollAreaItemList->setWidget(m_widget);
    initItemId();
}

void QToDoList::refreshGroupItemList(QString groupName)
{
    QItemList itemList = m_sqldata->getItemsByGroup(groupName);
    itemList.sortByDifferentType(m_sortType);
    QItemList itemListNotFinished = itemList.getItemNotFinished();
    QItemList itemListFinished = itemList.getItemFinished();
    addWidgetToLayout(itemListNotFinished);    if (itemListFinished.getSize() > 0)
    {
        QPushButton* btnShow = new QPushButton(m_widget);
        btnShow->setStyleSheet("QPushButton {background-color:rgba(0,0,0,0);}");
        (void)connect(btnShow, &QPushButton::clicked, this, &QToDoList::btnShowFinishClicked);
        addButtonToLayout(btnShow, true);        if (m_showStateList[3] == 1)
        {
            btnShow->setText(QString("隐藏已完成︿"));
            addWidgetToLayout(itemListFinished);
        }
        else
        {
            btnShow->setText(QString("显示已完成﹀"));
        }
    }
    m_layout->addStretch();    ui.scrollAreaItemList->setWidget(m_widget);    initItemId();
}

void QToDoList::initItemId()
{
    ui.widget->hide();
    if (m_itemId == -1)
    {
        return;
    }
    QList<int> idList = m_QItemMap.keys();    for (int i = 0; i < idList.size(); ++i)
    {
        if (idList[i] == m_itemId)
        {
            m_QItemMap[m_itemId]->setItemChecked(true);
            ui.widget->show();            emit showItemSideBarWidget(m_QItemMap[m_itemId]);            return;
        }
    }
    m_itemId = -1;
}

void QToDoList::addButtonToLayout(QPushButton* btnShow, bool isHideFinished)
{
    QHBoxLayout* layoutShow = new QHBoxLayout;    if (isHideFinished)
    {        QFrame* line = new QFrame();
        line->setFrameShape(QFrame::HLine);
        layoutShow->addWidget(line);
        layoutShow->addWidget(btnShow);
        line = new QFrame();
        line->setFrameShape(QFrame::HLine);
        layoutShow->addWidget(line);
    }
    else
    {        layoutShow->addWidget(btnShow);
    }
    m_layout->addLayout(layoutShow);}

void QToDoList::addWidgetToLayout(QItemList itemList)
{
        for (int i = 0; i < itemList.getSize(); ++i)
    {
        QItem* item = new QItem;
        item->initQItemUI(itemList.getItemData(i));
        (void)connect(item, &QItem::showItemSettingSignal, this, &QToDoList::showItemSetting);
        (void)connect(item, &QItem::checkFinishChangedSignal, this, &QToDoList::checkFinishChanged);
        (void)connect(item, &QItem::currentItemDeleted, this, &QToDoList::ItemDelete);
        m_layout->addWidget(item);
        m_QItemMap[itemList.getItemData(i)["id"].toInt()] = item;
    }
}


void QToDoList::addGroupNameButton(QString groupName)
{
        if (!m_sqldata->addGroup(groupName))
    {
        QMessageBox::warning(this, "错误", "新建失败！");
        return;
    }
    QPushButton* FirstGroup = initMenuButton();    m_menuBtnCur = FirstGroup;
    m_groupName = groupName;
    ui.labelPageTitle->setText(groupName);    ui.comboBoxSort->show();    refreshItemList();}

void QToDoList::refreshMenuBtn(QString groupName)
{
        for (int i = 0; i < m_menuBtnList.size(); ++i)
    {
        QString btnText = m_menuBtnList[i]->text();
        if (!QString::compare(btnText.split("(")[0], groupName))
        {
            int itemNum = m_sqldata->getNumNotFinishedByCurrentGroup(groupName);
            m_menuBtnList[i]->setText(groupName + "(" + QString::number(itemNum) + ")");
            return;
        }

    }
}

void QToDoList::menuBtnClicked()
{
    QPushButton* btn = qobject_cast<QPushButton*>(sender());    if (btn == m_menuBtnCur)
    {        return;
    }
    m_menuBtnCur = btn;    QString btnText = btn->text();
    if (!QString::compare(btnText, "我的首页"))
    {        m_groupName = "我的待办";
        ui.labelPageTitle->setText(QString("我的首页"));        ui.comboBoxSort->hide();    }
    else
    {        m_groupName = btnText.split("(")[0];
        ui.labelPageTitle->setText(m_groupName);        ui.comboBoxSort->show();    }
    refreshItemList();}
