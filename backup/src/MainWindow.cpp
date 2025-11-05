#include "../include/MainWindow.h"
#include "../include/PropertyDialog.h"
#include "../include/ClientDialog.h"
#include "../include/TransactionDialog.h"
#include "../include/Apartment.h"
#include "../include/House.h"
#include "../include/CommercialProperty.h"
#include <QMenuBar>
#include <QMenu>
#include <QStatusBar>
#include <QLayout>
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QTabWidget>
#include <QAbstractItemView>
#include <QSplitter>
#include <QMessageBox>
#include <sstream>
#include <iomanip>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    agency = EstateAgency::getInstance();
    agency->loadAllData();
    
    applyStyles(); // Применяем стили первым делом
    setupUI();
    setupMenuBar();
    
    setWindowTitle("Информационная система продажи недвижимости");
    setMinimumSize(1200, 700);
    resize(1400, 800);
    
    refreshProperties();
    refreshClients();
    refreshTransactions();
}

MainWindow::~MainWindow()
{
    EstateAgency::destroyInstance();
}

void MainWindow::applyStyles()
{
    setStyleSheet(R"(
        QMainWindow {
            background-color: #1e1e1e;
        }
        
        QTabWidget::pane {
            border: 1px solid #3d3d3d;
            background-color: #2d2d2d;
            top: -1px;
        }
        
        QTabBar::tab {
            background-color: #2d2d2d;
            color: #d0d0d0;
            padding: 8px 20px;
            margin-right: 2px;
            border-top-left-radius: 4px;
            border-top-right-radius: 4px;
            font-weight: 500;
            font-size: 11pt;
            min-width: 120px;
        }
        
        QTabBar::tab:selected {
            background-color: #3d3d3d;
            color: #ffffff;
            border-bottom: 2px solid #5a9;
        }
        
        QTabBar::tab:hover {
            background-color: #353535;
        }
        
        QPushButton {
            background-color: #404040;
            color: white;
            border: none;
            padding: 6px 14px;
            border-radius: 3px;
            font-weight: 500;
            font-size: 9pt;
            min-width: 80px;
        }
        
        QPushButton:hover {
            background-color: #505050;
        }
        
        QPushButton:pressed {
            background-color: #303030;
        }
        
        QPushButton[text="Добавить"] {
            background-color: #2d5a3d;
        }
        
        QPushButton[text="Добавить"]:hover {
            background-color: #3d6a4d;
        }
        
        QPushButton[text="Удалить"] {
            background-color: #5a2d2d;
        }
        
        QPushButton[text="Удалить"]:hover {
            background-color: #6a3d3d;
        }
        
        QPushButton[text="Редактировать"] {
            background-color: #2d3d5a;
        }
        
        QPushButton[text="Редактировать"]:hover {
            background-color: #3d4d6a;
        }
        
        QPushButton[text="Обновить"], QPushButton[text="Найти"] {
            background-color: #4a4a4a;
        }
        
        QPushButton[text="Обновить"]:hover, QPushButton[text="Найти"]:hover {
            background-color: #5a5a5a;
        }
        
        QTableWidget {
            background-color: #2d2d2d;
            alternate-background-color: #252525;
            border: 1px solid #3d3d3d;
            gridline-color: #404040;
            color: #d0d0d0;
        }
        
        QTableWidget::item {
            padding: 6px;
            color: #d0d0d0;
        }
        
        QTableWidget::item:selected {
            background-color: #5a9;
            color: white;
        }
        
        QHeaderView::section {
            background-color: #3d3d3d;
            color: #ffffff;
            padding: 8px;
            border: none;
            font-weight: 600;
            font-size: 9pt;
        }
        
        QLineEdit, QTextEdit {
            border: 1px solid #404040;
            border-radius: 3px;
            padding: 6px;
            background-color: #2d2d2d;
            color: #d0d0d0;
            font-size: 10pt;
        }
        
        QLineEdit:focus, QTextEdit:focus {
            border: 1px solid #5a9;
            outline: none;
        }
        
        QLabel {
            color: #d0d0d0;
            font-size: 10pt;
        }
        
        QGroupBox {
            border: 1px solid #404040;
            border-radius: 4px;
            margin-top: 12px;
            font-weight: 600;
            font-size: 10pt;
            color: #d0d0d0;
            background-color: #2d2d2d;
        }
        
        QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top left;
            padding: 2px 8px;
            background-color: #3d3d3d;
            color: #ffffff;
            border-radius: 2px;
        }
        
        QComboBox {
            border: 1px solid #404040;
            border-radius: 3px;
            padding: 6px;
            background-color: #2d2d2d;
            color: #d0d0d0;
            font-size: 10pt;
        }
        
        QComboBox:focus {
            border: 1px solid #5a9;
        }
        
        QComboBox::drop-down {
            border: none;
        }
        
        QComboBox QAbstractItemView {
            background-color: #2d2d2d;
            color: #d0d0d0;
            selection-background-color: #5a9;
            selection-color: white;
            border: 1px solid #404040;
        }
        
        QSpinBox, QDoubleSpinBox {
            border: 1px solid #404040;
            border-radius: 3px;
            padding: 6px;
            background-color: #2d2d2d;
            color: #d0d0d0;
            font-size: 10pt;
        }
        
        QSpinBox:focus, QDoubleSpinBox:focus {
            border: 1px solid #5a9;
        }
        
        QCheckBox {
            color: #d0d0d0;
            font-size: 10pt;
        }
        
        QCheckBox::indicator {
            width: 18px;
            height: 18px;
            border: 1px solid #404040;
            border-radius: 3px;
            background-color: #2d2d2d;
        }
        
        QCheckBox::indicator:checked {
            background-color: #5a9;
            border-color: #5a9;
        }
        
        QDialog {
            background-color: #2d2d2d;
        }
        
        QDialog QLabel {
            color: #d0d0d0;
        }
        
        QDialog QLineEdit, QDialog QTextEdit, QDialog QComboBox,
        QDialog QSpinBox, QDialog QDoubleSpinBox {
            background-color: #2d2d2d;
            color: #d0d0d0;
            border: 1px solid #404040;
        }
        
        QDialog QGroupBox {
            background-color: #2d2d2d;
            color: #d0d0d0;
        }
        
        QMenuBar {
            background-color: #252525;
            color: #d0d0d0;
            border-bottom: 1px solid #3d3d3d;
        }
        
        QMenuBar::item {
            padding: 6px 12px;
        }
        
        QMenuBar::item:selected {
            background-color: #3d3d3d;
        }
        
        QMenu {
            background-color: #2d2d2d;
            color: #d0d0d0;
            border: 1px solid #404040;
        }
        
        QMenu::item:selected {
            background-color: #5a9;
            color: white;
        }
        
        QStatusBar {
            background-color: #252525;
            color: #d0d0d0;
            border-top: 1px solid #3d3d3d;
        }
        
        QMessageBox {
            background-color: #2d2d2d;
        }
        
        QMessageBox QLabel {
            color: #d0d0d0;
        }
    )");
}

