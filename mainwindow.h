#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void InitCountMessage();

private slots:
    void on_action_About_triggered();

    void on_action_Find_triggered();

    void on_action_Replace_triggered();

    void on_action_New_triggered();

    void on_action_Open_triggered();

    void on_action_Save_triggered();

    void on_action_A_Save_triggered();

    void on_textEdit_textChanged();

    void on_action_U_triggered();

    void on_action_Recover_triggered();

    void on_action_Copy_triggered();

    void on_action_Paste_triggered();

    void on_action_Cut_triggered();

    void on_textEdit_copyAvailable(bool b);

    void on_textEdit_redoAvailable(bool b);

    void on_textEdit_undoAvailable(bool b);

    void on_action_Font_Color_triggered();

    void on_action_Font_Background_Color_triggered();

    void on_action_Edit_Background_Color_triggered();

    void on_action_Wrap_triggered();

    void on_action_Tool_triggered();

    void on_action_State_triggered();

    void on_action_Font_triggered();

    void on_action_Exit_triggered();

    void on_action_All_triggered();

    void on_textEdit_cursorPositionChanged();

    void saveOnTime();

    void on_action_new_window_triggered();

    void on_action_history_triggered();

private:
    Ui::MainWindow *ui;

    QLabel statusCursorLabel;
    QLabel statusLabel;

    QString filePath;
    bool textChange;

    bool userEditConfirmed();
    void toHerf();
};
#endif // MAINWINDOW_H
