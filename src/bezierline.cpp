#include "q_pen_drawer/bezierline.h"

#include <algorithm>
#include <cmath>
#include <QtMath>
#include <iostream>
#include <QTransform>
#include <Eigen/Core>
#include <Eigen/LU>

BezierLine::BezierLine(){
}

QRectF BezierLine::boundingRect() const {
    if(points_.begin() == points_.end()){
        return QRectF(0.0, 0.0, 0.0, 0.0);
    }
    else{
        auto x_min = std::min_element(points_.begin(), points_.end(), [](const BezierPoint &a, const BezierPoint &b){
            return a.position.x() < b.position.x();
        })->position.x();
        auto x_max = std::max_element(points_.begin(), points_.end(), [](const BezierPoint &a, const BezierPoint &b){
            return a.position.x() > b.position.x();
        })->position.x();
        auto y_min = std::min_element(points_.begin(), points_.end(), [](const BezierPoint &a, const BezierPoint &b){
            return a.position.y() < b.position.y();
        })->position.y();
        auto y_max = std::max_element(points_.begin(), points_.end(), [](const BezierPoint &a, const BezierPoint &b){
            return a.position.y() > b.position.y();
        })->position.y();
        return QRectF(x_min-5, y_min-5, x_max - x_min + 5, y_max - y_min + 5);
    }
}

bool BezierLine::selectClosest(QPointF point){
    for(int i=0 ;  i<points_.size(); i++){
        if(std::hypot((points_[i].position - point).x(), (points_[i].position - point).y()) < 5){
            active_point_ = qMakePair(i, UpdateMode::Position);
            return true;
        }
        else if(std::hypot((points_[i].control_before - point).x(), (points_[i].control_before - point).y()) < 5){
            active_point_ = qMakePair(i, UpdateMode::ControlBefore);
            return true;
        }
        else if(std::hypot((points_[i].control_after - point).x(), (points_[i].control_after - point).y()) < 5){
            active_point_ = qMakePair(i, UpdateMode::ControlAfter);
            return true;
        }
    }
    active_point_ = qMakePair(-1, UpdateMode::ControlAfter);
    return false;
}

void BezierLine::addPoint(QPointF point){
    points_.push_back({point, point, point});
    active_point_ = qMakePair(-1, UpdateMode::ControlAfter);
}

void BezierLine::updateControlPoint(QPointF point){
    if(active_point_.first == -1){
        auto current_point_iterator = points_.end()-1;
        current_point_iterator->control_after = point;
        current_point_iterator->control_before = 2 * current_point_iterator->position - point;
    }
    else{
        switch (active_point_.second) {
            case UpdateMode::Position: {
                QPointF delta = point - points_[active_point_.first].position;
                points_[active_point_.first].position += delta;
                points_[active_point_.first].control_after += delta;
                points_[active_point_.first].control_before += delta;
                break;
            }
            case UpdateMode::ControlAfter: {
                QPointF w = point - points_[active_point_.first].position;
                QPointF v = points_[active_point_.first].control_after - points_[active_point_.first].position;

                points_[active_point_.first].control_after = point;
                qreal delta = qAtan2(w.y() * v.x() - w.x() * v.y(), w.x() * v.x() + w.y() * v.y());

                qreal cosa = std::cos(delta);
                qreal sina = std::sin(delta);

                QTransform translate(1, 0, 0, 1, -points_[active_point_.first].position.x(), -points_[active_point_.first].position.y());
                QTransform rotate(cosa, sina, -sina, cosa, 0, 0);
                QTransform back_translate(1, 0, 0, 1, points_[active_point_.first].position.x(), points_[active_point_.first].position.y());

                QTransform transform = translate * rotate * back_translate;

                points_[active_point_.first].control_before = transform.map(points_[active_point_.first].control_before);

                break;
            }
            case UpdateMode::ControlBefore: {
                QPointF w = point - points_[active_point_.first].position;
                QPointF v = points_[active_point_.first].control_before - points_[active_point_.first].position;

                points_[active_point_.first].control_before = point;
                qreal delta = qAtan2(w.y() * v.x() - w.x() * v.y(), w.x() * v.x() + w.y() * v.y());

                qreal cosa = std::cos(delta);
                qreal sina = std::sin(delta);

                QTransform translate(1, 0, 0, 1, -points_[active_point_.first].position.x(), -points_[active_point_.first].position.y());
                QTransform rotate(cosa, sina, -sina, cosa, 0, 0);
                QTransform back_translate(1, 0, 0, 1, points_[active_point_.first].position.x(), points_[active_point_.first].position.y());

                QTransform transform = translate * rotate * back_translate;

                points_[active_point_.first].control_after = transform.map(points_[active_point_.first].control_after);

                break;
            }
        }

    }
}

QList<QPointF> BezierLine::getPoints(){
    return QList<QPointF>();
}

