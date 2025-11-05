#include "../include/TransactionDialog.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QLineEdit>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QTextEdit>
#include <QLabel>
#include <QDialogButtonBox>

TransactionDialog::TransactionDialog(QWidget* parent, Transaction* editTransaction,
                                     const QStringList& propertyIds,
                                     const QStringList& clientIds)
    : QDialog(parent), propertyIds(propertyIds), clientIds(clientIds)
{
    // Темная тема для диалога
    setStyleSheet(R"(
        QDialog {
            background-color: #2d2d2d;
        }
        QLabel {
            color: #d0d0d0;
        }
        QLineEdit, QTextEdit {
            background-color: #2d2d2d;
            color: #d0d0d0;
            border: 1px solid #404040;
        }
        QComboBox {
            background-color: #2d2d2d;
            color: #d0d0d0;
            border: 1px solid #404040;
        }
        QComboBox QAbstractItemView {
            background-color: #2d2d2d;
            color: #d0d0d0;
            selection-background-color: #5a9;
            selection-color: white;
        }
        QDoubleSpinBox {
            background-color: #2d2d2d;
            color: #d0d0d0;
            border: 1px solid #404040;
        }
        QDialogButtonBox QPushButton {
            background-color: #404040;
            color: white;
            min-width: 80px;
            padding: 6px;
        }
        QDialogButtonBox QPushButton:hover {
            background-color: #505050;
        }
    )");
    
    setupUI();
    
    propertyCombo->addItems(propertyIds);
    clientCombo->addItems(clientIds);
    
    if (editTransaction) {
        loadTransactionData(editTransaction);
    }
    
    setWindowTitle(editTransaction ? "Редактировать сделку" : "Добавить сделку");
    setMinimumWidth(400);
}

TransactionDialog::~TransactionDialog()
{
}

void TransactionDialog::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QFormLayout* formLayout = new QFormLayout;
    
    idEdit = new QLineEdit;
    idEdit->setPlaceholderText("123456 (6-8 цифр)");
    idEdit->setMaxLength(8);
    idEdit->setToolTip("ID должен содержать только цифры\nДлина: от 6 до 8 символов");
    propertyCombo = new QComboBox;
    clientCombo = new QComboBox;
    priceSpin = new QDoubleSpinBox;
    priceSpin->setMinimum(10000);
    priceSpin->setMaximum(1000000000);
    priceSpin->setValue(10000);
    priceSpin->setSuffix(" руб.");
    priceSpin->setToolTip("Минимальная цена: 10 000 руб.\nМаксимальная цена: 1 000 000 000 руб.");
    statusCombo = new QComboBox;
    statusCombo->addItems({"pending", "completed", "cancelled"});
    notesEdit = new QTextEdit;
    notesEdit->setMaximumHeight(100);
    
    formLayout->addRow("ID сделки:", idEdit);
    formLayout->addRow("Недвижимость:", propertyCombo);
    formLayout->addRow("Клиент:", clientCombo);
    formLayout->addRow("Цена:", priceSpin);
    formLayout->addRow("Статус:", statusCombo);
    formLayout->addRow("Примечания:", notesEdit);
    
    mainLayout->addLayout(formLayout);
    
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    mainLayout->addWidget(buttonBox);
    
    connect(buttonBox, &QDialogButtonBox::accepted, this, &TransactionDialog::validateAndAccept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

void TransactionDialog::loadTransactionData(Transaction* trans)
{
    idEdit->setText(QString::fromStdString(trans->getId()));
    int propIndex = propertyIds.indexOf(QString::fromStdString(trans->getPropertyId()));
    if (propIndex >= 0) propertyCombo->setCurrentIndex(propIndex);
    int clientIndex = clientIds.indexOf(QString::fromStdString(trans->getClientId()));
    if (clientIndex >= 0) clientCombo->setCurrentIndex(clientIndex);
    priceSpin->setValue(trans->getFinalPrice());
    
    if (trans->getStatus() == "pending") statusCombo->setCurrentIndex(0);
    else if (trans->getStatus() == "completed") statusCombo->setCurrentIndex(1);
    else statusCombo->setCurrentIndex(2);
    
    notesEdit->setPlainText(QString::fromStdString(trans->getNotes()));
}

void TransactionDialog::validateAndAccept()
{
    QString idText = idEdit->text();
    if (idText.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "ID не может быть пустым");
        return;
    }
    if (!Transaction::validateId(idText.toStdString())) {
        QMessageBox::warning(this, "Ошибка валидации", 
                            "ID должен содержать только цифры и быть длиной от 6 до 8 символов");
        return;
    }
    if (propertyCombo->currentIndex() < 0) {
        QMessageBox::warning(this, "Ошибка", "Выберите недвижимость");
        return;
    }
    if (clientCombo->currentIndex() < 0) {
        QMessageBox::warning(this, "Ошибка", "Выберите клиента");
        return;
    }
    double price = priceSpin->value();
    if (price <= 0) {
        QMessageBox::warning(this, "Ошибка", "Цена должна быть положительной");
        return;
    }
    
    // Валидация: цена сделки должна быть в разумных пределах
    if (price < 10000) {
        QMessageBox::warning(this, "Ошибка", "Цена сделки не может быть меньше 10 000 рублей");
        return;
    }
    if (price > 1000000000) {
        QMessageBox::warning(this, "Ошибка", "Цена сделки не может превышать 1 000 000 000 рублей");
        return;
    }
    
    // Проверка соответствия цены недвижимости и сделки (сделка не может быть намного дороже)
    if (propertyCombo->currentIndex() >= 0) {
        // Можно добавить дополнительную проверку против цены недвижимости
        // Но для сделки цена может отличаться (скидка, переплата и т.д.)
    }
    
    accept();
}

QString TransactionDialog::getId() const { return idEdit->text(); }
QString TransactionDialog::getPropertyId() const { return propertyCombo->currentText(); }
QString TransactionDialog::getClientId() const { return clientCombo->currentText(); }
double TransactionDialog::getFinalPrice() const { return priceSpin->value(); }
QString TransactionDialog::getStatus() const { return statusCombo->currentText(); }
QString TransactionDialog::getNotes() const { return notesEdit->toPlainText(); }

