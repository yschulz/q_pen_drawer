#include "q_pen_drawer/canvasview.h"

CanvasView::CanvasView(QWidget* parent): QGraphicsView(parent){
    scene_ = new QGraphicsScene(parent);
    QGraphicsView::setScene(scene_);
    QGraphicsView::setMouseTracking(true);
}

CanvasView::~CanvasView(){
}

void CanvasView::wheelEvent(QWheelEvent* event){
    QGraphicsView::wheelEvent(event);
    scene_->update();
}

void CanvasView::mousePressEvent(QMouseEvent* event){
    QGraphicsView::mousePressEvent(event);

    QPointF pressed = QGraphicsView::mapToScene(event->pos());

    if (event->button() == Qt::LeftButton) {
        control_drag_ = true;
    }

    scene_->update();

}

void CanvasView::mouseReleaseEvent(QMouseEvent* event){
    QGraphicsView::mouseReleaseEvent(event);
    if (event->button() == Qt::LeftButton) {
        control_drag_ = false;
    }
}

void CanvasView::mouseMoveEvent(QMouseEvent* event){
    QGraphicsView::mouseMoveEvent(event);
    QPointF drag = QGraphicsView::mapToScene(event->pos());
    if(control_drag_){
        scene_->update();
    }
}