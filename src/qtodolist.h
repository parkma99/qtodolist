#ifndef _QTODOLIST_H_
#define _QTODOLIST_H_
#pragma execution_character_set("utf-8")

#include <QtWidgets/QMainWindow>
#include "ui_qtodolist.h"
#include <QDebug>
#include <QMessageBox>
#include <QInputDialog>
#include "qitem.h"
#include "sqldata.h"

class QToDoList : public QMainWindow
{
    Q_OBJECT

public:
    QToDoList(QWidget *parent = Q_NULLPTR);

    void initUI();    QPushButton* initMenuButton();    void clearItemList();    void refreshHomeItemList();    void refreshGroupItemList(QString groupName);    void initItemId();    void addWidgetToLayout(QItemList itemList);    void addButtonToLayout(QPushButton* btnShow, bool isHideFinished);    void addGroupNameButton(QString groupName);    void refreshMenuBtn(QString groupName);signals:
    void showItemSideBarWidget(QItem* itemWidget);
public slots:
    void btnAddGroupClicked();    void btnRefreshClicked();    void comboSortChanged(int);    void lineAddItemEntered();    void showItemSetting(QItem* itemWidget);    void checkFinishChanged(QString itemGroupName);    void ItemDelete(QString itemGroupName);    void refreshItemList();    void btnShowNotSettedClicked();    void btnShowItemsInSevenDaysClicked();    void btnShowItemsOverSevenDaysClicked();    void btnShowFinishClicked();    void menuBtnClicked();
private:
    Ui::QToDoListClass ui;
    QList<QPushButton*> m_menuBtnList;    QPushButton* m_menuBtnCur = nullptr;    QString m_groupName = "我的待办";    int m_sortType = 0;    SqlData* m_sqldata= nullptr;    QWidget* m_widget = nullptr;    QVBoxLayout* m_layout = nullptr;    QMap<int, QItem*> m_QItemMap;    int m_itemId = -1;    QList<int> m_showStateList;    };
#endif
