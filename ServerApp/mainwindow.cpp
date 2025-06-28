#include "mainwindow.h"
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(500, 400);

    // ساخت UI با کد:
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    QVBoxLayout *layout = new QVBoxLayout(central);

    startButton = new QPushButton("شروع سرور", this);
    log = new QTextEdit(this);
    log->setReadOnly(true);

    layout->addWidget(startButton);
    layout->addWidget(log);

    serverManager = new ServerManager(this);

    // وصل کردن سیگنال لاگ به نمایش در QTextEdit
    connect(serverManager, &ServerManager::logMessage,
            this, [=](const QString &msg){
                log->append(msg);
            });

    connect(startButton, &QPushButton::clicked,
            this, &MainWindow::onStartButtonClicked);
}

MainWindow::~MainWindow()
{
    // خود Qt اشیاء زیرمجموعه رو پاک می‌کنه
}

void MainWindow::onStartButtonClicked()
{
    serverManager->connectToDatabase();
    serverManager->startServer(1234);
}
