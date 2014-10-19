#ifndef BJID_DELEGATE_H
#define BJID_DELEGATE_H
#include <QStyledItemDelegate>

class BjIdDelegate :public QStyledItemDelegate
{
	Q_OBJECT
public:
	explicit BjIdDelegate(QObject *parent = 0);
protected:
	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
private:
};
#endif