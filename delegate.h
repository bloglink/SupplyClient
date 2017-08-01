#ifndef DELEGATE_H
#define DELEGATE_H

#include <QItemDelegate>
#include <QStandardItemModel>
#include <QModelIndex>
#include <QObject>
#include <QSize>
#include <QSpinBox>
#include <QLineEdit>
#include <QComboBox>
#include <QApplication>
#include <QListView>

#include <QItemDelegate>
#include <QStandardItemModel>
#include <QStyledItemDelegate>
#include <QSpinBox>
#include <QDateEdit>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QCalendarWidget>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QSqlRelationalTableModel>

//编号列，只读委托
class ReadOnlyDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    ReadOnlyDelegate(QObject *parent = 0): QItemDelegate(parent) { }
    QWidget *createEditor(QWidget*, const QStyleOptionViewItem &,
                          const QModelIndex &) const
    {
        return NULL;
    }
};

//代理类，把所有单元格中的字符居中显示
class StandardItemModel : public QStandardItemModel
{
    Q_OBJECT
public:
    StandardItemModel(QObject *parent=NULL) : QStandardItemModel(parent) { }
    StandardItemModel(int row, int column, QObject *parent=NULL)
        : QStandardItemModel(row, column, parent) { }
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const
    {
        if( Qt::TextAlignmentRole == role )
            return Qt::AlignCenter;
        return QStandardItemModel::data(index, role);
    }
};

//代理类，把所有单元格中的字符居中显示
class StandardSqlModel : public QSqlTableModel
{
    Q_OBJECT
public:
    StandardSqlModel(QObject * parent = 0, QSqlDatabase db = QSqlDatabase()) : QSqlTableModel(parent,db) { }
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const
    {
        if( Qt::TextAlignmentRole == role )
            return Qt::AlignCenter;
        return QSqlTableModel::data(index, role);
    }
};

//代理类，把所有单元格中的字符居中显示
class SqlTableModel : public QSqlRelationalTableModel
{
    Q_OBJECT
public:
    SqlTableModel(QObject * parent = 0, QSqlDatabase db = QSqlDatabase()) : QSqlRelationalTableModel(parent,db) { }
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const
    {
        if( Qt::TextAlignmentRole == role )
            return Qt::AlignCenter;
        return QSqlRelationalTableModel::data(index, role);
    }
};
//代理类，把所有单元格中的字符居中显示
class SqlQueryModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    SqlQueryModel(QObject * parent = 0) : QSqlQueryModel(parent) { }
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const
    {
        if( Qt::TextAlignmentRole == role )
            return Qt::AlignCenter;
        return QSqlQueryModel::data(index, role);
    }
};

//利用 QComboBox 委托对输入进行限制
class ComboBoxDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    QStringList headers;
    ComboBoxDelegate(QObject *parent = 0): QItemDelegate(parent) { }
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &,
                          const QModelIndex &index) const
    {
        if (index.column() == 0)
            return 0;
        QComboBox *editor = new QComboBox(parent);
        editor->setItemDelegate(new QStyledItemDelegate());
        editor->setEditable(true);
        for (int i=0; i < headers.size(); i++)
            editor->addItem(headers.at(i));
        return editor;
    }
    void setEditorData(QWidget *editor, const QModelIndex &index) const
    {
        QString text = index.model()->data(index, Qt::EditRole).toString();
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        int tindex = comboBox->findText(text);
        comboBox->setCurrentIndex(tindex);
    }
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const
    {
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        QString text = comboBox->currentText();
        model->setData(index, text, Qt::EditRole);
    }
    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option, const QModelIndex &) const
    {
        editor->setGeometry(option.rect);
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        comboBox->showPopup();
    }
    void setItemHeaders(QStringList heads) {
        headers = heads;
    }
};

//利用 QDateEdit 委托对输入进行限制
class DateEditDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    QStringList headers;
    DateEditDelegate(QObject *parent = 0): QItemDelegate(parent) { }
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &,
                          const QModelIndex &index) const
    {
        if (index.column() == 0)
            return 0;
        QDateEdit *editor = new QDateEdit(parent);
        editor->setCalendarPopup(true);
        editor->setDate(QDate::currentDate());
        editor->setDisplayFormat("yyyy-MM-dd");
        return editor;
    }
    void setEditorData(QWidget *editor, const QModelIndex &index) const
    {
        QDate value = index.model()->data(index, Qt::EditRole).toDate();
        QDateEdit *dateEdit = static_cast<QDateEdit*>(editor);
        dateEdit->setDate(value);
    }
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const
    {
        QDateEdit *dateEdit = static_cast<QDateEdit*>(editor);
        QString value = dateEdit->date().toString("yyyy-MM-dd");
        model->setData(index, value, Qt::EditRole);
    }
    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option, const QModelIndex &) const
    {
        editor->setGeometry(option.rect);
    }
};
#endif // DELEGATE_H
