#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "about_dialog.h"
#include "find_dialog.h"
#include "replace_dialog.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QTextCodec>
#include <QColorDialog>
#include <QFontDialog>
#include <QDebug>
#include <QProcess>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    textChange = false;

    statusLabel.setMaximumWidth(200);
    statusLabel.setText("length：" + QString::number(0) + "    lines：" + QString::number(1));
    ui->statusbar->addPermanentWidget(&statusLabel);

    statusCursorLabel.setMaximumWidth(200);
    statusCursorLabel.setText("row：" + QString::number(0) + "    col：" + QString::number(1));
    ui->statusbar->addPermanentWidget(&statusCursorLabel);

    QLabel *person = new QLabel(ui->statusbar);
    person->setText("张朝科");
    ui->statusbar->addPermanentWidget(person);

    // 初始化按钮
    ui->action_Copy->setEnabled(false);
    ui->action_Cut->setEnabled(false);
    ui->action_Recover->setEnabled(false);
    ui->action_U->setEnabled(false);
    ui->action_Paste->setEnabled(false);

    QPlainTextEdit::LineWrapMode mode = ui->textEdit->lineWrapMode();

    if(mode == QTextEdit::NoWrap){
        ui->textEdit->setLineWrapMode(QPlainTextEdit::WidgetWidth);
        ui->action_Wrap->setChecked(false);
    } else{
        ui->textEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
        ui->action_Wrap->setChecked(true);
    }

    ui->action_State->setChecked(true);
    ui->action_Tool->setChecked(true);
    ui->action_Line->setChecked(true);

    connect(ui->action_Line, SIGNAL(triggered(bool)), ui->textEdit, SLOT(hideLineNumberArea(bool)));
    InitCountMessage();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_action_About_triggered()
{
    About_Dialog dlg;
    dlg.exec();
}


void MainWindow::on_action_Find_triggered()
{
    Find_Dialog find(this, ui->textEdit);
    find.exec();
}


void MainWindow::on_action_Replace_triggered()
{
    replace_Dialog replace(this, ui->textEdit);
    replace.exec();
}


//新建文件
void MainWindow::on_action_New_triggered()
{
    if(!userEditConfirmed()){
        return;
    }

    filePath = "";
    ui->textEdit->clear();
    this->setWindowTitle(tr("新建文本文件 - 编辑器"));

    textChange = false;
}


//打开文件
void MainWindow::on_action_Open_triggered()
{
    if(!userEditConfirmed()){
        return;
    }
    QString filename = QFileDialog::getOpenFileName(this, "打开文件",".",tr("Text files (*.txt) ;; All (*.*)"));
    QFile file(filename);

    if(!file.open(QFile::ReadOnly | QFile::Text)){
        QMessageBox::warning(this, "..", "打开文件失败");
        return;
    }

    filePath = filename;

    QTextStream in(&file);
    QString text = in.readAll();
    ui->textEdit->insertPlainText(text);
    file.close();

    this->setWindowTitle(QFileInfo(filename).absoluteFilePath());

    textChange = false;
}


//保存文件
void MainWindow::on_action_Save_triggered()
{
    if(filePath == ""){
        QString filename = QFileDialog::getSaveFileName(this, "保存文件", ".",tr("Text files(*.txt)"));

        QFile file(filename);
        if(!file.open(QFile::WriteOnly | QFile::Text)){
            QMessageBox::warning(this, "..", "保存文件失败");
            return;
        }
        file.close();
        filePath = filename;
    }
    QFile file(filePath);

    if(!file.open(QFile::WriteOnly | QFile::Text)){
        QMessageBox::warning(this, "..", "保存文件失败");
        return;
    }

    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.flush();
    file.close();

    this->setWindowTitle(QFileInfo(filePath).absoluteFilePath());

    textChange = false;
}


//另存为
void MainWindow::on_action_A_Save_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, "另存文件",".",tr("Text files (*.txt) ;; All (*.*)"));
    QFile file(filename);

    if(!file.open(QFile::ReadOnly | QFile::Text)){
        QMessageBox::warning(this, "..", "保存文件失败");
        return;
    }
    filePath = filename;

    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.flush();
    file.close();

    this->setWindowTitle(QFileInfo(filePath).absoluteFilePath());
    qDebug() << QString("保存成功");
}


void MainWindow::on_textEdit_textChanged()
{
    if(!textChange){
        if(!this->windowTitle().contains("*")){
            this->setWindowTitle("*" + this->windowTitle());
        }
        textChange = true;
    }

    statusLabel.setText("length：" + QString::number(ui->textEdit->toPlainText().length())
                        + "    lines: " + QString::number(ui->textEdit->document()->lineCount()));
}

