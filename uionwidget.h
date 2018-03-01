#ifndef UIONWIDGET_H
#define UIONWIDGET_H

#include <QWidget>

namespace Ui {
class UiOnWidget;
}

class UiOnWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UiOnWidget(QWidget *parent = 0);
    ~UiOnWidget();

private:
    Ui::UiOnWidget *ui;
};

#endif // UIONWIDGET_H
