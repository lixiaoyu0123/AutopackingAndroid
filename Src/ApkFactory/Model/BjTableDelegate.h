#ifndef BJTABLE_DELEGATE_H
#define BJTABLE_DELEGATE_H
#include <QStyledItemDelegate>

class BjTableDelegate:public QStyledItemDelegate
{
	Q_OBJECT
public: 
	explicit BjTableDelegate(QObject *parent = 0);
protected:
	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex & index) const;
private:
};
#endif