QPointF BezierLine::calculateBezierPoint(const QPointF &begin, const QPointF &begin_control, const QPointF &end_control, const QPointF &end, qreal t){
    QPointF p_b =   qPow(1.0-t, 3) *                begin +
                    3 * qPow(1.0 - t, 2) * t *      begin_control +
                    3 * qPow(t, 2) * (1.0 - t) *    end_control +
                    qPow(t, 3) *                    end;
    return p_b;
}

QPointF BezierLine::calculateBezierDerivative(const QPointF &begin, const QPointF &begin_control, const QPointF &end_control, const QPointF &end, qreal t){
    QPointF p_b =   3 * qPow(1.0-t, 2) *            (begin_control - begin) +
                    6 * (1.0 - t) * t *             (end_control - begin_control) +
                    3 * qPow(t, 2) *                (end - end_control);
    return p_b;
}

QPointF BezierLine::calculateBezierSecondDerivative(const QPointF &begin, const QPointF &begin_control, const QPointF &end_control, const QPointF &end, qreal t){
    QPointF p_b =   6 * (1.0-t) *                   (end_control - 2 * begin_control + begin) +
                    6 * t *                         (end - 2 * end_control + begin_control);
    return p_b;
}

void BezierLine::paint(QPainter* painter, const QStyleOptionGraphicsItem* /*option*/, QWidget* /*widget*/){
    QPen position_pen, control_pen, line_pen;

    position_pen.setWidth(2);
    position_pen.setStyle(Qt::SolidLine);
    control_pen = position_pen;

    line_pen.setWidth(1);
    line_pen.setStyle(Qt::DotLine);
    line_pen.setColor(Qt::blue);

    position_pen.setColor(Qt::green);
    control_pen.setColor(Qt::blue);


    for(const auto &point : points_){
        painter->setPen(position_pen);
        painter->drawEllipse(point.position.x() - point_size_ / 2, point.position.y() - point_size_ / 2, point_size_, point_size_);
        painter->setPen(control_pen);
        painter->drawEllipse(point.control_before.x() - point_size_ / 2, point.control_before.y() - point_size_ / 2, point_size_, point_size_);
        painter->drawEllipse(point.control_after.x() - point_size_ / 2, point.control_after.y() - point_size_ / 2, point_size_, point_size_);
        painter->setPen(line_pen);
        painter->drawLine(point.control_before, point.control_after);
    }

    if(points_.begin() == points_.end()) return;

    if(points_.begin() + 1 == points_.end()) return;

    for(auto it = points_.begin() ; it != points_.end(); it++){
        if (it == points_.begin()) continue;

        auto arc_length_approx = (std::hypot(((it-1)->position - it->position).x(), ((it-1)->position - it->position).y() ) +
                                 std::hypot(((it-1)->position - (it-1)->control_after).x(), ((it-1)->position - (it-1)->control_after).y() ) +
                                 std::hypot((it->control_before - (it-1)->control_after).x(), (it->control_before - (it-1)->control_after).y() ) +
                                 std::hypot((it->position - it->control_before).x(), (it->position - it->control_before).y() )) / 2.0;

        qreal delta = average_step_length_ / arc_length_approx;

        qreal t = delta;

        QPointF last = (it-1)->position;

        while(t < 1.0){

            auto p_b = calculateBezierPoint((it-1)->position, (it-1)->control_after, it->control_before, it->position, t);
            auto p_b_1 = calculateBezierDerivative((it-1)->position, (it-1)->control_after, it->control_before, it->position, t);
            auto p_b_2 = calculateBezierSecondDerivative((it-1)->position, (it-1)->control_after, it->control_before, it->position, t);

            Eigen::Matrix2d p_b_concat = (Eigen::Matrix2d() << p_b_1.x(), p_b_2.x(), 
                                                                p_b_1.y(), p_b_2.y()).finished();
            
            Eigen::Vector2d p_b_1_eig = (Eigen::Vector2d() << p_b_1.x(), p_b_1.y()).finished();

            auto nom = p_b_concat.determinant();
            auto denom = qPow(p_b_1_eig.norm(), 3);

            auto curvature = nom / denom;

            

            painter->setPen(control_pen);
            painter->drawEllipse(p_b.x() - 1, p_b.y() - 1, 2, 2);

            QPen curvature_pen;
            curvature = std::clamp(curvature, 0.0001, 1.0);

            

            int hue = 250 + (std::log10(abs(curvature)) + 4) / 4 * 110;
            QColor curvature_color;

            curvature_color.setHsl(hue, 255, 127, 255);

            curvature_pen.setWidth(2);
            curvature_pen.setStyle(Qt::SolidLine);
            curvature_pen.setColor(curvature_color);
            painter->setPen(curvature_pen);
            painter->drawLine(last, p_b);
            last = p_b;
            t += delta;
        }

        painter->drawLine(last, it->position);

    }
}
