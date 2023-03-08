#ifndef MSOU_STATE_H
#define MSOU_STATE_H

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QButtonGroup>
#include <QTimer>


// Класс для отображения статуса работаспособности приборов
class StateButton : public QWidget
{
    Q_OBJECT

public:
    StateButton(QVector<QString> listName, QWidget *parent = 0);
    ~StateButton();

    bool SetToolTips(QStringList listTips);

    void setDefaultStyle(const QString& value);

signals:
    void ChangeSize();

public slots:
    void setIndicatorState(size_t index, bool state);
    // выполняет скрытия кнопок
    void HiddenButton();
    // устанавливает все индикаторы в красный цвет
    void SetDefault();

private:
    QButtonGroup* group;
    QHBoxLayout* layout;
    QTimer* m_timer;
    // количество кнопок
    size_t m_amount;
    QString m_style;
    QString m_defStyle;

};

class MSOU_State : public StateButton
{
    Q_OBJECT

public:
    MSOU_State(QWidget *parent = 0);
    ~MSOU_State();

};





#endif // MSOU_STATE_H
