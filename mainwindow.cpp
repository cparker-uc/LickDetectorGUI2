#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), recordingAll(false)
{
    // Create main widget and layout
    centralWidget = new QWidget(this);
    mainLayout = new QVBoxLayout(centralWidget);
    

    // --- Create the All-Devices (Start/Stop All) Button ---
    allDevicesButton = new QPushButton("Start Recording");
    allDevicesButton->setFixedSize(400, 60);
    connect(allDevicesButton, &QPushButton::clicked, this, &MainWindow::onAllDevicesButtonClicked);
    mainLayout->addWidget(allDevicesButton);

    // --- Create Sensor Rows Grid Layout (organized in columns) ---
    sensorsColumnsLayout = new QHBoxLayout;
    int numColumns = NUM_SENSORS_TOTAL / SENSORS_PER_COLUMN;
    for (int col = 0; col < numColumns; ++col) {
        QWidget *columnWidget = new QWidget;
        QGridLayout *gridLayout = new QGridLayout(columnWidget);
        // Add headers at the top row
        gridLayout->addWidget(new QLabel(""), 0, 0);
        gridLayout->addWidget(new QLabel(""), 0, 1);
        gridLayout->addWidget(new QLabel("Start Vol"), 0, 2);
        gridLayout->addWidget(new QLabel("Stop Vol"), 0, 3);

        // Create SENSORS_PER_COLUMN rows per column
        for (int row = 0; row < SENSORS_PER_COLUMN; ++row) {
            int sensorID = col * SENSORS_PER_COLUMN + row;
            int gridRow = row + 1; // row 0 is used for header

            // Create sensor toggle button
            QPushButton *toggleButton = new QPushButton(QString("Sensor %1: Start").arg(sensorID));
            toggleButton->setCheckable(true);
            toggleButton->setFixedWidth(200);

            // Create sensor test button
            QPushButton *testButton = new QPushButton("Test");
            testButton->setFixedWidth(100);

            // Create volume input boxes (double spin boxes)
            QDoubleSpinBox *startVolSpin = new QDoubleSpinBox;
            startVolSpin->setRange(0, 100);
            startVolSpin->setSingleStep(0.01);
            startVolSpin->setFixedWidth(100);

            QDoubleSpinBox *stopVolSpin = new QDoubleSpinBox;
            stopVolSpin->setRange(0, 100);
            stopVolSpin->setSingleStep(0.01);
            stopVolSpin->setFixedWidth(100);

            // Add controls to grid layout
            gridLayout->addWidget(toggleButton, gridRow, 0);
            gridLayout->addWidget(testButton, gridRow, 1);
            gridLayout->addWidget(startVolSpin, gridRow, 2);
            gridLayout->addWidget(stopVolSpin, gridRow, 3);

            // Save sensor row data for later use
            SensorRow rowItem;
            rowItem.sensorId = sensorID;
            rowItem.toggleButton = toggleButton;
            rowItem.testButton = testButton;
            rowItem.startVolSpin = startVolSpin;
            rowItem.stopVolSpin = stopVolSpin;
            sensorRows.push_back(rowItem);

            // Connect toggle button signal (simulate sensor_recording callback)
            connect(toggleButton, &QPushButton::clicked, [=]() {
                bool isChecked = toggleButton->isChecked();
                if (isChecked) {
                    toggleButton->setText(QString("Sensor %1: Stop").arg(sensorID));
                    // Placeholder for starting sensor recording (record sensor start time)
                    outputArea->append(QString("Sensor %1: Recording start triggered.").arg(sensorID));
                    // TODO: Call your black-box sensor_recording(sensorID, true) function here.
                } else {
                    toggleButton->setText(QString("Sensor %1: Start").arg(sensorID));
                    outputArea->append(QString("Sensor %1: Recording stop triggered.").arg(sensorID));
                    // TODO: Call your black-box sensor_recording(sensorID, false) function here.
                }
            });

            // Connect test button signal (simulate sensor_test callback)
            connect(testButton, &QPushButton::clicked, [=]() {
                graphArea->clear();
                graphArea->append(QString("Sensor %1: Test triggered - plotting raw data for confirmation.").arg(sensorID));
                // TODO: Implement a plotting mechanism or call your test function.
            });
        } // end sensor row loop for column

        sensorsColumnsLayout->addWidget(columnWidget);
    } // end column loop
    mainLayout->addLayout(sensorsColumnsLayout);

    // --- Create Clear Output/Graph Buttons ---
    QHBoxLayout *clearButtonsLayout = new QHBoxLayout;
    clearOutputButton = new QPushButton("Clear Output");
    clearGraphButton = new QPushButton("Clear Graph");
    connect(clearOutputButton, &QPushButton::clicked, this, &MainWindow::clearOutput);
    connect(clearGraphButton, &QPushButton::clicked, this, &MainWindow::clearGraph);
    clearButtonsLayout->addWidget(clearOutputButton);
    clearButtonsLayout->addWidget(clearGraphButton);
    mainLayout->addLayout(clearButtonsLayout);

    // --- Create the Output Areas (for text and graph) ---
    QHBoxLayout *outputsLayout = new QHBoxLayout;
    outputArea = new QTextEdit;
    outputArea->setFixedSize(500, 500);
    graphArea = new QTextEdit;
    graphArea->setFixedSize(500, 500);
    outputsLayout->addWidget(outputArea);
    outputsLayout->addWidget(graphArea);
    mainLayout->addLayout(outputsLayout);

    setCentralWidget(centralWidget);
    mpr121Controller = new MPR121_Controller();
    connect(mpr121Controller, &MPR121_Controller::logMessage, this, [=](const QString &msg){
        outputArea->append(msg);
    });
    mpr121Controller->configure("FT232H1");
}

void MainWindow::onAllDevicesButtonClicked() 
{
    if (!recordingAll) 
    {
        // Start recording all sensors
        recordingAll = true;
        filename = QString("raw_data_%1.h5")
                        .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss"));
        allDevicesButton->setText("Stop Recording");
        outputArea->append("Started recording sensors asynchronously.");
        mpr121Controller->start_recording();
    } else 
    {
        // Stop recording all sensors
        recordingAll = false;
        allDevicesButton->setText("Start Recording");
        outputArea->append("Stopping recording sensors.");
        mpr121Controller->stop_recording();
        // TODO: Stop the asynchronous sensor recording thread/task

        // Make sure all sensor toggle buttons are reset
        for (auto &row : sensorRows) 
        {
            if (row.toggleButton->isChecked()) 
            {
                row.toggleButton->setChecked(false);
                row.toggleButton->setText(QString("Sensor %1: Start").arg(row.sensorId));
                // Optionally, trigger a sensor stop callback here if needed.
            }
        }
    }
}

void MainWindow::clearOutput() 
{
        outputArea->clear();
}

void MainWindow::clearGraph() 
{
        graphArea->clear();
}