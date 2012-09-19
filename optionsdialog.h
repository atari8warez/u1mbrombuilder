#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QtGui/QDialog>
#include <QTreeWidget>
#include <QFileDialog>


namespace Ui {
    class OptionsDialog;
}

class OptionsDialog : public QDialog {
    Q_OBJECT

public:
    OptionsDialog(QWidget *parent = 0);
    ~OptionsDialog();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::OptionsDialog *m_ui;
    QTreeWidgetItem *itemI18n, *itemRom;

private slots:
    void on_treeWidget_itemClicked(QTreeWidgetItem* item, int column);
    void on_treeWidget_currentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous);
    void OptionsDialog_accepted();
    void browseBasePath();
    void browsePath();
    void check_UseDef();
    void check_UseLast();
    void check_UseNone();
};

#endif // OPTIONSDIALOG_H
