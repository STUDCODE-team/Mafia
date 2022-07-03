#include <QTimer>

class Timer : public QObject
{
    Q_OBJECT
public:
    explicit Timer(){
        connect(timer, &QTimer::timeout, this, &Timer::timeout);
    }
    void start(int duration) {
        emit timeout(); timer->start(duration);
    }
    void stop(){
        timer->stop();
    }

private:
    QTimer *timer = new QTimer(this);
signals:
    void timeout();
};
