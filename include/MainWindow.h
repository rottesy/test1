#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../include/Constants.h"
#include "../include/EstateAgency.h"
#include "../include/HtmlFormatter.h"
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

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

  private slots:
    void addProperty();
    void editProperty();
    void deleteProperty();
    void refreshProperties();
    void searchProperties();
    void propertySelectionChanged();

    void addClient();
    void editClient();
    void deleteClient();
    void refreshClients();
    void searchClients();

    void addTransaction();
    void editTransaction();
    void deleteTransaction();
    void refreshTransactions();
    void searchTransactions();

    void addAuction();
    void viewAuction();
    void deleteAuction();
    void refreshAuctions();
    void searchAuctions();
    void auctionSelectionChanged();

    void saveAllData();
    void loadAllData();

  private:
    void setupUI();
    void setupMenuBar();
    void setupPropertiesTab();
    void setupClientsTab();
    void setupTransactionsTab();
    void setupAuctionsTab();

    void setupNewUI();
    void setupDashboard();
    void setupPropertiesPage();
    void setupClientsPage();
    void setupTransactionsPage();
    void setupAuctionsPage();
    void updateDashboardStats();
    void onNavigationChanged(int index);
    void applyStyles();

    void updatePropertiesTable();
    void updateClientsTable();
    void updateTransactionsTable();
    void updateAuctionsTable();

    void showPropertyDetails(Property *prop);
    void showClientDetails(Client *client);
    void showTransactionDetails(Transaction *trans);
    void showAuctionDetails(Auction *auction);

    void showPropertyTransactions(const std::string &propertyId);
    void showClientTransactions(const std::string &clientId);
    void highlightRelatedItems();

    bool validateTransaction(const std::string &propertyId, const std::string &clientId, const std::string &status,
                             const std::string &excludeTransactionId = "");

    void showStatusMessage(const QString &message, int timeout = 3000);
    QString getSelectedIdFromTable(QTableWidget *table);
    bool checkTableSelection(QTableWidget *table, const QString &errorMessage);
    void refreshAllData();
    bool hasActiveTransactions(const std::string &propertyId);

    // Helper methods for code reduction
    QWidget *createActionButtons(QTableWidget *table, const QString &id, std::function<void()> editAction,
                                 std::function<void()> deleteAction);
    QWidget *createActionButtons(QTableWidget *table, const QString &id, std::function<void()> viewAction,
                                 std::function<void()> deleteAction, bool isView);
    void selectRowById(QTableWidget *table, const QString &id);
    QFrame *createDetailsFrame(const QString &title, QTextEdit *&detailsText);
    void setupTableCommon(QTableWidget *table, int rowHeight = 50);
    bool isNumericId(const QString &text);
    QStringList preparePropertyIdsList();
    QStringList prepareClientIdsList();

    EstateAgency *agency;

    QTableWidget *propertiesTable;
    QPushButton *addPropertyBtn;
    QPushButton *refreshPropertyBtn;
    QPushButton *searchPropertyBtn;
    QLineEdit *searchPropertyEdit;
    QTextEdit *propertyDetailsText;

    QTableWidget *clientsTable;
    QPushButton *addClientBtn;
    QPushButton *refreshClientBtn;
    QPushButton *searchClientBtn;
    QLineEdit *searchClientEdit;
    QTextEdit *clientDetailsText;

    QTableWidget *transactionsTable;
    QPushButton *addTransactionBtn;
    QPushButton *refreshTransactionBtn;
    QPushButton *searchTransactionBtn;
    QLineEdit *searchTransactionEdit;
    QTextEdit *transactionDetailsText;

    QTableWidget *auctionsTable;
    QPushButton *addAuctionBtn;
    QPushButton *refreshAuctionBtn;
    QPushButton *searchAuctionBtn;
    QLineEdit *searchAuctionEdit;
    QTextEdit *auctionDetailsText;

    QListWidget *navigationList;
    QStackedWidget *contentStack;

    QWidget *dashboardWidget;
    QWidget *propertiesPageWidget;
    QWidget *clientsPageWidget;
    QWidget *transactionsPageWidget;
    QWidget *auctionsPageWidget;

    QLabel *statsPropertiesLabel;
    QLabel *statsClientsLabel;
    QLabel *statsTransactionsLabel;
    QLabel *statsAvailableLabel;
    QLabel *statsAuctionsLabel;
};

#endif // MAINWINDOW_H