bool MainWindow::userEditConfirmed()
{
    if(textChange){
        QString path = (filePath != "") ? filePath: "无标题.txt";

        QMessageBox msg(this);
        msg.setIcon(QMessageBox::Question);
        msg.setWindowTitle("");
        msg.setText(QString("是否将更改保存到\n") + "\"" + path + "\"" "?");
        msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

        int r =  msg.exec();
        switch (r) {
        case QMessageBox::Yes:
            on_action_Save_triggered();
            break;
        case QMessageBox::No:
            textChange = false;
            break;
        case QMessageBox::Cancel:
            return false;
        }
    }

    return true;
}


// 撤销操作
void MainWindow::on_action_U_triggered()
{
    ui->textEdit->undo();
}


// 恢复
void MainWindow::on_action_Recover_triggered()
{
    ui->textEdit->redo();
}


void MainWindow::on_action_Copy_triggered()
{
    ui->textEdit->copy();
    ui->action_Paste->setEnabled(true);
}


void MainWindow::on_action_Paste_triggered()
{
    ui->textEdit->paste();
}


void MainWindow::on_action_Cut_triggered()
{
    ui->textEdit->cut();
    ui->action_Paste->setEnabled(true);
}

void MainWindow::on_textEdit_copyAvailable(bool b)
{
    ui->action_Copy->setEnabled(b);
    ui->action_Cut->setEnabled(b);
}


void MainWindow::on_textEdit_redoAvailable(bool b)
{
    ui->action_Recover->setEnabled(b);
}


void MainWindow::on_textEdit_undoAvailable(bool b)
{
    ui->action_U->setEnabled(b);
}

//字体颜色
void MainWindow::on_action_Font_Color_triggered()
{
    QColor color = QColorDialog::getColor(Qt::black, this, "选择颜色");
    if(color.isValid()){
        ui->textEdit->setStyleSheet(QString("QPlainTextEdit{color: %1}").arg(color.name()));
    }
}


void MainWindow::on_action_Font_Background_Color_triggered()
{
}

//背景颜色
void MainWindow::on_action_Edit_Background_Color_triggered()
{
    QColor color = QColorDialog::getColor(Qt::black, this, "选择颜色");
    if(color.isValid()){
        ui->textEdit->setStyleSheet(QString("QPlainTextEdit{background-color: %1}").arg(color.name()));
    }
}

//自动换行
void MainWindow::on_action_Wrap_triggered()
{
    QPlainTextEdit::LineWrapMode mode = ui->textEdit->lineWrapMode();

    if(mode == QTextEdit::NoWrap){
        ui->textEdit->setLineWrapMode(QPlainTextEdit::WidgetWidth);
        ui->action_Wrap->setChecked(true);
    } else{
        ui->textEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
        ui->action_Wrap->setChecked(false);
    }
}

//工具栏
void MainWindow::on_action_Tool_triggered()
{
    bool v = ui->toolBar->isVisible();
    ui->toolBar->setVisible(!v);
    ui->action_Tool->setChecked(!v);
}

//状态栏
void MainWindow::on_action_State_triggered()
{
    bool v = ui->statusbar->isVisible();
    ui->statusbar->setVisible(!v);
    ui->action_State->setChecked(!v);
}

//改变字体
void MainWindow::on_action_Font_triggered()
{
    bool ok = false;
    QFont font = QFontDialog::getFont(&ok,this);

    if(ok){
        ui->textEdit->setFont(font);
    }
}

void MainWindow::on_action_Exit_triggered()
{
    if(userEditConfirmed()){
        exit(0);
    }
}


//全选
void MainWindow::on_action_All_triggered()
{
    ui->textEdit->selectAll();
}


void MainWindow::on_textEdit_cursorPositionChanged()
{
    int col = 0;
    int row = 0;
    int flag = -1;
    int pos = ui->textEdit->textCursor().position();
    QString text = ui->textEdit->toPlainText();

    for(int i = 0;i < pos;i++){
        if(text[i] == '\n'){
            row++;
            flag = i;
        }
    }
    flag++;
    col = pos - flag;
    statusCursorLabel.setText("row: " + QString::number(row+1) + "  col: " + QString::number(col+1));
}

//保存函数
void MainWindow::saveOnTime()
{
    if(filePath == ""){
        return;
    }else{
        QFile file(filePath);
        if(!file.open(QFile::WriteOnly | QFile::Text)){
            QMessageBox::warning(this, "..", "保存文件失败");
            return;
        }

        QTextStream out(&file);
        QString text = ui->textEdit->toPlainText();
        out << text;
        file.flush();
        file.close();

        this->setWindowTitle(QFileInfo(filePath).absoluteFilePath());

        textChange = false;
    }
}

//自动保存
void MainWindow::InitCountMessage()
{
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(saveOnTime())); // 自动执行saveOnTime
    timer->start(10000); // 每隔10秒自动保存一次
}

//多开窗口
void MainWindow::on_action_new_window_triggered()
{
    QString path("Lab2.exe");

    QProcess *process = new QProcess(this);
    qDebug() << "new Process";

    process->startDetached(path);
}

