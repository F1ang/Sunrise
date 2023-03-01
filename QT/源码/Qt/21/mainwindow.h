#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "school.h"
#include "student.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class School;
class Student;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    School *school;
    Student *student;
};
#endif // MAINWINDOW_H