void MainWindow::setupUI()
{
    tabWidget = new QTabWidget(this);
    setCentralWidget(tabWidget);
    
    setupPropertiesTab();
    setupClientsTab();
    setupTransactionsTab();
}

void MainWindow::setupMenuBar()
{
    QMenu* fileMenu = menuBar()->addMenu("Файл");
    fileMenu->addAction("Сохранить все", this, &MainWindow::saveAllData);
    fileMenu->addAction("Загрузить все", this, &MainWindow::loadAllData);
    fileMenu->addSeparator();
    fileMenu->addAction("Выход", this, &QWidget::close);
    
    statusBar()->showMessage("Готов к работе");
}

void MainWindow::setupPropertiesTab()
{
    QWidget* propertiesWidget = new QWidget;
    QVBoxLayout* mainLayout = new QVBoxLayout(propertiesWidget);
    
    // Кнопки управления
    QHBoxLayout* buttonLayout = new QHBoxLayout;
    addPropertyBtn = new QPushButton("Добавить");
    editPropertyBtn = new QPushButton("Редактировать");
    deletePropertyBtn = new QPushButton("Удалить");
    refreshPropertyBtn = new QPushButton("Обновить");
    
    buttonLayout->addWidget(addPropertyBtn);
    buttonLayout->addWidget(editPropertyBtn);
    buttonLayout->addWidget(deletePropertyBtn);
    buttonLayout->addWidget(refreshPropertyBtn);
    buttonLayout->addStretch();
    
    // Поиск
    QHBoxLayout* searchLayout = new QHBoxLayout;
    searchLayout->addWidget(new QLabel("Поиск (ID или адрес):"));
    searchPropertyEdit = new QLineEdit;
    searchPropertyEdit->setPlaceholderText("ID (6-8 цифр) или адрес");
    searchPropertyBtn = new QPushButton("Найти");
    searchLayout->addWidget(searchPropertyEdit);
    searchLayout->addWidget(searchPropertyBtn);
    
    // Таблица
    propertiesTable = new QTableWidget;
    propertiesTable->setColumnCount(6);
    propertiesTable->setHorizontalHeaderLabels({"ID", "Тип", "Адрес", "Цена", "Площадь", "Доступность"});
    propertiesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    propertiesTable->setSelectionMode(QAbstractItemView::SingleSelection);
    propertiesTable->horizontalHeader()->setStretchLastSection(true);
    propertiesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    
    // Детали
    propertyDetailsText = new QTextEdit;
    propertyDetailsText->setReadOnly(true);
    propertyDetailsText->setMaximumHeight(200);
    
    QSplitter* splitter = new QSplitter(Qt::Vertical);
    splitter->addWidget(propertiesTable);
    splitter->addWidget(propertyDetailsText);
    
    mainLayout->addLayout(buttonLayout);
    mainLayout->addLayout(searchLayout);
    mainLayout->addWidget(splitter);
    
    tabWidget->addTab(propertiesWidget, "Недвижимость");
    
    connect(addPropertyBtn, &QPushButton::clicked, this, &MainWindow::addProperty);
    connect(editPropertyBtn, &QPushButton::clicked, this, &MainWindow::editProperty);
    connect(deletePropertyBtn, &QPushButton::clicked, this, &MainWindow::deleteProperty);
    connect(refreshPropertyBtn, &QPushButton::clicked, this, &MainWindow::refreshProperties);
    connect(searchPropertyBtn, &QPushButton::clicked, this, &MainWindow::searchProperties);
    connect(propertiesTable, &QTableWidget::itemSelectionChanged, this, &MainWindow::propertySelectionChanged);
}

