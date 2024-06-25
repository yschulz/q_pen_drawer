#ifndef CANVAS_VIEW_H
#define CANVAS_VIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>

#include <QMouseEvent>
#include <QWheelEvent>

#include "q_pen_drawer/bezierline.h"

class CanvasView : public QGraphicsView{
    Q_OBJECT

    public:
        explicit CanvasView(QWidget* parent = nullptr);
        ~CanvasView();

    private slots:
        void updateAverageLength(int value);
        void setMinimumRadius(double value);

    private:
        void wheelEvent(QWheelEvent* event) override;
        void mousePressEvent(QMouseEvent* event) override;
        void mouseReleaseEvent(QMouseEvent* event) override;
        void mouseMoveEvent(QMouseEvent* event) override;

        QGraphicsScene* scene_;
        BezierLine* bezier_item_;
        bool control_drag_ = false;
};

#endif // CANVAS_VIEW_H
