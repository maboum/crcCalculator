#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "crcutils.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QFont monoFont("mono");
    monoFont.setStyleHint(QFont::Monospace);
    ui->plainTextEdit->setFont(monoFont);

    QString text;

    for (int i = 0; i < 256; ++i)
    {
        auto d = Crc64ECMA[i];
        QString hex = QString::number(d, 16).rightJustified(sizeof(d)*2, '0').toUpper().prepend("0x");
        //hex = hex.sprintf("init(%d)", i).rightJustified(9, ' ');
        hex.append(", ");
        if (((i+1) % 8) == 0)
        {
            hex.append("\n");;
        }
        text.append(hex);
    }

    ui->plainTextEdit->setPlainText(text);

    calculateCrcs();

    connect(ui->plainTextEdit, &QPlainTextEdit::textChanged, this, &MainWindow::calculateCrcs);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::calculateCrcs()
{
    QByteArray data = ui->plainTextEdit->document()->toPlainText().toUtf8();

    auto crc32Value = crc32(data.data());
    ui->crc32Result->setText(QString::number(crc32Value, 16).toUpper() + " (" + QString::number(crc32Value) + ")");

    auto crc64Value = crc64(data.data());
    ui->crc64Result->setText(QString::number(crc64Value, 16).toUpper() + " (" + QString::number(crc64Value) + ")");
}
