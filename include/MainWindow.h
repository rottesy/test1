#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../include/Constants.h"
#include "../include/EstateAgency.h"
#include "../include/TableHelper.h"
#include "../include/Utils.h"
#include <QCheckBox>
#include <QComboBox>
#include <QDateEdit>
#include <QDialog>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QFrame>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollArea>
#include <QSpinBox>
#include <QSplitter>
#include <QStackedWidget>
#include <QStatusBar>
#include <QTabWidget>
#include <QTableWidget>
#include <QTextEdit>
#include <QVBoxLayout>

class MainWindow : public QMainWindow {
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

  // Аукционы
  void addAuction();
  void viewAuction();
  void deleteAuction();
  void refreshAuctions();
  void searchAuctions();
  void auctionSelectionChanged();

  // Файлы
  void saveAllData();
  void loadAllData();

private:
  void setupUI();
  void setupMenuBar();
  void setupPropertiesTab();
  void setupClientsTab();
  void setupTransactionsTab();
  void setupAuctionsTab();

  // Новый интерфейс
  void setupNewUI();
  void setupDashboard();
  void setupPropertiesPage();
  void setupClientsPage();
  void setupTransactionsPage();
  void setupAuctionsPage();
  void updateDashboardStats();
  void onNavigationChanged(int index);
  void applyStyles(); // Применение стилей

  void updatePropertiesTable();
  void updateClientsTable();
  void updateTransactionsTable();
  void updateAuctionsTable();

  void showPropertyDetails(Property *prop);
  void showClientDetails(Client *client);
  void showTransactionDetails(Transaction *trans);
  void showAuctionDetails(Auction *auction);

  // Взаимосвязи
  void showPropertyTransactions(const std::string &propertyId);
  void showClientTransactions(const std::string &clientId);
  void highlightRelatedItems();

  // Валидация
  bool validateTransaction(const std::string &propertyId,
                           const std::string &clientId,
                           const std::string &status,
                           const std::string &excludeTransactionId = "");

  // Вспомогательные функции
  void showStatusMessage(const QString &message, int timeout = 3000);
  QString getSelectedIdFromTable(QTableWidget *table);
  bool checkTableSelection(QTableWidget *table, const QString &errorMessage);
  void refreshAllData();
  bool hasActiveTransactions(const std::string &propertyId);

  EstateAgency *agency;

  // Недвижимость
  QTableWidget *propertiesTable;
  QPushButton *addPropertyBtn;
  QPushButton *editPropertyBtn;
  QPushButton *deletePropertyBtn;
  QPushButton *refreshPropertyBtn;
  QPushButton *searchPropertyBtn;
  QLineEdit *searchPropertyEdit;
  QTextEdit *propertyDetailsText;

  // Клиенты
  QTableWidget *clientsTable;
  QPushButton *addClientBtn;
  QPushButton *editClientBtn;
  QPushButton *deleteClientBtn;
  QPushButton *refreshClientBtn;
  QPushButton *searchClientBtn;
  QLineEdit *searchClientEdit;
  QTextEdit *clientDetailsText;

  // Сделки
  QTableWidget *transactionsTable;
  QPushButton *addTransactionBtn;
  QPushButton *editTransactionBtn;
  QPushButton *deleteTransactionBtn;
  QPushButton *refreshTransactionBtn;
  QPushButton *searchTransactionBtn;
  QLineEdit *searchTransactionEdit;
  QTextEdit *transactionDetailsText;

  // Аукционы
  QTableWidget *auctionsTable;
  QPushButton *addAuctionBtn;
  QPushButton *viewAuctionBtn;
  QPushButton *deleteAuctionBtn;
  QPushButton *refreshAuctionBtn;
  QPushButton *searchAuctionBtn;
  QLineEdit *searchAuctionEdit;
  QTextEdit *auctionDetailsText;

  // Новый интерфейс - навигация
  QListWidget *navigationList;
  QStackedWidget *contentStack;

  // Виджеты для нового интерфейса
  QWidget *dashboardWidget;
  QWidget *propertiesPageWidget;
  QWidget *clientsPageWidget;
  QWidget *transactionsPageWidget;
  QWidget *auctionsPageWidget;

  // Статистика для dashboard
  QLabel *statsPropertiesLabel;
  QLabel *statsClientsLabel;
  QLabel *statsTransactionsLabel;
  QLabel *statsAvailableLabel;
  QLabel *statsAuctionsLabel;
};

#endif // MAINWINDOW_H
