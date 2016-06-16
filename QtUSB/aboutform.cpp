#include "aboutform.hpp"

AboutForm::AboutForm(QWidget * parent) : QWidget(parent) {
	ui.setupUi(this);
	QPixmap pixmap(":/image/Resources/images/zjupng100x100.png");
	ui.label_13->setPixmap(pixmap);
	ui.label_13->show();
}

AboutForm::~AboutForm() {
	
}