void MainWindow::setupClientsTab()
{
    QWidget* clientsWidget = new QWidget;
    QVBoxLayout* mainLayout = new QVBoxLayout(clientsWidget);
    
    QHBoxLayout* buttonLayout = new QHBoxLayout;
    addClientBtn = new QPushButton("Добавить");
    editClientBtn = new QPushButton("Редактировать");
    deleteClientBtn = new QPushButton("Удалить");
    refreshClientBtn = new QPushButton("Обновить");
    
    buttonLayout->addWidget(addClientBtn);
    buttonLayout->addWidget(editClientBtn);
    buttonLayout->addWidget(deleteClientBtn);
    buttonLayout->addWidget(refreshClientBtn);
    buttonLayout->addStretch();
    
    QHBoxLayout* searchLayout = new QHBoxLayout;
    searchLayout->addWidget(new QLabel("Поиск (ID или имя):"));
    searchClientEdit = new QLineEdit;
    searchClientEdit->setPlaceholderText("ID (6-8 цифр) или имя");
    searchClientBtn = new QPushButton("Найти");
    searchLayout->addWidget(searchClientEdit);
    searchLayout->addWidget(searchClientBtn);
    
    clientsTable = new QTableWidget;
    clientsTable->setColumnCount(5);
    clientsTable->setHorizontalHeaderLabels({"ID", "Имя", "Телефон", "Email", "Дата регистрации"});
    clientsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    clientsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    clientsTable->horizontalHeader()->setStretchLastSection(true);
    clientsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    clientsTable->setAlternatingRowColors(true);
    
    // Детали клиента и связанные сделки
    clientDetailsText = new QTextEdit;
    clientDetailsText->setReadOnly(true);
    clientDetailsText->setMaximumHeight(200);
    
    QSplitter* clientSplitter = new QSplitter(Qt::Vertical);
    clientSplitter->addWidget(clientsTable);
    clientSplitter->addWidget(clientDetailsText);
    
    mainLayout->addLayout(buttonLayout);
    mainLayout->addLayout(searchLayout);
    mainLayout->addWidget(clientSplitter);
    
    tabWidget->addTab(clientsWidget, "Клиенты");
    
    connect(addClientBtn, &QPushButton::clicked, this, &MainWindow::addClient);
    connect(editClientBtn, &QPushButton::clicked, this, &MainWindow::editClient);
    connect(deleteClientBtn, &QPushButton::clicked, this, &MainWindow::deleteClient);
    connect(refreshClientBtn, &QPushButton::clicked, this, &MainWindow::refreshClients);
    connect(searchClientBtn, &QPushButton::clicked, this, &MainWindow::searchClients);
    connect(clientsTable, &QTableWidget::itemSelectionChanged, this, [this]() {
        int row = clientsTable->currentRow();
        if (row >= 0) {
            QString id = clientsTable->item(row, 0)->text();
            Client* client = agency->getClientManager().findClient(id.toStdString());
            if (client) {
                showClientDetails(client);
                showClientTransactions(id.toStdString());
            }
        }
    });
}

void MainWindow::setupTransactionsTab()
{
    QWidget* transactionsWidget = new QWidget;
    QVBoxLayout* mainLayout = new QVBoxLayout(transactionsWidget);
    
    QHBoxLayout* buttonLayout = new QHBoxLayout;
    addTransactionBtn = new QPushButton("Добавить");
    editTransactionBtn = new QPushButton("Редактировать");
    deleteTransactionBtn = new QPushButton("Удалить");
    refreshTransactionBtn = new QPushButton("Обновить");
    
    buttonLayout->addWidget(addTransactionBtn);
    buttonLayout->addWidget(editTransactionBtn);
    buttonLayout->addWidget(deleteTransactionBtn);
    buttonLayout->addWidget(refreshTransactionBtn);
    buttonLayout->addStretch();
    
    // Поиск
    QHBoxLayout* searchLayout = new QHBoxLayout;
    searchLayout->addWidget(new QLabel("Поиск по ID:"));
    searchTransactionEdit = new QLineEdit;
    searchTransactionEdit->setPlaceholderText("ID сделки (6-8 цифр)");
    searchTransactionBtn = new QPushButton("Найти");
    searchLayout->addWidget(searchTransactionEdit);
    searchLayout->addWidget(searchTransactionBtn);
    
    transactionsTable = new QTableWidget;
    transactionsTable->setColumnCount(6);
    transactionsTable->setHorizontalHeaderLabels({"ID", "Недвижимость", "Клиент", "Цена", "Дата", "Статус"});
    transactionsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    transactionsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    transactionsTable->horizontalHeader()->setStretchLastSection(true);
    transactionsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    transactionsTable->setAlternatingRowColors(true);
    
    // Детали сделки
    transactionDetailsText = new QTextEdit;
    transactionDetailsText->setReadOnly(true);
    transactionDetailsText->setMaximumHeight(250);
    
    QSplitter* transSplitter = new QSplitter(Qt::Vertical);
    transSplitter->addWidget(transactionsTable);
    transSplitter->addWidget(transactionDetailsText);
    
    mainLayout->addLayout(buttonLayout);
    mainLayout->addLayout(searchLayout);
    mainLayout->addWidget(transSplitter);
    
    tabWidget->addTab(transactionsWidget, "Сделки");
    
    connect(addTransactionBtn, &QPushButton::clicked, this, &MainWindow::addTransaction);
    connect(editTransactionBtn, &QPushButton::clicked, this, &MainWindow::editTransaction);
    connect(deleteTransactionBtn, &QPushButton::clicked, this, &MainWindow::deleteTransaction);
    connect(refreshTransactionBtn, &QPushButton::clicked, this, &MainWindow::refreshTransactions);
    connect(searchTransactionBtn, &QPushButton::clicked, this, &MainWindow::searchTransactions);
    connect(transactionsTable, &QTableWidget::itemSelectionChanged, this, [this]() {
        int row = transactionsTable->currentRow();
        if (row >= 0) {
            QString id = transactionsTable->item(row, 0)->text();
            Transaction* trans = agency->getTransactionManager().findTransaction(id.toStdString());
            if (trans) {
                showTransactionDetails(trans);
            }
        }
    });
}

void MainWindow::updatePropertiesTable()
{
    propertiesTable->setRowCount(0);
    auto properties = agency->getPropertyManager().getAllProperties();
    
    for (Property* prop : properties) {
        int row = propertiesTable->rowCount();
        propertiesTable->insertRow(row);
        
        propertiesTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(prop->getId())));
        propertiesTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(prop->getType())));
        propertiesTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(prop->getAddress())));
        propertiesTable->setItem(row, 3, new QTableWidgetItem(QString::number(prop->getPrice(), 'f', 2)));
        propertiesTable->setItem(row, 4, new QTableWidgetItem(QString::number(prop->getArea(), 'f', 2)));
        propertiesTable->setItem(row, 5, new QTableWidgetItem(prop->getIsAvailable() ? "Да" : "Нет"));
    }
}

