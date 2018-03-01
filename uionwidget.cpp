#include "uionwidget.h"
#include "ui_uionwidget.h"

UiOnWidget::UiOnWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UiOnWidget)
{
    ui->setupUi(this);
}

UiOnWidget::~UiOnWidget()
{
    delete ui;
}
