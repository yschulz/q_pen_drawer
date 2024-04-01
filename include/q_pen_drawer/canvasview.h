#ifndef CANVAS_VIEW_H
#define CANVAS_VIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>

#include <QMouseEvent>
#include <QWheelEvent>

class CanvasView : public QGraphicsView{
    Q_OBJECT

    public:
        explicit CanvasView(QWidget* parent = nullptr);
        ~CanvasView();

    private:
        void wheelEvent(QWheelEvent* event) override;
        void mousePressEvent(QMouseEvent* event) override;
        void mouseReleaseEvent(QMouseEvent* event) override;
        void mouseMoveEvent(QMouseEvent* event) override;

        QGraphicsScene* scene_;
        bool control_drag_ = false;
};

#endif // CANVAS_VIEW_H