void MainWindow::updateClientsTable()
{
    clientsTable->setRowCount(0);
    auto clients = agency->getClientManager().getAllClients();
    
    for (Client* client : clients) {
        int row = clientsTable->rowCount();
        clientsTable->insertRow(row);
        
        clientsTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(client->getId())));
        clientsTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(client->getName())));
        clientsTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(client->getPhone())));
        clientsTable->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(client->getEmail())));
        clientsTable->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(client->getRegistrationDate())));
    }
}

void MainWindow::updateTransactionsTable()
{
    transactionsTable->setRowCount(0);
    auto transactions = agency->getTransactionManager().getAllTransactions();
    
    for (Transaction* trans : transactions) {
        int row = transactionsTable->rowCount();
        transactionsTable->insertRow(row);
        
        Property* prop = agency->getPropertyManager().findProperty(trans->getPropertyId());
        Client* client = agency->getClientManager().findClient(trans->getClientId());
        
        transactionsTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(trans->getId())));
        transactionsTable->setItem(row, 1, new QTableWidgetItem(prop ? QString::fromStdString(prop->getAddress()) : "N/A"));
        transactionsTable->setItem(row, 2, new QTableWidgetItem(client ? QString::fromStdString(client->getName()) : "N/A"));
        transactionsTable->setItem(row, 3, new QTableWidgetItem(QString::number(trans->getFinalPrice(), 'f', 2)));
        transactionsTable->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(trans->getDate())));
        
        QString statusText;
        if (trans->getStatus() == "pending") statusText = "В ожидании";
        else if (trans->getStatus() == "completed") statusText = "Завершена";
        else statusText = "Отменена";
        transactionsTable->setItem(row, 5, new QTableWidgetItem(statusText));
    }
}

void MainWindow::addProperty()
{
    PropertyDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        try {
            QString id = dialog.getId();
            QString city = dialog.getCity();
            QString street = dialog.getStreet();
            QString house = dialog.getHouse();
            double price = dialog.getPrice();
            double area = dialog.getArea();
            QString description = dialog.getDescription();
            
            if (dialog.getPropertyType() == PropertyDialog::TypeApartment) {
                agency->getPropertyManager().addApartment(
                    id.toStdString(), city.toStdString(), street.toStdString(), house.toStdString(),
                    price, area, description.toStdString(), dialog.getRooms(), dialog.getFloor(),
                    dialog.getHasBalcony(), dialog.getHasElevator());
            } else if (dialog.getPropertyType() == PropertyDialog::TypeHouse) {
                agency->getPropertyManager().addHouse(
                    id.toStdString(), city.toStdString(), street.toStdString(), house.toStdString(),
                    price, area, description.toStdString(), dialog.getFloors(), dialog.getRooms(),
                    dialog.getLandArea(), dialog.getHasGarage(), dialog.getHasGarden());
            } else {
                agency->getPropertyManager().addCommercialProperty(
                    id.toStdString(), city.toStdString(), street.toStdString(), house.toStdString(),
                    price, area, description.toStdString(), dialog.getBusinessType().toStdString(),
                    dialog.getHasParking(), dialog.getParkingSpaces(),
                    dialog.getIsVisibleFromStreet());
            }
            
            refreshProperties();
            statusBar()->showMessage("Недвижимость добавлена", 3000);
        } catch (const std::exception& e) {
            QMessageBox::warning(this, "Ошибка", QString("Ошибка добавления: %1").arg(e.what()));
        }
    }
}

void MainWindow::editProperty()
{
    int row = propertiesTable->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "Информация", "Выберите недвижимость для редактирования");
        return;
    }
    
    QString id = propertiesTable->item(row, 0)->text();
    Property* prop = agency->getPropertyManager().findProperty(id.toStdString());
    if (!prop) return;
    
    PropertyDialog dialog(this, prop);
    if (dialog.exec() == QDialog::Accepted) {
        try {
            // Удаляем старую и добавляем новую
            agency->getPropertyManager().removeProperty(id.toStdString());
            
            QString newId = dialog.getId();
            QString city = dialog.getCity();
            QString street = dialog.getStreet();
            QString house = dialog.getHouse();
            double price = dialog.getPrice();
            double area = dialog.getArea();
            QString description = dialog.getDescription();
            
            if (dialog.getPropertyType() == PropertyDialog::TypeApartment) {
                agency->getPropertyManager().addApartment(
                    newId.toStdString(), city.toStdString(), street.toStdString(), house.toStdString(),
                    price, area, description.toStdString(), dialog.getRooms(), dialog.getFloor(),
                    dialog.getHasBalcony(), dialog.getHasElevator());
            } else if (dialog.getPropertyType() == PropertyDialog::TypeHouse) {
                agency->getPropertyManager().addHouse(
                    newId.toStdString(), city.toStdString(), street.toStdString(), house.toStdString(),
                    price, area, description.toStdString(), dialog.getFloors(), dialog.getRooms(),
                    dialog.getLandArea(), dialog.getHasGarage(), dialog.getHasGarden());
            } else {
                agency->getPropertyManager().addCommercialProperty(
                    newId.toStdString(), city.toStdString(), street.toStdString(), house.toStdString(),
                    price, area, description.toStdString(), dialog.getBusinessType().toStdString(),
                    dialog.getHasParking(), dialog.getParkingSpaces(),
                    dialog.getIsVisibleFromStreet());
            }
            
            refreshProperties();
            statusBar()->showMessage("Недвижимость обновлена", 3000);
        } catch (const std::exception& e) {
            QMessageBox::warning(this, "Ошибка", QString("Ошибка редактирования: %1").arg(e.what()));
        }
    }
}

