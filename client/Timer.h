#include <QTimer>
#include <QThread>

class Timer : public QTimer
{
    Q_OBJECT
public:
    explicit Timer(){}

    void start_with_fire(int duration) {
        emit this->timeout({});
        this->start(duration);
    }
};
