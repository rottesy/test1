#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTableWidget>
#include <QTabWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QGroupBox>
#include <QMessageBox>
#include <QHeaderView>
#include <QSplitter>
#include <QFormLayout>
#include <QDialog>
#include <QDialogButtonBox>
#include <QDateEdit>
#include "../include/EstateAgency.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Недвижимость
    void addProperty();
    void editProperty();
    void deleteProperty();
    void refreshProperties();
    void searchProperties();
    void propertySelectionChanged();

    // Клиенты
    void addClient();
    void editClient();
    void deleteClient();
    void refreshClients();
    void searchClients();

    // Сделки
    void addTransaction();
    void editTransaction();
    void deleteTransaction();
    void refreshTransactions();
    void searchTransactions();

    // Файлы
    void saveAllData();
    void loadAllData();

private:
    void setupUI();
    void setupMenuBar();
    void setupPropertiesTab();
    void setupClientsTab();
    void setupTransactionsTab();
    void applyStyles(); // Применение стилей
    
    void updatePropertiesTable();
    void updateClientsTable();
    void updateTransactionsTable();
    
    void showPropertyDetails(Property* prop);
    void showClientDetails(Client* client);
    void showTransactionDetails(Transaction* trans);
    
    // Взаимосвязи
    void showPropertyTransactions(const std::string& propertyId);
    void showClientTransactions(const std::string& clientId);
    void highlightRelatedItems();
    
    // Валидация
    bool validateTransaction(const std::string& propertyId, const std::string& clientId, const std::string& status);

    EstateAgency* agency;
    
    // Недвижимость
    QTableWidget* propertiesTable;
    QPushButton* addPropertyBtn;
    QPushButton* editPropertyBtn;
    QPushButton* deletePropertyBtn;
    QPushButton* refreshPropertyBtn;
    QPushButton* searchPropertyBtn;
    QLineEdit* searchPropertyEdit;
    QTextEdit* propertyDetailsText;
    
    // Клиенты
    QTableWidget* clientsTable;
    QPushButton* addClientBtn;
    QPushButton* editClientBtn;
    QPushButton* deleteClientBtn;
    QPushButton* refreshClientBtn;
    QPushButton* searchClientBtn;
    QLineEdit* searchClientEdit;
    QTextEdit* clientDetailsText;
    
    // Сделки
    QTableWidget* transactionsTable;
    QPushButton* addTransactionBtn;
    QPushButton* editTransactionBtn;
    QPushButton* deleteTransactionBtn;
    QPushButton* refreshTransactionBtn;
    QPushButton* searchTransactionBtn;
    QLineEdit* searchTransactionEdit;
    QTextEdit* transactionDetailsText;
    
    QTabWidget* tabWidget;
};

#endif // MAINWINDOW_H