void MainWindow::deleteProperty()
{
    int row = propertiesTable->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "Информация", "Выберите недвижимость для удаления");
        return;
    }
    
    QString id = propertiesTable->item(row, 0)->text();
    int ret = QMessageBox::question(this, "Подтверждение", "Удалить недвижимость?",
                                    QMessageBox::Yes | QMessageBox::No);
    
    if (ret == QMessageBox::Yes) {
        agency->getPropertyManager().removeProperty(id.toStdString());
        refreshProperties();
        statusBar()->showMessage("Недвижимость удалена", 3000);
    }
}

void MainWindow::refreshProperties()
{
    updatePropertiesTable();
    statusBar()->showMessage(QString("Недвижимость: %1 объектов").arg(agency->getPropertyManager().getCount()), 2000);
}

void MainWindow::searchProperties()
{
    QString searchText = searchPropertyEdit->text();
    if (searchText.isEmpty()) {
        refreshProperties();
        return;
    }
    
    propertiesTable->setRowCount(0);
    
    // Проверяем, является ли поисковый запрос ID (только цифры)
    bool isNumericOnly = true;
    for (QChar ch : searchText) {
        if (!ch.isDigit()) {
            isNumericOnly = false;
            break;
        }
    }
    
    std::vector<Property*> properties;
    if (isNumericOnly && searchText.length() >= 6 && searchText.length() <= 8) {
        // Поиск по ID
        Property* prop = agency->getPropertyManager().findProperty(searchText.toStdString());
        if (prop) {
            properties.push_back(prop);
        }
    } else {
        // Поиск по адресу (город, улица, дом)
        properties = agency->getPropertyManager().searchByAddress(searchText.toStdString(), "", "");
    }
    
    for (Property* prop : properties) {
        int row = propertiesTable->rowCount();
        propertiesTable->insertRow(row);
        
        propertiesTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(prop->getId())));
        propertiesTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(prop->getType())));
        propertiesTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(prop->getAddress())));
        propertiesTable->setItem(row, 3, new QTableWidgetItem(QString::number(prop->getPrice(), 'f', 2)));
        propertiesTable->setItem(row, 4, new QTableWidgetItem(QString::number(prop->getArea(), 'f', 2)));
        propertiesTable->setItem(row, 5, new QTableWidgetItem(prop->getIsAvailable() ? "Да" : "Нет"));
    }
    
    if (properties.empty()) {
        statusBar()->showMessage("Ничего не найдено", 2000);
    } else {
        statusBar()->showMessage(QString("Найдено: %1 объектов").arg(properties.size()), 2000);
    }
}

void MainWindow::propertySelectionChanged()
{
    int row = propertiesTable->currentRow();
    if (row >= 0) {
        QString id = propertiesTable->item(row, 0)->text();
        Property* prop = agency->getPropertyManager().findProperty(id.toStdString());
        if (prop) {
            showPropertyDetails(prop);
            showPropertyTransactions(id.toStdString());
        }
    }
}

void MainWindow::showPropertyDetails(Property* prop)
{
    std::ostringstream oss;
    oss << *prop;
    
    if (Apartment* apt = dynamic_cast<Apartment*>(prop)) {
        oss << "\nКомнат: " << apt->getRooms();
        oss << "\nЭтаж: " << apt->getFloor();
        oss << "\nБалкон: " << (apt->getHasBalcony() ? "Да" : "Нет");
        oss << "\nЛифт: " << (apt->getHasElevator() ? "Да" : "Нет");
    } else if (House* house = dynamic_cast<House*>(prop)) {
        oss << "\nЭтажей: " << house->getFloors();
        oss << "\nКомнат: " << house->getRooms();
        oss << "\nУчасток: " << house->getLandArea() << " м²";
        oss << "\nГараж: " << (house->getHasGarage() ? "Да" : "Нет");
        oss << "\nСад: " << (house->getHasGarden() ? "Да" : "Нет");
    } else if (CommercialProperty* comm = dynamic_cast<CommercialProperty*>(prop)) {
        oss << "\nТип бизнеса: " << comm->getBusinessType();
        oss << "\nПарковка: " << (comm->getHasParking() ? "Да" : "Нет");
        oss << "\nМест парковки: " << comm->getParkingSpaces();
        oss << "\nВидимость с улицы: " << (comm->getIsVisibleFromStreet() ? "Да" : "Нет");
    }
    
    propertyDetailsText->setPlainText(QString::fromStdString(oss.str()));
}

void MainWindow::addClient()
{
    ClientDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        try {
            auto client = std::make_shared<Client>(
                dialog.getId().toStdString(),
                dialog.getName().toStdString(),
                dialog.getPhone().toStdString(),
                dialog.getEmail().toStdString());
            agency->getClientManager().addClient(client);
            refreshClients();
            statusBar()->showMessage("Клиент добавлен", 3000);
        } catch (const std::exception& e) {
            QMessageBox::warning(this, "Ошибка", QString("Ошибка добавления: %1").arg(e.what()));
        }
    }
}

void MainWindow::editClient()
{
    int row = clientsTable->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "Информация", "Выберите клиента для редактирования");
        return;
    }
    
    QString id = clientsTable->item(row, 0)->text();
    Client* client = agency->getClientManager().findClient(id.toStdString());
    if (!client) return;
    
    ClientDialog dialog(this, client);
    if (dialog.exec() == QDialog::Accepted) {
        try {
            agency->getClientManager().removeClient(id.toStdString());
            auto newClient = std::make_shared<Client>(
                dialog.getId().toStdString(),
                dialog.getName().toStdString(),
                dialog.getPhone().toStdString(),
                dialog.getEmail().toStdString());
            agency->getClientManager().addClient(newClient);
            refreshClients();
            statusBar()->showMessage("Клиент обновлен", 3000);
        } catch (const std::exception& e) {
            QMessageBox::warning(this, "Ошибка", QString("Ошибка редактирования: %1").arg(e.what()));
        }
    }
}

