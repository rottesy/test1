#ifndef TRANSACTION_DIALOG_H
#define TRANSACTION_DIALOG_H

#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QTextEdit>
#include <QDialogButtonBox>
#include "../include/Transaction.h"

class TransactionDialog : public QDialog
{
    Q_OBJECT

public:
    TransactionDialog(QWidget* parent = nullptr, Transaction* editTransaction = nullptr,
                     const QStringList& propertyIds = QStringList(),
                     const QStringList& clientIds = QStringList());
    ~TransactionDialog();

    QString getId() const;
    QString getPropertyId() const;
    QString getClientId() const;
    double getFinalPrice() const;
    QString getStatus() const;
    QString getNotes() const;

private slots:
    void validateAndAccept();

private:
    void setupUI();
    void loadTransactionData(Transaction* trans);

    QLineEdit* idEdit;
    QComboBox* propertyCombo;
    QComboBox* clientCombo;
    QDoubleSpinBox* priceSpin;
    QComboBox* statusCombo;
    QTextEdit* notesEdit;
    QDialogButtonBox* buttonBox;
    QStringList propertyIds;
    QStringList clientIds;
};

#endif // TRANSACTION_DIALOG_H

