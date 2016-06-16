#pragma once
#include <QWidget>
#include "ui_aboutform.h"

class AboutForm : public QWidget {
	Q_OBJECT

public:
	AboutForm(QWidget * parent = Q_NULLPTR);
	~AboutForm();

private:
	Ui::AboutForm ui;
};