void MainWindow::deleteClient()
{
    int row = clientsTable->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "Информация", "Выберите клиента для удаления");
        return;
    }
    
    QString id = clientsTable->item(row, 0)->text();
    int ret = QMessageBox::question(this, "Подтверждение", "Удалить клиента?",
                                    QMessageBox::Yes | QMessageBox::No);
    
    if (ret == QMessageBox::Yes) {
        agency->getClientManager().removeClient(id.toStdString());
        refreshClients();
        statusBar()->showMessage("Клиент удален", 3000);
    }
}

void MainWindow::refreshClients()
{
    updateClientsTable();
    statusBar()->showMessage(QString("Клиенты: %1 записей").arg(agency->getClientManager().getCount()), 2000);
}

void MainWindow::searchClients()
{
    QString searchText = searchClientEdit->text();
    if (searchText.isEmpty()) {
        refreshClients();
        return;
    }
    
    clientsTable->setRowCount(0);
    
    // Проверяем, является ли поисковый запрос ID (только цифры)
    bool isNumericOnly = true;
    for (QChar ch : searchText) {
        if (!ch.isDigit()) {
            isNumericOnly = false;
            break;
        }
    }
    
    std::vector<Client*> clients;
    if (isNumericOnly && searchText.length() >= 6 && searchText.length() <= 8) {
        // Поиск по ID
        Client* client = agency->getClientManager().findClient(searchText.toStdString());
        if (client) {
            clients.push_back(client);
        }
    } else {
        // Поиск по имени
        clients = agency->getClientManager().searchByName(searchText.toStdString());
    }
    
    for (Client* client : clients) {
        int row = clientsTable->rowCount();
        clientsTable->insertRow(row);
        
        clientsTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(client->getId())));
        clientsTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(client->getName())));
        clientsTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(client->getPhone())));
        clientsTable->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(client->getEmail())));
        clientsTable->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(client->getRegistrationDate())));
    }
    
    if (clients.empty()) {
        statusBar()->showMessage("Ничего не найдено", 2000);
    } else {
        statusBar()->showMessage(QString("Найдено: %1 клиентов").arg(clients.size()), 2000);
    }
}

void MainWindow::addTransaction()
{
    QStringList propertyIds, clientIds;
    auto properties = agency->getPropertyManager().getAllProperties();
    auto clients = agency->getClientManager().getAllClients();
    
    for (Property* p : properties) {
        propertyIds << QString::fromStdString(p->getId());
    }
    for (Client* c : clients) {
        clientIds << QString::fromStdString(c->getId());
    }
    
    TransactionDialog dialog(this, nullptr, propertyIds, clientIds);
    if (dialog.exec() == QDialog::Accepted) {
        try {
            // Валидация перед добавлением
            std::string propertyId = dialog.getPropertyId().toStdString();
            std::string clientId = dialog.getClientId().toStdString();
            std::string status = dialog.getStatus().toStdString();
            
            if (!validateTransaction(propertyId, clientId, status)) {
                return; // Валидация не прошла, показываем ошибку
            }
            
            auto trans = std::make_shared<Transaction>(
                dialog.getId().toStdString(),
                propertyId,
                clientId,
                dialog.getFinalPrice(),
                status,
                dialog.getNotes().toStdString());
            agency->getTransactionManager().addTransaction(trans);
            
            // Помечаем недвижимость как недоступную, если сделка активна
            Property* prop = agency->getPropertyManager().findProperty(propertyId);
            if (prop && (status == "pending" || status == "completed")) {
                prop->setAvailable(false);
            }
            
            refreshTransactions();
            refreshProperties(); // Обновляем, чтобы показать изменение доступности
            statusBar()->showMessage("Сделка добавлена", 3000);
        } catch (const std::exception& e) {
            QMessageBox::warning(this, "Ошибка", QString("Ошибка добавления: %1").arg(e.what()));
        }
    }
}

void MainWindow::editTransaction()
{
    int row = transactionsTable->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "Информация", "Выберите сделку для редактирования");
        return;
    }
    
    QString id = transactionsTable->item(row, 0)->text();
    Transaction* trans = agency->getTransactionManager().findTransaction(id.toStdString());
    if (!trans) return;
    
    QStringList propertyIds, clientIds;
    auto properties = agency->getPropertyManager().getAllProperties();
    auto clients = agency->getClientManager().getAllClients();
    
    for (Property* p : properties) {
        propertyIds << QString::fromStdString(p->getId());
    }
    for (Client* c : clients) {
        clientIds << QString::fromStdString(c->getId());
    }
    
    TransactionDialog dialog(this, trans, propertyIds, clientIds);
    if (dialog.exec() == QDialog::Accepted) {
        try {
            std::string propertyId = dialog.getPropertyId().toStdString();
            std::string clientId = dialog.getClientId().toStdString();
            std::string status = dialog.getStatus().toStdString();
            
            // Валидация
            if (!validateTransaction(propertyId, clientId, status)) {
                return;
            }
            
            std::string oldPropertyId = trans->getPropertyId();
            Property* oldProp = agency->getPropertyManager().findProperty(oldPropertyId);
            
            agency->getTransactionManager().removeTransaction(id.toStdString());
            auto newTrans = std::make_shared<Transaction>(
                dialog.getId().toStdString(),
                propertyId,
                clientId,
                dialog.getFinalPrice(),
                status,
                dialog.getNotes().toStdString());
            agency->getTransactionManager().addTransaction(newTrans);
            
            // Обновляем доступность недвижимости
            Property* prop = agency->getPropertyManager().findProperty(propertyId);
            if (prop) {
                bool shouldBeAvailable = (status == "cancelled");
                prop->setAvailable(shouldBeAvailable);
            }
            
            // Освобождаем старую недвижимость, если она была другая
            if (oldProp && oldPropertyId != propertyId) {
                auto oldTrans = agency->getTransactionManager().getTransactionsByProperty(oldPropertyId);
                bool hasActive = false;
                for (Transaction* t : oldTrans) {
                    if (t->getStatus() == "pending" || t->getStatus() == "completed") {
                        hasActive = true;
                        break;
                    }
                }
                if (!hasActive) {
                    oldProp->setAvailable(true);
                }
            }
            
            refreshTransactions();
            refreshProperties();
            statusBar()->showMessage("Сделка обновлена", 3000);
        } catch (const std::exception& e) {
            QMessageBox::warning(this, "Ошибка", QString("Ошибка редактирования: %1").arg(e.what()));
        }
    }
}

