#include <QPushButton>

#include <QMouseEvent>
#include <QEnterEvent>
#include <QHoverEvent>
// #include <QPointingDevice>
#include <iostream>

class MyButton : public QPushButton {
    Q_OBJECT
public:
    MyButton(QWidget *parent = nullptr)
            : QPushButton(parent){
        
        // setFixedSize(size);
        setFlat(true);
        setMouseTracking(true);
        setAttribute(Qt::WA_Hover, true);
        is_hover_ = false;

    }

    bool event(QEvent * e)
    {
        switch(e->type())
        {
        case QEvent::HoverEnter:
            hoverEnter(static_cast<QHoverEvent*>(e));
            break;
        case QEvent::HoverLeave:
            hoverLeave(static_cast<QHoverEvent*>(e));
            break;
        case QEvent::HoverMove:
            hoverMove(static_cast<QHoverEvent*>(e));
            break;
        default:
            notHoverMove(e);
            break;
        }
        return QWidget::event(e);
    }


    void hoverEnter(QHoverEvent * /*event*/){
        std::cout << "hoverEnter\n";
        if(is_hover_) return;
        setIcon(QIcon::fromTheme(":images/add.png"));
        is_hover_ = true;
    }

    void hoverLeave(QHoverEvent * /*event*/){
        std::cout << "hoverLeave\n";
        if(!is_hover_) return;
        setIcon(QIcon::fromTheme(":images/load.png"));
        is_hover_ = false;
    }

    void hoverMove(QHoverEvent * /*event*/){
        std::cout << "hoverMove\n";
        if(is_hover_) return;
        setIcon(QIcon::fromTheme(":images/add.png"));
        is_hover_ = false;
    }

    void notHoverMove(QEvent * /*event*/){
        QRect button_rect(0,0,height(), width());
        QPoint global_pos = QCursor::pos();
        auto point = mapFromGlobal(global_pos);

        std::cout << point.x() << "  " << point.y() << "\n";

        bool cursor_is_outside = !button_rect.contains(mapFromGlobal(global_pos));

        if(!cursor_is_outside) return;
        else if(cursor_is_outside && !is_hover_) return;

        setIcon(QIcon::fromTheme(":images/load.png"));
        is_hover_ = false;
    }

    // bool event(QEvent * e)
    // {
    //     switch(e->type())
    //     {
    //     case QEvent::HoverEnter:
    //         hoverEnter(static_cast<QHoverEvent*>(e));
    //         return true;
    //         break;
    //     case QEvent::HoverLeave:
    //         hoverLeave(static_cast<QHoverEvent*>(e));
    //         return true;
    //         break;
    //     case QEvent::HoverMove:
    //         hoverMove(static_cast<QHoverEvent*>(e));
    //         return true;
    //         break;
    //     default:
    //         break;
    //     }
    //     return QWidget::event(e);
    // }

    // void mouseMoveEvent(QMouseEvent *event){
    //     qreal delta = 3;
    //     QRect hover_area(delta, delta, width() - 2*delta, height() - 2*delta);
    //     if(hover_area.contains(event->pos()))
    //         setText("yes yes");
    //     else
    //         setText("no no");
    // }

    void setTextColor(const QString& color) {
        QString style = "QPushButton {"
                        "  background-image: url(%1);"
                        "  color: " + color + ";"
                        "  border: none;"
                        "}"
                        "QPushButton:hover {"
                        "  background-image: url(%2);"
                        "}";
        setStyleSheet(style.arg(normalImage, hoverImage));
    }

private:
    bool is_hover_;
    QString normalImage;
    QString hoverImage;
};