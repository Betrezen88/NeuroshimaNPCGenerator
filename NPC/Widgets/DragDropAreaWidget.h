#ifndef DRAGDROPAREAWIDGET_H
#define DRAGDROPAREAWIDGET_H

#include <QWidget>

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QLabel>

class DragDropAreaWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DragDropAreaWidget(QWidget *parent = nullptr);

    void addLabel(const QString &text);
    int value() const;
    bool hasLabel() const;

signals:
    void hasLabelChanged();

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *) override;

private:
    void changeHasLabel( bool value );

private:
    bool m_hasLabel{false};
    QLabel *m_pLabel{nullptr};
    const QString m_style {
        "DragDropAreaWidget{"
        "border: 1px solid black;"
        "}"
    };
};

#endif // DRAGDROPAREAWIDGET_H
