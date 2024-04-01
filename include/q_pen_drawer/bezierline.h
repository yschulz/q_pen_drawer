#ifndef BEZIERLINE_H
#define BEZIERLINE_H

#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include <QList>
#include <QVector>
#include <QPointF>
#include <QRectF>
#include <QPair>

struct BezierPoint{
    QPointF position;
    QPointF control_before;
    QPointF control_after;
};

enum UpdateMode{
    ControlBefore,
    Position,
    ControlAfter

};

class BezierLine : public QGraphicsItem
{
    public:
        BezierLine();

        QRectF boundingRect() const override;

        void addPoint(QPointF point);
        void updateControlPoint(QPointF point);
        inline void setAverageStep(int value){average_step_length_ = value;}

        bool selectClosest(QPointF point);

        QList<QPointF> getPoints();

        void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    private:
        QVector<BezierPoint> points_;

        QPair<qint16, UpdateMode> active_point_;
        qint16 point_size_ = 10;
        int average_step_length_ = 20;
};

#endif // BEZIERLINE_H
