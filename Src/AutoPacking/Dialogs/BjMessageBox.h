#ifndef BJMESSAGEBOX_H
#define BJMESSAGEBOX_H
#include <QMessageBox>

class BjMessageBox : public QMessageBox
{

public:
    explicit BjMessageBox(QWidget *parent = 0);
    ~BjMessageBox();

private:
};

#endif // BJMESSAGEBOX_H