void MainWindow::deleteTransaction()
{
    int row = transactionsTable->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "Информация", "Выберите сделку для удаления");
        return;
    }
    
    QString id = transactionsTable->item(row, 0)->text();
    int ret = QMessageBox::question(this, "Подтверждение", "Удалить сделку?",
                                    QMessageBox::Yes | QMessageBox::No);
    
    if (ret == QMessageBox::Yes) {
        Transaction* trans = agency->getTransactionManager().findTransaction(id.toStdString());
        std::string propertyId = trans ? trans->getPropertyId() : "";
        
        agency->getTransactionManager().removeTransaction(id.toStdString());
        
        // Освобождаем недвижимость, если нет других активных сделок
        if (!propertyId.empty()) {
            Property* prop = agency->getPropertyManager().findProperty(propertyId);
            if (prop) {
                auto remainingTrans = agency->getTransactionManager().getTransactionsByProperty(propertyId);
                bool hasActive = false;
                for (Transaction* t : remainingTrans) {
                    if (t->getStatus() == "pending" || t->getStatus() == "completed") {
                        hasActive = true;
                        break;
                    }
                }
                if (!hasActive) {
                    prop->setAvailable(true);
                }
            }
        }
        
        refreshTransactions();
        refreshProperties();
        statusBar()->showMessage("Сделка удалена", 3000);
    }
}

void MainWindow::refreshTransactions()
{
    updateTransactionsTable();
    statusBar()->showMessage(QString("Сделки: %1 записей").arg(agency->getTransactionManager().getCount()), 2000);
}

void MainWindow::searchTransactions()
{
    QString searchText = searchTransactionEdit->text();
    if (searchText.isEmpty()) {
        refreshTransactions();
        return;
    }
    
    transactionsTable->setRowCount(0);
    
    // Проверяем, является ли поисковый запрос ID (только цифры)
    bool isNumericOnly = true;
    for (QChar ch : searchText) {
        if (!ch.isDigit()) {
            isNumericOnly = false;
            break;
        }
    }
    
    if (isNumericOnly && searchText.length() >= 6 && searchText.length() <= 8) {
        // Поиск по ID сделки
        Transaction* trans = agency->getTransactionManager().findTransaction(searchText.toStdString());
        if (trans) {
            int row = transactionsTable->rowCount();
            transactionsTable->insertRow(row);
            
            Property* prop = agency->getPropertyManager().findProperty(trans->getPropertyId());
            Client* client = agency->getClientManager().findClient(trans->getClientId());
            
            transactionsTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(trans->getId())));
            transactionsTable->setItem(row, 1, new QTableWidgetItem(prop ? QString::fromStdString(prop->getAddress()) : "N/A"));
            transactionsTable->setItem(row, 2, new QTableWidgetItem(client ? QString::fromStdString(client->getName()) : "N/A"));
            transactionsTable->setItem(row, 3, new QTableWidgetItem(QString::number(trans->getFinalPrice(), 'f', 2)));
            transactionsTable->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(trans->getDate())));
            
            QString statusText;
            if (trans->getStatus() == "pending") statusText = "В ожидании";
            else if (trans->getStatus() == "completed") statusText = "Завершена";
            else statusText = "Отменена";
            transactionsTable->setItem(row, 5, new QTableWidgetItem(statusText));
            
            statusBar()->showMessage("Сделка найдена", 2000);
        } else {
            statusBar()->showMessage("Сделка не найдена", 2000);
        }
    } else {
        QMessageBox::information(this, "Информация", "Введите ID сделки (6-8 цифр)");
        refreshTransactions();
    }
}

void MainWindow::saveAllData()
{
    try {
        agency->saveAllData();
        statusBar()->showMessage("Данные сохранены", 3000);
        QMessageBox::information(this, "Успех", "Все данные успешно сохранены");
    } catch (const std::exception& e) {
        QMessageBox::warning(this, "Ошибка", QString("Ошибка сохранения: %1").arg(e.what()));
    }
}

void MainWindow::loadAllData()
{
    try {
        agency->loadAllData();
        refreshProperties();
        refreshClients();
        refreshTransactions();
        statusBar()->showMessage("Данные загружены", 3000);
        QMessageBox::information(this, "Успех", "Данные успешно загружены");
    } catch (const std::exception& e) {
        QMessageBox::warning(this, "Ошибка", QString("Ошибка загрузки: %1").arg(e.what()));
    }
}

void MainWindow::showClientDetails(Client* client)
{
    if (!client) return;
    
    std::ostringstream oss;
    oss << "=== ИНФОРМАЦИЯ О КЛИЕНТЕ ===\n\n";
    oss << "ID: " << client->getId() << "\n";
    oss << "Имя: " << client->getName() << "\n";
    oss << "Телефон: " << client->getPhone() << "\n";
    oss << "Email: " << client->getEmail() << "\n";
    oss << "Дата регистрации: " << client->getRegistrationDate() << "\n";
    
    clientDetailsText->setPlainText(QString::fromStdString(oss.str()));
}

