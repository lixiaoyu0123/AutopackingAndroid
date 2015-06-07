#ifndef BJTABLE_MODEL_H
#define BJTABLE_MODEL_H
#include <QSqlTableModel>

class BjTableModel:public QSqlTableModel
{
	Q_OBJECT
public:
	BjTableModel(QObject *parent = 0, QSqlDatabase db = QSqlDatabase());
protected:
	Qt::ItemFlags flags(const QModelIndex &index) const;
private:
};
#endif