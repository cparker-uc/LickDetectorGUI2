#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QDateTime>
#include <QDebug>
#include <vector>
#include <functional>
#include "mpr121_controller.h"

// Constant definitions
constexpr int NUM_SENSORS_TOTAL = 24;
constexpr int SENSORS_PER_COLUMN = 6;

// A structure to hold pointers for each sensor row control
struct SensorRow {
    int sensorId;
    QPushButton* toggleButton;
    QPushButton* testButton;
    QDoubleSpinBox* startVolSpin;
    QDoubleSpinBox* stopVolSpin;
};

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override = default;
private slots:
    void onAllDevicesButtonClicked();
    void clearOutput();
    void clearGraph();
private:
    // Widgets
    QPushButton *allDevicesButton;
    QPushButton *clearOutputButton;
    QPushButton *clearGraphButton;
    QTextEdit *outputArea;
    QTextEdit *graphArea;
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    QHBoxLayout *sensorsColumnsLayout;
    MPR121_Controller *mpr121Controller;
    // List of sensor row controls
    std::vector<SensorRow> sensorRows;

    // State variables
    bool recordingAll;
    QString filename;
};