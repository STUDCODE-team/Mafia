#include <QTimer>
#include <QThread>

class Timer : public QTimer
{
    Q_OBJECT
private:
    int duration = 3000;
public:
//    explicit Timer(const int& duration_): duration(duration_){};
//    explicit Timer() = default;

    void start_with_fire(int duration) {
        emit this->timeout({});
        this->start(duration);
    }

    void start_with_fire() {
        emit this->timeout({});
        this->start(duration);
    }

};
