#ifndef ABOUT_H
#define ABOUT_H

#include <QtCore/QDateTime>
#include <QtWidgets/QDialog>
#include "ui_About.h"

class About : public QDialog
{
    Q_OBJECT

public:

    /**
     * Constructor.
     *
     * @param parent The dialog's parent.
     */
	explicit About(QWidget *parent = 0);
	~About();

private:

    /**
     * Returns the time that Sigil was built in UTC.
     *
     * @return The build time in UTC.
     */
    static QDateTime GetUTCBuildTime();

    /**
     * Converts a three letter string like "Jun"
     * into that month's index.
     *
     * @param three_letter_string The month string.
     * @return The index of that month, 1-12.
     */
    static int MonthIndexFromString(const QString &three_letter_string);

    /**
     * Holds all the widgets Qt Designer created for us.
     */
    Ui::About *ui;
};

#endif // ABOUT_H