void MainWindow::showTransactionDetails(Transaction* trans)
{
    if (!trans) return;
    
    Property* prop = agency->getPropertyManager().findProperty(trans->getPropertyId());
    Client* client = agency->getClientManager().findClient(trans->getClientId());
    
    std::ostringstream oss;
    oss << "=== ИНФОРМАЦИЯ О СДЕЛКЕ ===\n\n";
    oss << "ID сделки: " << trans->getId() << "\n";
    oss << "Дата: " << trans->getDate() << "\n";
    oss << "Цена: " << std::fixed << std::setprecision(2) << trans->getFinalPrice() << " руб.\n";
    
    QString statusText;
    if (trans->getStatus() == "pending") statusText = "В ожидании";
    else if (trans->getStatus() == "completed") statusText = "Завершена";
    else statusText = "Отменена";
    oss << "Статус: " << statusText.toStdString() << "\n";
    oss << "Примечания: " << trans->getNotes() << "\n\n";
    
    oss << "=== НЕДВИЖИМОСТЬ ===\n";
    if (prop) {
        oss << "ID: " << prop->getId() << "\n";
        oss << "Тип: " << prop->getType() << "\n";
        oss << "Адрес: " << prop->getAddress() << "\n";
        oss << "Цена: " << prop->getPrice() << " руб.\n";
        oss << "Площадь: " << prop->getArea() << " м²\n";
        oss << "Доступность: " << (prop->getIsAvailable() ? "Да" : "Нет") << "\n";
    } else {
        oss << "НЕДВИЖИМОСТЬ НЕ НАЙДЕНА!\n";
    }
    
    oss << "\n=== КЛИЕНТ ===\n";
    if (client) {
        oss << "ID: " << client->getId() << "\n";
        oss << "Имя: " << client->getName() << "\n";
        oss << "Телефон: " << client->getPhone() << "\n";
        oss << "Email: " << client->getEmail() << "\n";
    } else {
        oss << "КЛИЕНТ НЕ НАЙДЕН!\n";
    }
    
    transactionDetailsText->setPlainText(QString::fromStdString(oss.str()));
}

void MainWindow::showPropertyTransactions(const std::string& propertyId)
{
    auto transactions = agency->getTransactionManager().getTransactionsByProperty(propertyId);
    
    std::ostringstream oss;
    if (!propertyDetailsText->toPlainText().isEmpty()) {
        oss << propertyDetailsText->toPlainText().toStdString();
        oss << "\n\n=== СВЯЗАННЫЕ СДЕЛКИ ===\n";
    } else {
        oss << "=== СВЯЗАННЫЕ СДЕЛКИ ===\n";
    }
    
    if (transactions.empty()) {
        oss << "Нет связанных сделок\n";
    } else {
        for (Transaction* trans : transactions) {
            Client* client = agency->getClientManager().findClient(trans->getClientId());
            oss << "\nСделка ID: " << trans->getId() << "\n";
            oss << "Клиент: " << (client ? client->getName() : "N/A") << "\n";
            oss << "Цена: " << trans->getFinalPrice() << " руб.\n";
            oss << "Статус: " << trans->getStatus() << "\n";
            oss << "Дата: " << trans->getDate() << "\n";
        }
    }
    
    propertyDetailsText->setPlainText(QString::fromStdString(oss.str()));
}

void MainWindow::showClientTransactions(const std::string& clientId)
{
    auto transactions = agency->getTransactionManager().getTransactionsByClient(clientId);
    
    std::ostringstream oss;
    if (!clientDetailsText->toPlainText().isEmpty()) {
        oss << clientDetailsText->toPlainText().toStdString();
        oss << "\n\n=== СВЯЗАННЫЕ СДЕЛКИ (" << transactions.size() << ") ===\n";
    } else {
        oss << "=== СВЯЗАННЫЕ СДЕЛКИ (" << transactions.size() << ") ===\n";
    }
    
    if (transactions.empty()) {
        oss << "Нет связанных сделок\n";
    } else {
        for (Transaction* trans : transactions) {
            Property* prop = agency->getPropertyManager().findProperty(trans->getPropertyId());
            oss << "\nСделка ID: " << trans->getId() << "\n";
            oss << "Недвижимость: " << (prop ? prop->getAddress() : "N/A") << "\n";
            oss << "Цена: " << trans->getFinalPrice() << " руб.\n";
            oss << "Статус: " << trans->getStatus() << "\n";
            oss << "Дата: " << trans->getDate() << "\n";
        }
    }
    
    clientDetailsText->setPlainText(QString::fromStdString(oss.str()));
}

bool MainWindow::validateTransaction(const std::string& propertyId, const std::string& clientId, const std::string& status)
{
    // Проверка существования недвижимости
    Property* prop = agency->getPropertyManager().findProperty(propertyId);
    if (!prop) {
        QMessageBox::warning(this, "Ошибка валидации", 
                            QString("Недвижимость с ID '%1' не найдена!").arg(QString::fromStdString(propertyId)));
        return false;
    }
    
    // Проверка существования клиента
    Client* client = agency->getClientManager().findClient(clientId);
    if (!client) {
        QMessageBox::warning(this, "Ошибка валидации", 
                            QString("Клиент с ID '%1' не найден!").arg(QString::fromStdString(clientId)));
        return false;
    }
    
    // Проверка доступности недвижимости при создании новой сделки
    if (status == "pending" || status == "completed") {
        if (!prop->getIsAvailable()) {
            // Проверяем, нет ли уже активных сделок для этой недвижимости
            auto existingTrans = agency->getTransactionManager().getTransactionsByProperty(propertyId);
            bool hasActive = false;
            for (Transaction* t : existingTrans) {
                if (t->getStatus() == "pending" || t->getStatus() == "completed") {
                    hasActive = true;
                    break;
                }
            }
            
            if (hasActive) {
                QMessageBox::warning(this, "Ошибка валидации", 
                                    "Недвижимость недоступна или уже используется в другой активной сделке!");
                return false;
            }
        }
    }
    
    return true;
}

