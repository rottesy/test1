#include "../include/MainWindow.h"
#include "../include/Apartment.h"
#include "../include/Auction.h"
#include "../include/AuctionDialog.h"
#include "../include/ClientDialog.h"
#include "../include/CommercialProperty.h"
#include "../include/Constants.h"
#include "../include/House.h"
#include "../include/PropertyDialog.h"
#include "../include/TableHelper.h"
#include "../include/TransactionDialog.h"
#include "../include/Utils.h"
#include <QAbstractItemView>
#include <QDebug>
#include <QFrame>
#include <QHeaderView>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QListWidget>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QSplitter>
#include <QStackedWidget>
#include <QStatusBar>
#include <QTabWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTextEdit>
#include <QTimer>
#include <functional>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
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
  // refreshAuctions() вызывается после полной инициализации в setupNewUI()
}

MainWindow::~MainWindow() { EstateAgency::destroyInstance(); }

void MainWindow::applyStyles() {
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
        
        /* Новые стили для кардинально другого интерфейса */
        QListWidget {
            background-color: #1a1a1a;
            border: none;
            outline: none;
            color: #d0d0d0;
            font-size: 11pt;
            padding: 10px;
        }
        
        QListWidget::item {
            padding: 15px 20px;
            border-radius: 8px;
            margin: 3px;
            background-color: #252525;
            border-left: 4px solid transparent;
        }
        
        QListWidget::item:hover {
            background-color: #2d2d2d;
            border-left-color: #5a9;
        }
        
        QListWidget::item:selected {
            background-color: #353535;
            border-left-color: #7bb;
            color: #ffffff;
            font-weight: 600;
        }
        
        QStackedWidget {
            background-color: #1e1e1e;
        }
        
        QFrame {
            background-color: #252525;
            border: 1px solid #3d3d3d;
            border-radius: 8px;
            padding: 15px;
        }
        
        QScrollArea {
            border: none;
            background-color: #1e1e1e;
        }
        
        QScrollArea QWidget {
            background-color: #1e1e1e;
        }
    )");
}

void MainWindow::setupUI() {
  // Новый интерфейс с боковой навигацией
  setupNewUI();
}

void MainWindow::setupMenuBar() {
  QMenu *fileMenu = menuBar()->addMenu("Файл");
  fileMenu->addAction("Сохранить все", QKeySequence::Save, this,
                      &MainWindow::saveAllData);
  fileMenu->addAction("Загрузить все", QKeySequence::Open, this,
                      &MainWindow::loadAllData);
  fileMenu->addSeparator();
  fileMenu->addAction("Выход", QKeySequence::Quit, this, &QWidget::close);

  QMenu *viewMenu = menuBar()->addMenu("Вид");
  viewMenu->addAction("Обновить все", this, [this]() {
    refreshProperties();
    refreshClients();
    refreshTransactions();
    updateDashboardStats();
  });

  showStatusMessage(Constants::Messages::READY);
}

void MainWindow::setupNewUI() {
  QWidget *centralWidget = new QWidget(this);
  QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
  mainLayout->setContentsMargins(0, 0, 0, 0);
  mainLayout->setSpacing(0);

  // Боковая панель навигации
  QFrame *sidebar = new QFrame;
  sidebar->setFixedWidth(250);
  sidebar->setStyleSheet(
      "background-color: #1a1a1a; border-right: 1px solid #3d3d3d;");

  QVBoxLayout *sidebarLayout = new QVBoxLayout(sidebar);
  sidebarLayout->setContentsMargins(0, 20, 0, 20);
  sidebarLayout->setSpacing(10);

  // Заголовок
  QLabel *titleLabel = new QLabel("Недвижимость");
  titleLabel->setStyleSheet("color: #ffffff; font-size: 18pt; font-weight: "
                            "bold; padding: 10px 20px;");
  sidebarLayout->addWidget(titleLabel);

  sidebarLayout->addSpacing(20);

  // Навигационный список
  navigationList = new QListWidget;
  navigationList->addItem("Панель управления");
  navigationList->addItem("Недвижимость");
  navigationList->addItem("Клиенты");
  navigationList->addItem("Сделки");
  navigationList->addItem("Аукционы");
  navigationList->setCurrentRow(0);
  sidebarLayout->addWidget(navigationList);

  sidebarLayout->addStretch();

  // Кнопки быстрого доступа
  QPushButton *saveBtn = new QPushButton("Сохранить");
  QPushButton *loadBtn = new QPushButton("Загрузить");
  saveBtn->setStyleSheet(
      "QPushButton { background-color: #2d5a3d; padding: 10px; border-radius: "
      "5px; } QPushButton:hover { background-color: #3d6a4d; }");
  loadBtn->setStyleSheet(
      "QPushButton { background-color: #2d3d5a; padding: 10px; border-radius: "
      "5px; } QPushButton:hover { background-color: #3d4d6a; }");
  connect(saveBtn, &QPushButton::clicked, this, &MainWindow::saveAllData);
  connect(loadBtn, &QPushButton::clicked, this, &MainWindow::loadAllData);
  sidebarLayout->addWidget(saveBtn);
  sidebarLayout->addWidget(loadBtn);

  // Контентная область
  contentStack = new QStackedWidget;

  // Создаем страницы
  setupDashboard();
  setupPropertiesPage();
  setupClientsPage();
  setupTransactionsPage();
  setupAuctionsPage();

  // Добавляем в стек
  contentStack->addWidget(dashboardWidget);
  contentStack->addWidget(propertiesPageWidget);
  contentStack->addWidget(clientsPageWidget);
  contentStack->addWidget(transactionsPageWidget);
  contentStack->addWidget(auctionsPageWidget);

  // Компоновка
  mainLayout->addWidget(sidebar);
  mainLayout->addWidget(contentStack, 1);

  setCentralWidget(centralWidget);

  // Подключение навигации
  connect(navigationList, &QListWidget::currentRowChanged, this,
          &MainWindow::onNavigationChanged);
  connect(navigationList, &QListWidget::currentRowChanged, contentStack,
          &QStackedWidget::setCurrentIndex);

  // Обновляем статистику и таблицы после полной инициализации UI
  QTimer::singleShot(100, this, [this]() {
    updateDashboardStats();
    refreshAuctions();
  });
}

// void MainWindow::setupDashboard()
// {
//     dashboardWidget = new QWidget;
//     QVBoxLayout* layout = new QVBoxLayout(dashboardWidget);
//     layout->setContentsMargins(30, 30, 30, 30);
//     layout->setSpacing(20);

//     // Заголовок
//     QLabel* header = new QLabel("Панель управления");
//     header->setStyleSheet("color: #ffffff; font-size: 24pt; font-weight:
//     bold; padding: 10px 0;"); layout->addWidget(header);

//     // Статистические карточки
//     QHBoxLayout* statsLayout = new QHBoxLayout;
//     statsLayout->setSpacing(20);

//     // Карточка недвижимости
//     QFrame* propCard = new QFrame;
//     propCard->setStyleSheet("background: qlineargradient(x1:0, y1:0, x2:1,
//     y2:1, stop:0 #2d5a3d, stop:1 #3d6a4d); border-radius: 15px; padding: 30px
//     25px;"); propCard->setFixedHeight(140); QHBoxLayout* propCardLayout = new
//     QHBoxLayout(propCard); propCardLayout->setContentsMargins(0, 0, 0, 0);
//     QVBoxLayout* propLeftLayout = new QVBoxLayout;
//     propLeftLayout->setContentsMargins(0, 0, 0, 0);
//     propLeftLayout->setSpacing(10);
//     QLabel* propLabel = new QLabel("Недвижимость");
//     propLabel->setStyleSheet("color: #ffffff; font-size: 13pt; font-weight:
//     600; letter-spacing: 0.5px;"); propLeftLayout->addWidget(propLabel);
//     statsPropertiesLabel = new QLabel("0");
//     statsPropertiesLabel->setStyleSheet("color: #ffffff; font-size: 42pt;
//     font-weight: bold;"); propLeftLayout->addWidget(statsPropertiesLabel);
//     propCardLayout->addLayout(propLeftLayout, 1);
//     statsLayout->addWidget(propCard, 1);

//     // Карточка клиентов
//     QFrame* clientCard = new QFrame;
//     clientCard->setStyleSheet("background: qlineargradient(x1:0, y1:0, x2:1,
//     y2:1, stop:0 #2d3d5a, stop:1 #3d4d6a); border-radius: 15px; padding: 30px
//     25px;"); clientCard->setFixedHeight(140); QHBoxLayout* clientCardLayout =
//     new QHBoxLayout(clientCard); clientCardLayout->setContentsMargins(0, 0,
//     0, 0); QVBoxLayout* clientLeftLayout = new QVBoxLayout;
//     clientLeftLayout->setContentsMargins(0, 0, 0, 0);
//     clientLeftLayout->setSpacing(10);
//     QLabel* clientLabel = new QLabel("Клиенты");
//     clientLabel->setStyleSheet("color: #ffffff; font-size: 13pt; font-weight:
//     600; letter-spacing: 0.5px;"); clientLeftLayout->addWidget(clientLabel);
//     statsClientsLabel = new QLabel("0");
//     statsClientsLabel->setStyleSheet("color: #ffffff; font-size: 42pt;
//     font-weight: bold;"); clientLeftLayout->addWidget(statsClientsLabel);
//     clientCardLayout->addLayout(clientLeftLayout, 1);
//     statsLayout->addWidget(clientCard, 1);

//     // Карточка сделок
//     QFrame* transCard = new QFrame;
//     transCard->setStyleSheet("background: qlineargradient(x1:0, y1:0, x2:1,
//     y2:1, stop:0 #5a4d2d, stop:1 #6a5d3d); border-radius: 15px; padding: 30px
//     25px;"); transCard->setFixedHeight(140); QHBoxLayout* transCardLayout =
//     new QHBoxLayout(transCard); transCardLayout->setContentsMargins(0, 0, 0,
//     0); QVBoxLayout* transLeftLayout = new QVBoxLayout;
//     transLeftLayout->setContentsMargins(0, 0, 0, 0);
//     transLeftLayout->setSpacing(10);
//     QLabel* transLabel = new QLabel("Сделки");
//     transLabel->setStyleSheet("color: #ffffff; font-size: 13pt; font-weight:
//     600; letter-spacing: 0.5px;"); transLeftLayout->addWidget(transLabel);
//     statsTransactionsLabel = new QLabel("0");
//     statsTransactionsLabel->setStyleSheet("color: #ffffff; font-size: 42pt;
//     font-weight: bold;"); transLeftLayout->addWidget(statsTransactionsLabel);
//     transCardLayout->addLayout(transLeftLayout, 1);
//     statsLayout->addWidget(transCard, 1);

//     // Карточка доступной недвижимости
//     QFrame* availCard = new QFrame;
//     availCard->setStyleSheet("background: qlineargradient(x1:0, y1:0, x2:1,
//     y2:1, stop:0 #4d5a2d, stop:1 #5d6a3d); border-radius: 15px; padding: 30px
//     25px;"); availCard->setFixedHeight(140); QHBoxLayout* availCardLayout =
//     new QHBoxLayout(availCard); availCardLayout->setContentsMargins(0, 0, 0,
//     0); QVBoxLayout* availLeftLayout = new QVBoxLayout;
//     availLeftLayout->setContentsMargins(0, 0, 0, 0);
//     availLeftLayout->setSpacing(10);
//     QLabel* availLabel = new QLabel("Доступно");
//     availLabel->setStyleSheet("color: #ffffff; font-size: 13pt; font-weight:
//     600; letter-spacing: 0.5px;"); availLeftLayout->addWidget(availLabel);
//     statsAvailableLabel = new QLabel("0");
//     statsAvailableLabel->setStyleSheet("color: #ffffff; font-size: 42pt;
//     font-weight: bold;"); availLeftLayout->addWidget(statsAvailableLabel);
//     availCardLayout->addLayout(availLeftLayout, 1);
//     statsLayout->addWidget(availCard, 1);

//     layout->addLayout(statsLayout);
//     layout->addStretch();
// }

void MainWindow::setupDashboard() {
  dashboardWidget = new QWidget;
  QVBoxLayout *mainLayout = new QVBoxLayout(dashboardWidget);
  mainLayout->setContentsMargins(50, 50, 50, 50);
  mainLayout->setSpacing(40);

  // Минималистичный заголовок
  QLabel *header = new QLabel("Панель управления");
  header->setStyleSheet("color: #ffffff; "
                        "font-size: 48pt; "
                        "font-weight: 300; "
                        "letter-spacing: 3px; "
                        "margin-bottom: 10px;");
  mainLayout->addWidget(header);

  // Создаем статистические виджеты в новом стиле
  auto createMetricWidget = [](const QString &label, const QString &color,
                               QLabel **valuePtr) -> QWidget * {
    QWidget *widget = new QWidget;
    widget->setFixedHeight(160);

    QVBoxLayout *layout = new QVBoxLayout(widget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(12);

    // Заголовок метрики
    QLabel *labelWidget = new QLabel(label);
    labelWidget->setStyleSheet(QString("color: %1; "
                                       "font-size: 11pt; "
                                       "font-weight: 500; "
                                       "letter-spacing: 2px;")
                                   .arg(color));
    layout->addWidget(labelWidget);

    // Значение
    QLabel *valueLabel = new QLabel("0");
    valueLabel->setStyleSheet(QString("color: %1; "
                                      "font-size: 50pt; "
                                      "font-weight: 700; "
                                      "background: transparent;")
                                  .arg(color));
    valueLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    valueLabel->setMinimumHeight(90);
    valueLabel->setMinimumWidth(120);
    layout->addWidget(valueLabel);
    layout->addStretch();

    if (valuePtr) {
      *valuePtr = valueLabel;
    }

    return widget;
  };

  // Создаем метрики
  QWidget *propMetric =
      createMetricWidget("Недвижимость", "#00D4AA", &statsPropertiesLabel);
  QWidget *clientMetric =
      createMetricWidget("Клиенты", "#5B9FFF", &statsClientsLabel);
  QWidget *transMetric =
      createMetricWidget("Сделки", "#FFB020", &statsTransactionsLabel);
  QWidget *availMetric =
      createMetricWidget("Доступно", "#B794F6", &statsAvailableLabel);
  QWidget *auctionMetric =
      createMetricWidget("Аукционы", "#FF6B6B", &statsAuctionsLabel);

  // Горизонтальная компоновка метрик
  QHBoxLayout *metricsLayout = new QHBoxLayout;
  metricsLayout->setSpacing(60);
  metricsLayout->setContentsMargins(0, 0, 0, 0);

  metricsLayout->addWidget(propMetric);
  metricsLayout->addWidget(clientMetric);
  metricsLayout->addWidget(transMetric);
  metricsLayout->addWidget(availMetric);
  metricsLayout->addWidget(auctionMetric);
  metricsLayout->addStretch();

  mainLayout->addLayout(metricsLayout);

  // Разделительная линия
  QFrame *divider = new QFrame;
  divider->setFixedHeight(1);
  divider->setStyleSheet("background-color: #333333;");
  mainLayout->addWidget(divider);

  // Блок действий
  QWidget *actionsWidget = new QWidget;
  QVBoxLayout *actionsLayout = new QVBoxLayout(actionsWidget);
  actionsLayout->setContentsMargins(0, 0, 0, 0);
  actionsLayout->setSpacing(25);

  QLabel *actionsTitle = new QLabel("Действия");
  actionsTitle->setStyleSheet("color: #888888; "
                              "font-size: 11pt; "
                              "font-weight: 500; "
                              "letter-spacing: 2px;");
  actionsLayout->addWidget(actionsTitle);

  QHBoxLayout *buttonsLayout = new QHBoxLayout;
  buttonsLayout->setSpacing(20);
  buttonsLayout->setContentsMargins(0, 0, 0, 0);

  // Стильные кнопки
  auto createModernButton = [](const QString &text,
                               const QString &color) -> QPushButton * {
    QPushButton *btn = new QPushButton(text);
    btn->setStyleSheet(QString("QPushButton {"
                               "    background-color: transparent;"
                               "    color: %1;"
                               "    border: 1px solid %1;"
                               "    border-radius: 0px;"
                               "    padding: 14px 40px;"
                               "    font-size: 12pt;"
                               "    font-weight: 400;"
                               "    letter-spacing: 1px;"
                               "    min-width: 180px;"
                               "}"
                               "QPushButton:hover {"
                               "    background-color: %1;"
                               "    color: #1e1e1e;"
                               "}"
                               "QPushButton:pressed {"
                               "    background-color: %2;"
                               "}")
                           .arg(color, color));
    return btn;
  };

  QPushButton *saveBtn = createModernButton("Сохранить", "#00D4AA");
  QPushButton *loadBtn = createModernButton("Загрузить", "#5B9FFF");
  QPushButton *refreshBtn = createModernButton("Обновить", "#FFB020");

  connect(saveBtn, &QPushButton::clicked, this, &MainWindow::saveAllData);
  connect(loadBtn, &QPushButton::clicked, this, &MainWindow::loadAllData);
  connect(refreshBtn, &QPushButton::clicked, this, [this]() {
    refreshProperties();
    refreshClients();
    refreshTransactions();
    refreshAuctions();
    updateDashboardStats();
    statusBar()->showMessage("Все данные обновлены", 2000);
  });

  buttonsLayout->addWidget(saveBtn);
  buttonsLayout->addWidget(loadBtn);
  buttonsLayout->addWidget(refreshBtn);
  buttonsLayout->addStretch();

  actionsLayout->addLayout(buttonsLayout);
  mainLayout->addWidget(actionsWidget);
  mainLayout->addStretch();
}

void MainWindow::setupPropertiesPage() {
  propertiesPageWidget = new QWidget;
  QVBoxLayout *layout = new QVBoxLayout(propertiesPageWidget);
  layout->setContentsMargins(20, 20, 20, 20);
  layout->setSpacing(15);

  // Заголовок с кнопками действий
  QHBoxLayout *headerLayout = new QHBoxLayout;
  QLabel *title = new QLabel("Недвижимость");
  title->setStyleSheet("color: #ffffff; font-size: 22pt; font-weight: bold;");
  headerLayout->addWidget(title);
  headerLayout->addStretch();

  // Поиск
  searchPropertyEdit = new QLineEdit;
  searchPropertyEdit->setPlaceholderText("🔍 Поиск по ID или адресу");
  searchPropertyEdit->setFixedWidth(300);
  searchPropertyBtn = new QPushButton("Найти");
  headerLayout->addWidget(searchPropertyEdit);
  headerLayout->addWidget(searchPropertyBtn);

  addPropertyBtn = new QPushButton("➕ Добавить");
  refreshPropertyBtn = new QPushButton("🔄 Обновить");
  headerLayout->addWidget(addPropertyBtn);
  headerLayout->addWidget(refreshPropertyBtn);
  layout->addLayout(headerLayout);

  // Область с таблицей и деталями
  QSplitter *splitter = new QSplitter(Qt::Horizontal);

  propertiesTable = new QTableWidget;
  propertiesTable->setColumnCount(7);
  propertiesTable->setHorizontalHeaderLabels(
      {"ID", "Тип", "Адрес", "Цена", "Площадь", "Доступность", "Действия"});
  propertiesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
  propertiesTable->setSelectionMode(QAbstractItemView::SingleSelection);
  propertiesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
  propertiesTable->verticalHeader()->setDefaultSectionSize(60);
  propertiesTable->setColumnWidth(0, 100);
  propertiesTable->setColumnWidth(1, 120);
  propertiesTable->setColumnWidth(2, 300);
  propertiesTable->setColumnWidth(3, 150);
  propertiesTable->setColumnWidth(4, 100);
  propertiesTable->setColumnWidth(5, 120);
  propertiesTable->setColumnWidth(6, 300);
  propertiesTable->horizontalHeader()->setStretchLastSection(false);

  QFrame *detailsFrame = new QFrame;
  detailsFrame->setFixedWidth(400);
  detailsFrame->setStyleSheet(
      "background-color: #252525; border-radius: 8px; padding: 15px;");
  QVBoxLayout *detailsLayout = new QVBoxLayout(detailsFrame);
  QLabel *detailsTitle = new QLabel("Информация");
  detailsTitle->setStyleSheet("color: #ffffff; font-size: 14pt; font-weight: "
                              "bold; padding-bottom: 10px;");
  detailsLayout->addWidget(detailsTitle);
  propertyDetailsText = new QTextEdit;
  propertyDetailsText->setReadOnly(true);
  propertyDetailsText->setStyleSheet(
      "background-color: #2d2d2d; color: #d0d0d0; border: none;");
  detailsLayout->addWidget(propertyDetailsText);

  splitter->addWidget(propertiesTable);
  splitter->addWidget(detailsFrame);
  splitter->setStretchFactor(0, 2);
  splitter->setStretchFactor(1, 1);

  layout->addWidget(splitter);

  connect(addPropertyBtn, &QPushButton::clicked, this,
          &MainWindow::addProperty);
  connect(refreshPropertyBtn, &QPushButton::clicked, this,
          &MainWindow::refreshProperties);
  connect(searchPropertyBtn, &QPushButton::clicked, this,
          &MainWindow::searchProperties);
  connect(propertiesTable, &QTableWidget::itemSelectionChanged, this,
          &MainWindow::propertySelectionChanged);
}

void MainWindow::setupClientsPage() {
  clientsPageWidget = new QWidget;
  QVBoxLayout *layout = new QVBoxLayout(clientsPageWidget);
  layout->setContentsMargins(20, 20, 20, 20);
  layout->setSpacing(15);

  // Заголовок с кнопками
  QHBoxLayout *headerLayout = new QHBoxLayout;
  QLabel *title = new QLabel("Клиенты");
  title->setStyleSheet("color: #ffffff; font-size: 22pt; font-weight: bold;");
  headerLayout->addWidget(title);
  headerLayout->addStretch();

  // Поиск
  searchClientEdit = new QLineEdit;
  searchClientEdit->setPlaceholderText("🔍 Поиск по ID или имени");
  searchClientEdit->setFixedWidth(300);
  searchClientBtn = new QPushButton("Найти");
  headerLayout->addWidget(searchClientEdit);
  headerLayout->addWidget(searchClientBtn);

  addClientBtn = new QPushButton("➕ Добавить");
  refreshClientBtn = new QPushButton("🔄 Обновить");
  headerLayout->addWidget(addClientBtn);
  headerLayout->addWidget(refreshClientBtn);
  layout->addLayout(headerLayout);

  QSplitter *splitter = new QSplitter(Qt::Horizontal);

  clientsTable = new QTableWidget;
  clientsTable->setColumnCount(6);
  clientsTable->setHorizontalHeaderLabels(
      {"ID", "Имя", "Телефон", "Email", "Дата регистрации", "Действия"});
  clientsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
  clientsTable->setSelectionMode(QAbstractItemView::SingleSelection);
  clientsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
  clientsTable->setAlternatingRowColors(true);
  clientsTable->verticalHeader()->setDefaultSectionSize(60);
  clientsTable->setColumnWidth(0, 100);
  clientsTable->setColumnWidth(1, 200);
  clientsTable->setColumnWidth(2, 150);
  clientsTable->setColumnWidth(3, 250);
  clientsTable->setColumnWidth(4, 150);
  clientsTable->setColumnWidth(5, 300);
  clientsTable->horizontalHeader()->setStretchLastSection(false);

  QFrame *detailsFrame = new QFrame;
  detailsFrame->setFixedWidth(400);
  detailsFrame->setStyleSheet(
      "background-color: #252525; border-radius: 8px; padding: 15px;");
  QVBoxLayout *detailsLayout = new QVBoxLayout(detailsFrame);
  QLabel *detailsTitle = new QLabel("Информация о клиенте");
  detailsTitle->setStyleSheet("color: #ffffff; font-size: 14pt; font-weight: "
                              "bold; padding-bottom: 10px;");
  detailsLayout->addWidget(detailsTitle);
  clientDetailsText = new QTextEdit;
  clientDetailsText->setReadOnly(true);
  clientDetailsText->setStyleSheet(
      "background-color: #2d2d2d; color: #d0d0d0; border: none;");
  detailsLayout->addWidget(clientDetailsText);

  splitter->addWidget(clientsTable);
  splitter->addWidget(detailsFrame);
  splitter->setStretchFactor(0, 2);
  splitter->setStretchFactor(1, 1);

  layout->addWidget(splitter);

  connect(addClientBtn, &QPushButton::clicked, this, &MainWindow::addClient);
  connect(refreshClientBtn, &QPushButton::clicked, this,
          &MainWindow::refreshClients);
  connect(searchClientBtn, &QPushButton::clicked, this,
          &MainWindow::searchClients);
  connect(clientsTable, &QTableWidget::itemSelectionChanged, this, [this]() {
    int row = clientsTable->currentRow();
    if (row >= 0) {
      QString id = clientsTable->item(row, 0)->text();
      Client *client = agency->getClientManager().findClient(id.toStdString());
      if (client) {
        showClientDetails(client);
        showClientTransactions(id.toStdString());
      }
    }
  });
}

void MainWindow::setupTransactionsPage() {
  transactionsPageWidget = new QWidget;
  QVBoxLayout *layout = new QVBoxLayout(transactionsPageWidget);
  layout->setContentsMargins(20, 20, 20, 20);
  layout->setSpacing(15);

  // Заголовок
  QHBoxLayout *headerLayout = new QHBoxLayout;
  QLabel *title = new QLabel("Сделки");
  title->setStyleSheet("color: #ffffff; font-size: 22pt; font-weight: bold;");
  headerLayout->addWidget(title);
  headerLayout->addStretch();

  // Поиск
  searchTransactionEdit = new QLineEdit;
  searchTransactionEdit->setPlaceholderText("🔍 Поиск по ID");
  searchTransactionEdit->setFixedWidth(250);
  searchTransactionBtn = new QPushButton("Найти");
  headerLayout->addWidget(searchTransactionEdit);
  headerLayout->addWidget(searchTransactionBtn);

  addTransactionBtn = new QPushButton("➕ Добавить");
  refreshTransactionBtn = new QPushButton("🔄 Обновить");
  headerLayout->addWidget(addTransactionBtn);
  headerLayout->addWidget(refreshTransactionBtn);
  layout->addLayout(headerLayout);

  QSplitter *splitter = new QSplitter(Qt::Horizontal);

  transactionsTable = new QTableWidget;
  transactionsTable->setColumnCount(7);
  transactionsTable->setHorizontalHeaderLabels(
      {"ID", "Недвижимость", "Клиент", "Цена", "Дата", "Статус", "Действия"});
  transactionsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
  transactionsTable->setSelectionMode(QAbstractItemView::SingleSelection);
  transactionsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
  transactionsTable->setAlternatingRowColors(true);
  transactionsTable->verticalHeader()->setDefaultSectionSize(60);
  transactionsTable->setColumnWidth(0, 100);
  transactionsTable->setColumnWidth(1, 250);
  transactionsTable->setColumnWidth(2, 200);
  transactionsTable->setColumnWidth(3, 150);
  transactionsTable->setColumnWidth(4, 120);
  transactionsTable->setColumnWidth(5, 120);
  transactionsTable->setColumnWidth(6, 300);
  transactionsTable->horizontalHeader()->setStretchLastSection(false);

  QFrame *detailsFrame = new QFrame;
  detailsFrame->setFixedWidth(400);
  detailsFrame->setStyleSheet(
      "background-color: #252525; border-radius: 8px; padding: 15px;");
  QVBoxLayout *detailsLayout = new QVBoxLayout(detailsFrame);
  QLabel *detailsTitle = new QLabel("Детали сделки");
  detailsTitle->setStyleSheet("color: #ffffff; font-size: 14pt; font-weight: "
                              "bold; padding-bottom: 10px;");
  detailsLayout->addWidget(detailsTitle);
  transactionDetailsText = new QTextEdit;
  transactionDetailsText->setReadOnly(true);
  transactionDetailsText->setStyleSheet(
      "background-color: #2d2d2d; color: #d0d0d0; border: none;");
  detailsLayout->addWidget(transactionDetailsText);

  splitter->addWidget(transactionsTable);
  splitter->addWidget(detailsFrame);
  splitter->setStretchFactor(0, 2);
  splitter->setStretchFactor(1, 1);

  layout->addWidget(splitter);

  connect(addTransactionBtn, &QPushButton::clicked, this,
          &MainWindow::addTransaction);
  connect(refreshTransactionBtn, &QPushButton::clicked, this,
          &MainWindow::refreshTransactions);
  connect(searchTransactionBtn, &QPushButton::clicked, this,
          &MainWindow::searchTransactions);
  connect(
      transactionsTable, &QTableWidget::itemSelectionChanged, this, [this]() {
        int row = transactionsTable->currentRow();
        if (row >= 0) {
          QString id = transactionsTable->item(row, 0)->text();
          Transaction *trans =
              agency->getTransactionManager().findTransaction(id.toStdString());
          if (trans) {
            showTransactionDetails(trans);
          }
        }
      });
}

void MainWindow::updateDashboardStats() {
  // Проверяем, что все необходимые элементы инициализированы
  if (!agency) {
    return;
  }

  // Проверяем каждый label отдельно перед использованием
  if (statsPropertiesLabel) {
    try {
      int propCount = agency->getPropertyManager().getCount();
      statsPropertiesLabel->setText(QString::number(propCount));
    } catch (...) {
    }
  }

  if (statsClientsLabel) {
    try {
      int clientCount = agency->getClientManager().getCount();
      statsClientsLabel->setText(QString::number(clientCount));
    } catch (...) {
    }
  }

  if (statsTransactionsLabel) {
    try {
      int transCount = agency->getTransactionManager().getCount();
      statsTransactionsLabel->setText(QString::number(transCount));
    } catch (...) {
    }
  }

  if (statsAvailableLabel) {
    try {
      auto availableProps =
          agency->getPropertyManager().getAvailableProperties();
      int availCount = availableProps.size();
      statsAvailableLabel->setText(QString::number(availCount));
    } catch (...) {
    }
  }

  if (statsAuctionsLabel) {
    try {
      int auctionCount = agency->getAuctionManager().getCount();
      statsAuctionsLabel->setText(QString::number(auctionCount));
    } catch (...) {
    }
  }
}

// Вспомогательные функции
void MainWindow::showStatusMessage(const QString &message, int timeout) {
  if (statusBar()) {
    statusBar()->showMessage(message, timeout);
  }
}

QWidget *MainWindow::createActionButtons(QTableWidget *table, const QString &id,
                                         std::function<void()> editAction,
                                         std::function<void()> deleteAction) {
  QWidget *actionsWidget = new QWidget;
  QHBoxLayout *actionsLayout = new QHBoxLayout(actionsWidget);
  actionsLayout->setContentsMargins(5, 5, 5, 5);
  actionsLayout->setSpacing(8);

  QPushButton *editBtn = new QPushButton("Редактировать");
  editBtn->setMinimumWidth(110);
  editBtn->setFixedHeight(35);
  QPushButton *deleteBtn = new QPushButton("Удалить");
  deleteBtn->setMinimumWidth(90);
  deleteBtn->setFixedHeight(35);

  connect(editBtn, &QPushButton::clicked, this,
          [this, table, id, editAction]() {
            selectRowById(table, id);
            editAction();
          });
  connect(deleteBtn, &QPushButton::clicked, this,
          [this, table, id, deleteAction]() {
            selectRowById(table, id);
            deleteAction();
          });

  actionsLayout->addWidget(editBtn);
  actionsLayout->addWidget(deleteBtn);
  actionsLayout->addStretch();

  return actionsWidget;
}

QWidget *MainWindow::createActionButtons(QTableWidget *table, const QString &id,
                                         std::function<void()> viewAction,
                                         std::function<void()> deleteAction,
                                         bool isView) {
  QWidget *actionsWidget = new QWidget;
  QHBoxLayout *actionsLayout = new QHBoxLayout(actionsWidget);
  actionsLayout->setContentsMargins(5, 5, 5, 5);
  actionsLayout->setSpacing(8);

  QPushButton *viewBtn = new QPushButton("Просмотр");
  viewBtn->setMinimumWidth(100);
  viewBtn->setFixedHeight(35);
  QPushButton *deleteBtn = new QPushButton("Удалить");
  deleteBtn->setMinimumWidth(90);
  deleteBtn->setFixedHeight(35);

  connect(viewBtn, &QPushButton::clicked, this,
          [this, table, id, viewAction]() {
            selectRowById(table, id);
            viewAction();
          });
  connect(deleteBtn, &QPushButton::clicked, this,
          [this, table, id, deleteAction]() {
            selectRowById(table, id);
            deleteAction();
          });

  actionsLayout->addWidget(viewBtn);
  actionsLayout->addWidget(deleteBtn);
  actionsLayout->addStretch();

  return actionsWidget;
}

void MainWindow::selectRowById(QTableWidget *table, const QString &id) {
  if (!table)
    return;
  for (int i = 0; i < table->rowCount(); ++i) {
    if (table->item(i, 0) && table->item(i, 0)->text() == id) {
      table->selectRow(i);
      break;
    }
  }
}

QString MainWindow::getSelectedIdFromTable(QTableWidget *table) {
  return TableHelper::getSelectedId(table);
}

bool MainWindow::checkTableSelection(QTableWidget *table,
                                     const QString &errorMessage) {
  if (!table || !TableHelper::hasValidSelection(table)) {
    if (!errorMessage.isEmpty()) {
      QMessageBox::information(this, Constants::Messages::INFORMATION,
                               errorMessage);
    }
    return false;
  }
  return true;
}

void MainWindow::refreshAllData() {
  refreshProperties();
  refreshClients();
  refreshTransactions();
  refreshAuctions();
  updateDashboardStats();
}

bool MainWindow::hasActiveTransactions(const std::string &propertyId) {
  auto transactions =
      agency->getTransactionManager().getTransactionsByProperty(propertyId);
  for (Transaction *t : transactions) {
    if (t && (t->getStatus() == Constants::TransactionStatus::PENDING ||
              t->getStatus() == Constants::TransactionStatus::COMPLETED)) {
      return true;
    }
  }
  return false;
}

void MainWindow::onNavigationChanged(int index) {
  // Обновляем статистику при переходе на dashboard
  if (index == 0) {
    updateDashboardStats();
  }
}

void MainWindow::setupPropertiesTab() {
  QWidget *propertiesWidget = new QWidget;
  QVBoxLayout *mainLayout = new QVBoxLayout(propertiesWidget);

  QHBoxLayout *buttonLayout = new QHBoxLayout;
  addPropertyBtn = new QPushButton("Добавить");
  refreshPropertyBtn = new QPushButton("Обновить");

  buttonLayout->addWidget(addPropertyBtn);
  buttonLayout->addWidget(refreshPropertyBtn);
  buttonLayout->addStretch();

  // Поиск
  QHBoxLayout *searchLayout = new QHBoxLayout;
  searchLayout->addWidget(new QLabel("Поиск (ID или адрес):"));
  searchPropertyEdit = new QLineEdit;
  searchPropertyEdit->setPlaceholderText("ID (6-8 цифр) или адрес");
  searchPropertyBtn = new QPushButton("Найти");
  searchLayout->addWidget(searchPropertyEdit);
  searchLayout->addWidget(searchPropertyBtn);

  // Таблица
  propertiesTable = new QTableWidget;
  propertiesTable->setColumnCount(6);
  propertiesTable->setHorizontalHeaderLabels(
      {"ID", "Тип", "Адрес", "Цена", "Площадь", "Доступность"});
  propertiesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
  propertiesTable->setSelectionMode(QAbstractItemView::SingleSelection);
  propertiesTable->horizontalHeader()->setStretchLastSection(true);
  propertiesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

  // Детали
  propertyDetailsText = new QTextEdit;
  propertyDetailsText->setReadOnly(true);
  propertyDetailsText->setMaximumHeight(200);

  QSplitter *splitter = new QSplitter(Qt::Vertical);
  splitter->addWidget(propertiesTable);
  splitter->addWidget(propertyDetailsText);

  mainLayout->addLayout(buttonLayout);
  mainLayout->addLayout(searchLayout);
  mainLayout->addWidget(splitter);

  // tabWidget больше не используется в новом интерфейсе

  connect(addPropertyBtn, &QPushButton::clicked, this,
          &MainWindow::addProperty);
  connect(refreshPropertyBtn, &QPushButton::clicked, this,
          &MainWindow::refreshProperties);
  connect(searchPropertyBtn, &QPushButton::clicked, this,
          &MainWindow::searchProperties);
  connect(propertiesTable, &QTableWidget::itemSelectionChanged, this,
          &MainWindow::propertySelectionChanged);
}

void MainWindow::setupClientsTab() {
  QWidget *clientsWidget = new QWidget;
  QVBoxLayout *mainLayout = new QVBoxLayout(clientsWidget);

  QHBoxLayout *buttonLayout = new QHBoxLayout;
  addClientBtn = new QPushButton("Добавить");
  refreshClientBtn = new QPushButton("Обновить");

  buttonLayout->addWidget(addClientBtn);
  buttonLayout->addWidget(refreshClientBtn);
  buttonLayout->addStretch();

  QHBoxLayout *searchLayout = new QHBoxLayout;
  searchLayout->addWidget(new QLabel("Поиск (ID или имя):"));
  searchClientEdit = new QLineEdit;
  searchClientEdit->setPlaceholderText("ID (6-8 цифр) или имя");
  searchClientBtn = new QPushButton("Найти");
  searchLayout->addWidget(searchClientEdit);
  searchLayout->addWidget(searchClientBtn);

  clientsTable = new QTableWidget;
  clientsTable->setColumnCount(5);
  clientsTable->setHorizontalHeaderLabels(
      {"ID", "Имя", "Телефон", "Email", "Дата регистрации"});
  clientsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
  clientsTable->setSelectionMode(QAbstractItemView::SingleSelection);
  clientsTable->horizontalHeader()->setStretchLastSection(true);
  clientsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
  clientsTable->setAlternatingRowColors(true);

  // Детали клиента и связанные сделки
  clientDetailsText = new QTextEdit;
  clientDetailsText->setReadOnly(true);
  clientDetailsText->setMaximumHeight(200);

  QSplitter *clientSplitter = new QSplitter(Qt::Vertical);
  clientSplitter->addWidget(clientsTable);
  clientSplitter->addWidget(clientDetailsText);

  mainLayout->addLayout(buttonLayout);
  mainLayout->addLayout(searchLayout);
  mainLayout->addWidget(clientSplitter);

  // tabWidget больше не используется в новом интерфейсе

  connect(addClientBtn, &QPushButton::clicked, this, &MainWindow::addClient);
  connect(refreshClientBtn, &QPushButton::clicked, this,
          &MainWindow::refreshClients);
  connect(searchClientBtn, &QPushButton::clicked, this,
          &MainWindow::searchClients);
  connect(clientsTable, &QTableWidget::itemSelectionChanged, this, [this]() {
    int row = clientsTable->currentRow();
    if (row >= 0) {
      QString id = clientsTable->item(row, 0)->text();
      Client *client = agency->getClientManager().findClient(id.toStdString());
      if (client) {
        showClientDetails(client);
        showClientTransactions(id.toStdString());
      }
    }
  });
}

void MainWindow::setupTransactionsTab() {
  QWidget *transactionsWidget = new QWidget;
  QVBoxLayout *mainLayout = new QVBoxLayout(transactionsWidget);

  QHBoxLayout *buttonLayout = new QHBoxLayout;
  addTransactionBtn = new QPushButton("Добавить");
  refreshTransactionBtn = new QPushButton("Обновить");

  buttonLayout->addWidget(addTransactionBtn);
  buttonLayout->addWidget(refreshTransactionBtn);
  buttonLayout->addStretch();

  // Поиск
  QHBoxLayout *searchLayout = new QHBoxLayout;
  searchLayout->addWidget(new QLabel("Поиск по ID:"));
  searchTransactionEdit = new QLineEdit;
  searchTransactionEdit->setPlaceholderText("ID сделки (6-8 цифр)");
  searchTransactionBtn = new QPushButton("Найти");
  searchLayout->addWidget(searchTransactionEdit);
  searchLayout->addWidget(searchTransactionBtn);

  transactionsTable = new QTableWidget;
  transactionsTable->setColumnCount(6);
  transactionsTable->setHorizontalHeaderLabels(
      {"ID", "Недвижимость", "Клиент", "Цена", "Дата", "Статус"});
  transactionsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
  transactionsTable->setSelectionMode(QAbstractItemView::SingleSelection);
  transactionsTable->horizontalHeader()->setStretchLastSection(true);
  transactionsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
  transactionsTable->setAlternatingRowColors(true);

  // Детали сделки
  transactionDetailsText = new QTextEdit;
  transactionDetailsText->setReadOnly(true);
  transactionDetailsText->setMaximumHeight(250);

  QSplitter *transSplitter = new QSplitter(Qt::Vertical);
  transSplitter->addWidget(transactionsTable);
  transSplitter->addWidget(transactionDetailsText);

  mainLayout->addLayout(buttonLayout);
  mainLayout->addLayout(searchLayout);
  mainLayout->addWidget(transSplitter);

  // tabWidget больше не используется в новом интерфейсе

  connect(addTransactionBtn, &QPushButton::clicked, this,
          &MainWindow::addTransaction);
  connect(refreshTransactionBtn, &QPushButton::clicked, this,
          &MainWindow::refreshTransactions);
  connect(searchTransactionBtn, &QPushButton::clicked, this,
          &MainWindow::searchTransactions);
  connect(
      transactionsTable, &QTableWidget::itemSelectionChanged, this, [this]() {
        int row = transactionsTable->currentRow();
        if (row >= 0) {
          QString id = transactionsTable->item(row, 0)->text();
          Transaction *trans =
              agency->getTransactionManager().findTransaction(id.toStdString());
          if (trans) {
            showTransactionDetails(trans);
          }
        }
      });
}

void MainWindow::updatePropertiesTable() {
  if (!propertiesTable) {
    return;
  }

  TableHelper::clearTable(propertiesTable);
  auto properties = agency->getPropertyManager().getAllProperties();

  for (Property *prop : properties) {
    if (!prop)
      continue;

    int row = propertiesTable->rowCount();
    propertiesTable->insertRow(row);

    propertiesTable->setItem(
        row, 0, new QTableWidgetItem(Utils::toQString(prop->getId())));
    propertiesTable->setItem(
        row, 1,
        new QTableWidgetItem(
            TableHelper::getPropertyTypeText(prop->getType())));
    propertiesTable->setItem(
        row, 2, new QTableWidgetItem(Utils::toQString(prop->getAddress())));
    propertiesTable->setItem(
        row, 3, new QTableWidgetItem(Utils::formatNumber(prop->getPrice())));
    propertiesTable->setItem(
        row, 4, new QTableWidgetItem(Utils::formatNumber(prop->getArea())));
    propertiesTable->setItem(
        row, 5, new QTableWidgetItem(prop->getIsAvailable() ? "Да" : "Нет"));

    QString propId = Utils::toQString(prop->getId());
    QWidget *actionsWidget = createActionButtons(
        propertiesTable, propId, [this]() { editProperty(); },
        [this]() { deleteProperty(); });
    propertiesTable->setCellWidget(row, 6, actionsWidget);
  }
}

void MainWindow::updateClientsTable() {
  if (!clientsTable) {
    return;
  }

  TableHelper::clearTable(clientsTable);
  auto clients = agency->getClientManager().getAllClients();

  for (Client *client : clients) {
    if (!client)
      continue;

    int row = clientsTable->rowCount();
    clientsTable->insertRow(row);

    clientsTable->setItem(
        row, 0, new QTableWidgetItem(Utils::toQString(client->getId())));
    clientsTable->setItem(
        row, 1, new QTableWidgetItem(Utils::toQString(client->getName())));
    clientsTable->setItem(
        row, 2, new QTableWidgetItem(Utils::toQString(client->getPhone())));
    clientsTable->setItem(
        row, 3, new QTableWidgetItem(Utils::toQString(client->getEmail())));
    clientsTable->setItem(
        row, 4,
        new QTableWidgetItem(Utils::toQString(client->getRegistrationDate())));

    QString clientId = Utils::toQString(client->getId());
    QWidget *actionsWidget = createActionButtons(
        clientsTable, clientId, [this]() { editClient(); },
        [this]() { deleteClient(); });
    clientsTable->setCellWidget(row, 5, actionsWidget);
  }
}

void MainWindow::updateTransactionsTable() {
  if (!transactionsTable) {
    return;
  }

  TableHelper::clearTable(transactionsTable);
  auto transactions = agency->getTransactionManager().getAllTransactions();

  for (Transaction *trans : transactions) {
    if (!trans)
      continue;

    int row = transactionsTable->rowCount();
    transactionsTable->insertRow(row);

    Property *prop =
        agency->getPropertyManager().findProperty(trans->getPropertyId());
    Client *client =
        agency->getClientManager().findClient(trans->getClientId());

    transactionsTable->setItem(
        row, 0, new QTableWidgetItem(Utils::toQString(trans->getId())));
    transactionsTable->setItem(
        row, 1,
        new QTableWidgetItem(prop ? Utils::toQString(prop->getAddress())
                                  : "Не указана"));
    transactionsTable->setItem(
        row, 2,
        new QTableWidgetItem(client ? Utils::toQString(client->getName())
                                    : "Не указан"));
    transactionsTable->setItem(
        row, 3,
        new QTableWidgetItem(Utils::formatNumber(trans->getFinalPrice())));
    transactionsTable->setItem(
        row, 4, new QTableWidgetItem(Utils::toQString(trans->getDate())));
    transactionsTable->setItem(
        row, 5,
        new QTableWidgetItem(
            TableHelper::getTransactionStatusText(trans->getStatus())));

    QString transId = Utils::toQString(trans->getId());
    QWidget *actionsWidget = createActionButtons(
        transactionsTable, transId, [this]() { editTransaction(); },
        [this]() { deleteTransaction(); });
    transactionsTable->setCellWidget(row, 6, actionsWidget);
  }
}

void MainWindow::addProperty() {
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
            Utils::toString(id), Utils::toString(city), Utils::toString(street),
            Utils::toString(house), price, area, Utils::toString(description),
            dialog.getRooms(), dialog.getFloor(), dialog.getHasBalcony(),
            dialog.getHasElevator());
      } else if (dialog.getPropertyType() == PropertyDialog::TypeHouse) {
        agency->getPropertyManager().addHouse(
            Utils::toString(id), Utils::toString(city), Utils::toString(street),
            Utils::toString(house), price, area, Utils::toString(description),
            dialog.getFloors(), dialog.getRooms(), dialog.getLandArea(),
            dialog.getHasGarage(), dialog.getHasGarden());
      } else {
        agency->getPropertyManager().addCommercialProperty(
            Utils::toString(id), Utils::toString(city), Utils::toString(street),
            Utils::toString(house), price, area, Utils::toString(description),
            Utils::toString(dialog.getBusinessType()), dialog.getHasParking(),
            dialog.getParkingSpaces(), dialog.getIsVisibleFromStreet());
      }

      refreshProperties();
      updateDashboardStats();
      showStatusMessage(Constants::OperationMessages::PROPERTY_ADDED);
    } catch (const std::exception &e) {
      QMessageBox::warning(
          this, Constants::Messages::ERROR,
          QString("%1: %2").arg(Constants::ErrorMessages::ADD_ERROR, e.what()));
    }
  }
}

void MainWindow::editProperty() {
  if (!checkTableSelection(
          propertiesTable,
          Constants::SelectionMessages::SELECT_PROPERTY_TO_EDIT)) {
    return;
  }

  QString id = getSelectedIdFromTable(propertiesTable);
  Property *prop =
      agency->getPropertyManager().findProperty(Utils::toString(id));
  if (!prop)
    return;

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
            Utils::toString(newId), Utils::toString(city),
            Utils::toString(street), Utils::toString(house), price, area,
            Utils::toString(description), dialog.getRooms(), dialog.getFloor(),
            dialog.getHasBalcony(), dialog.getHasElevator());
      } else if (dialog.getPropertyType() == PropertyDialog::TypeHouse) {
        agency->getPropertyManager().addHouse(
            Utils::toString(newId), Utils::toString(city),
            Utils::toString(street), Utils::toString(house), price, area,
            Utils::toString(description), dialog.getFloors(), dialog.getRooms(),
            dialog.getLandArea(), dialog.getHasGarage(), dialog.getHasGarden());
      } else {
        agency->getPropertyManager().addCommercialProperty(
            Utils::toString(newId), Utils::toString(city),
            Utils::toString(street), Utils::toString(house), price, area,
            Utils::toString(description),
            Utils::toString(dialog.getBusinessType()), dialog.getHasParking(),
            dialog.getParkingSpaces(), dialog.getIsVisibleFromStreet());
      }

      refreshProperties();
      updateDashboardStats();
      showStatusMessage(Constants::OperationMessages::PROPERTY_UPDATED);
    } catch (const std::exception &e) {
      QMessageBox::warning(this, Constants::Messages::ERROR,
                           QString("%1: %2").arg(
                               Constants::ErrorMessages::EDIT_ERROR, e.what()));
    }
  }
}

void MainWindow::deleteProperty() {
  if (!checkTableSelection(
          propertiesTable,
          Constants::SelectionMessages::SELECT_PROPERTY_TO_DELETE)) {
    return;
  }

  QString id = getSelectedIdFromTable(propertiesTable);
  int ret =
      QMessageBox::question(this, Constants::Messages::CONFIRMATION,
                            Constants::ConfirmationMessages::DELETE_PROPERTY,
                            QMessageBox::Yes | QMessageBox::No);

  if (ret == QMessageBox::Yes) {
    agency->getPropertyManager().removeProperty(Utils::toString(id));
    refreshProperties();
    updateDashboardStats();
    showStatusMessage(Constants::OperationMessages::PROPERTY_DELETED);
  }
}

void MainWindow::refreshProperties() {
  updatePropertiesTable();
  showStatusMessage(QString("Недвижимость: %1 объектов")
                        .arg(agency->getPropertyManager().getCount()),
                    Constants::MessageTimeout::SHORT);
}

void MainWindow::searchProperties() {
  if (!propertiesTable || !searchPropertyEdit) {
    return;
  }

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

  std::vector<Property *> properties;
  if (isNumericOnly && searchText.length() >= 6 && searchText.length() <= 8) {
    // Поиск по ID
    Property *prop =
        agency->getPropertyManager().findProperty(searchText.toStdString());
    if (prop) {
      properties.push_back(prop);
    }
  } else {
    // Поиск по адресу (город, улица, дом)
    properties = agency->getPropertyManager().searchByAddress(
        searchText.toStdString(), "", "");
  }

  for (Property *prop : properties) {
    int row = propertiesTable->rowCount();
    propertiesTable->insertRow(row);

    propertiesTable->setItem(
        row, 0, new QTableWidgetItem(QString::fromStdString(prop->getId())));
    propertiesTable->setItem(
        row, 1,
        new QTableWidgetItem(
            TableHelper::getPropertyTypeText(prop->getType())));
    propertiesTable->setItem(
        row, 2,
        new QTableWidgetItem(QString::fromStdString(prop->getAddress())));
    propertiesTable->setItem(
        row, 3,
        new QTableWidgetItem(QString::number(prop->getPrice(), 'f', 2)));
    propertiesTable->setItem(
        row, 4, new QTableWidgetItem(QString::number(prop->getArea(), 'f', 2)));
    propertiesTable->setItem(
        row, 5, new QTableWidgetItem(prop->getIsAvailable() ? "Да" : "Нет"));

    QString propId = QString::fromStdString(prop->getId());
    QWidget *actionsWidget = createActionButtons(
        propertiesTable, propId, [this]() { editProperty(); },
        [this]() { deleteProperty(); });
    propertiesTable->setCellWidget(row, 6, actionsWidget);
  }

  if (properties.empty()) {
    if (statusBar()) {
      statusBar()->showMessage("Ничего не найдено", 2000);
    }
  } else {
    if (statusBar()) {
      statusBar()->showMessage(
          QString("Найдено: %1 объектов").arg(properties.size()), 2000);
    }
  }
}

void MainWindow::propertySelectionChanged() {
  if (!propertiesTable) {
    return;
  }

  int row = propertiesTable->currentRow();
  if (row >= 0 && row < propertiesTable->rowCount()) {
    QTableWidgetItem *item = propertiesTable->item(row, 0);
    if (item) {
      QString id = item->text();
      Property *prop =
          agency->getPropertyManager().findProperty(id.toStdString());
      if (prop) {
        showPropertyDetails(prop);
        showPropertyTransactions(id.toStdString());
      }
    }
  }
}

void MainWindow::showPropertyDetails(Property *prop) {
  if (!prop)
    return;

  QString html;
  html += "<html><body style='font-family: Arial, sans-serif;'>";
  html += "<h2 style='font-weight: bold; margin-bottom: 15px;'>ИНФОРМАЦИЯ О "
          "НЕДВИЖИМОСТИ</h2>";

  html += "<p><b>ID:</b> " + QString::fromStdString(prop->getId()) + "</p>";
  html += "<p><b>Тип:</b> " +
          TableHelper::getPropertyTypeText(prop->getType()) + "</p>";
  html +=
      "<p><b>Город:</b> " + QString::fromStdString(prop->getCity()) + "</p>";
  html +=
      "<p><b>Улица:</b> " + QString::fromStdString(prop->getStreet()) + "</p>";
  html += "<p><b>Дом:</b> " + QString::fromStdString(prop->getHouse()) + "</p>";
  html +=
      "<p><b>Адрес:</b> " + QString::fromStdString(prop->getAddress()) + "</p>";
  html += "<p><b>Цена:</b> " + QString::number(prop->getPrice(), 'f', 2) +
          " руб.</p>";
  html += "<p><b>Площадь:</b> " + QString::number(prop->getArea(), 'f', 2) +
          " м²</p>";
  html += "<p><b>Доступность:</b> " +
          QString(prop->getIsAvailable() ? "Доступна" : "Недоступна") + "</p>";
  if (!prop->getDescription().empty()) {
    html += "<p><b>Описание:</b> " +
            QString::fromStdString(prop->getDescription()) + "</p>";
  }

  html += "<h3 style='font-weight: bold; margin-top: 20px; margin-bottom: "
          "10px;'>ДОПОЛНИТЕЛЬНЫЕ ХАРАКТЕРИСТИКИ</h3>";

  if (Apartment *apt = dynamic_cast<Apartment *>(prop)) {
    html += "<p><b>Комнат:</b> " + QString::number(apt->getRooms()) + "</p>";
    html += "<p><b>Этаж:</b> " + QString::number(apt->getFloor()) + "</p>";
    html += "<p><b>Балкон:</b> " +
            QString(apt->getHasBalcony() ? "Да" : "Нет") + "</p>";
    html += "<p><b>Лифт:</b> " + QString(apt->getHasElevator() ? "Да" : "Нет") +
            "</p>";
  } else if (House *house = dynamic_cast<House *>(prop)) {
    html += "<p><b>Этажей:</b> " + QString::number(house->getFloors()) + "</p>";
    html += "<p><b>Комнат:</b> " + QString::number(house->getRooms()) + "</p>";
    html += "<p><b>Участок:</b> " +
            QString::number(house->getLandArea(), 'f', 2) + " м²</p>";
    html += "<p><b>Гараж:</b> " +
            QString(house->getHasGarage() ? "Да" : "Нет") + "</p>";
    html += "<p><b>Сад:</b> " + QString(house->getHasGarden() ? "Да" : "Нет") +
            "</p>";
  } else if (CommercialProperty *comm =
                 dynamic_cast<CommercialProperty *>(prop)) {
    html += "<p><b>Тип бизнеса:</b> " +
            QString::fromStdString(comm->getBusinessType()) + "</p>";
    html += "<p><b>Парковка:</b> " +
            QString(comm->getHasParking() ? "Да" : "Нет") + "</p>";
    html += "<p><b>Мест парковки:</b> " +
            QString::number(comm->getParkingSpaces()) + "</p>";
    html += "<p><b>Видимость с улицы:</b> " +
            QString(comm->getIsVisibleFromStreet() ? "Да" : "Нет") + "</p>";
  }

  html += "</body></html>";
  propertyDetailsText->setHtml(html);
}

void MainWindow::addClient() {
  ClientDialog dialog(this);
  if (dialog.exec() == QDialog::Accepted) {
    try {
      auto client = std::make_shared<Client>(
          Utils::toString(dialog.getId()), Utils::toString(dialog.getName()),
          Utils::toString(dialog.getPhone()),
          Utils::toString(dialog.getEmail()));
      agency->getClientManager().addClient(client);
      refreshClients();
      updateDashboardStats();
      showStatusMessage(Constants::OperationMessages::CLIENT_ADDED);
    } catch (const std::exception &e) {
      QMessageBox::warning(
          this, Constants::Messages::ERROR,
          QString("%1: %2").arg(Constants::ErrorMessages::ADD_ERROR, e.what()));
    }
  }
}

void MainWindow::editClient() {
  if (!checkTableSelection(
          clientsTable, Constants::SelectionMessages::SELECT_CLIENT_TO_EDIT)) {
    return;
  }

  QString id = getSelectedIdFromTable(clientsTable);
  Client *client = agency->getClientManager().findClient(Utils::toString(id));
  if (!client)
    return;

  ClientDialog dialog(this, client);
  if (dialog.exec() == QDialog::Accepted) {
    try {
      agency->getClientManager().removeClient(Utils::toString(id));
      auto newClient = std::make_shared<Client>(
          Utils::toString(dialog.getId()), Utils::toString(dialog.getName()),
          Utils::toString(dialog.getPhone()),
          Utils::toString(dialog.getEmail()));
      agency->getClientManager().addClient(newClient);
      refreshClients();
      updateDashboardStats();
      showStatusMessage(Constants::OperationMessages::CLIENT_UPDATED);
    } catch (const std::exception &e) {
      QMessageBox::warning(this, Constants::Messages::ERROR,
                           QString("%1: %2").arg(
                               Constants::ErrorMessages::EDIT_ERROR, e.what()));
    }
  }
}

void MainWindow::deleteClient() {
  if (!checkTableSelection(
          clientsTable,
          Constants::SelectionMessages::SELECT_CLIENT_TO_DELETE)) {
    return;
  }

  QString id = getSelectedIdFromTable(clientsTable);
  int ret =
      QMessageBox::question(this, Constants::Messages::CONFIRMATION,
                            Constants::ConfirmationMessages::DELETE_CLIENT,
                            QMessageBox::Yes | QMessageBox::No);

  if (ret == QMessageBox::Yes) {
    agency->getClientManager().removeClient(Utils::toString(id));
    refreshClients();
    updateDashboardStats();
    showStatusMessage(Constants::OperationMessages::CLIENT_DELETED);
  }
}

void MainWindow::refreshClients() {
  updateClientsTable();
  showStatusMessage(
      QString("Клиенты: %1 записей").arg(agency->getClientManager().getCount()),
      Constants::MessageTimeout::SHORT);
}

void MainWindow::searchClients() {
  if (!clientsTable || !searchClientEdit) {
    return;
  }

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

  std::vector<Client *> clients;
  if (isNumericOnly && searchText.length() >= 6 && searchText.length() <= 8) {
    // Поиск по ID
    Client *client =
        agency->getClientManager().findClient(searchText.toStdString());
    if (client) {
      clients.push_back(client);
    }
  } else {
    // Поиск по имени
    clients = agency->getClientManager().searchByName(searchText.toStdString());
  }

  for (Client *client : clients) {
    if (!client)
      continue;

    int row = clientsTable->rowCount();
    clientsTable->insertRow(row);

    clientsTable->setItem(
        row, 0, new QTableWidgetItem(QString::fromStdString(client->getId())));
    clientsTable->setItem(
        row, 1,
        new QTableWidgetItem(QString::fromStdString(client->getName())));
    clientsTable->setItem(
        row, 2,
        new QTableWidgetItem(QString::fromStdString(client->getPhone())));
    clientsTable->setItem(
        row, 3,
        new QTableWidgetItem(QString::fromStdString(client->getEmail())));
    clientsTable->setItem(row, 4,
                          new QTableWidgetItem(QString::fromStdString(
                              client->getRegistrationDate())));

    QString clientId = QString::fromStdString(client->getId());
    QWidget *actionsWidget = createActionButtons(
        clientsTable, clientId, [this]() { editClient(); },
        [this]() { deleteClient(); });
    clientsTable->setCellWidget(row, 5, actionsWidget);
  }

  if (clients.empty()) {
    if (statusBar()) {
      statusBar()->showMessage("Ничего не найдено", 2000);
    }
  } else {
    if (statusBar()) {
      statusBar()->showMessage(
          QString("Найдено: %1 клиентов").arg(clients.size()), 2000);
    }
  }
}

void MainWindow::addTransaction() {
  QStringList propertyIds, clientIds;
  auto properties = agency->getPropertyManager().getAllProperties();
  auto clients = agency->getClientManager().getAllClients();

  if (properties.empty()) {
    QMessageBox::information(this, "Информация",
                             "Нет доступной недвижимости для создания сделки");
    return;
  }

  if (clients.empty()) {
    QMessageBox::information(
        this, "Информация",
        "Нет зарегистрированных клиентов для создания сделки");
    return;
  }

  for (Property *p : properties) {
    if (p) {
      propertyIds << QString::fromStdString(p->getId() + " - " +
                                            p->getAddress());
    }
  }
  for (Client *c : clients) {
    if (c) {
      clientIds << QString::fromStdString(c->getId() + " - " + c->getName());
    }
  }

  if (propertyIds.isEmpty() || clientIds.isEmpty()) {
    QMessageBox::information(this, "Информация",
                             "Недостаточно данных для создания сделки");
    return;
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
          dialog.getId().toStdString(), propertyId, clientId,
          dialog.getFinalPrice(), status, dialog.getNotes().toStdString());
      agency->getTransactionManager().addTransaction(trans);

      // Помечаем недвижимость как недоступную, если сделка активна
      Property *prop = agency->getPropertyManager().findProperty(propertyId);
      if (prop && (status == Constants::TransactionStatus::PENDING ||
                   status == Constants::TransactionStatus::COMPLETED)) {
        prop->setAvailable(false);
      }

      refreshTransactions();
      refreshProperties(); // Обновляем, чтобы показать изменение доступности
      updateDashboardStats();
      if (statusBar()) {
        statusBar()->showMessage("Сделка добавлена", 3000);
      }
    } catch (const std::exception &e) {
      QMessageBox::warning(this, "Ошибка",
                           QString("Ошибка добавления: %1").arg(e.what()));
    }
  }
}

void MainWindow::editTransaction() {
  if (!transactionsTable) {
    return;
  }

  int row = transactionsTable->currentRow();
  if (row < 0 || row >= transactionsTable->rowCount()) {
    QMessageBox::information(this, "Информация",
                             "Выберите сделку для редактирования");
    return;
  }

  QTableWidgetItem *item = transactionsTable->item(row, 0);
  if (!item) {
    return;
  }

  QString id = item->text();
  Transaction *trans =
      agency->getTransactionManager().findTransaction(id.toStdString());
  if (!trans)
    return;

  QStringList propertyIds, clientIds;
  auto properties = agency->getPropertyManager().getAllProperties();
  auto clients = agency->getClientManager().getAllClients();

  if (properties.empty() || clients.empty()) {
    QMessageBox::warning(this, "Ошибка",
                         "Недостаточно данных для редактирования сделки");
    return;
  }

  for (Property *p : properties) {
    if (p) {
      propertyIds << QString::fromStdString(p->getId() + " - " +
                                            p->getAddress());
    }
  }
  for (Client *c : clients) {
    if (c) {
      clientIds << QString::fromStdString(c->getId() + " - " + c->getName());
    }
  }

  if (propertyIds.isEmpty() || clientIds.isEmpty()) {
    QMessageBox::warning(this, "Ошибка",
                         "Недостаточно данных для редактирования сделки");
    return;
  }

  TransactionDialog dialog(this, trans, propertyIds, clientIds);
  if (dialog.exec() == QDialog::Accepted) {
    try {
      std::string propertyId = dialog.getPropertyId().toStdString();
      std::string clientId = dialog.getClientId().toStdString();
      std::string status = dialog.getStatus().toStdString();

      // Валидация - исключаем текущую сделку из проверки
      if (!validateTransaction(propertyId, clientId, status,
                               id.toStdString())) {
        return;
      }

      std::string oldPropertyId = trans->getPropertyId();
      Property *oldProp =
          agency->getPropertyManager().findProperty(oldPropertyId);

      agency->getTransactionManager().removeTransaction(id.toStdString());
      auto newTrans = std::make_shared<Transaction>(
          dialog.getId().toStdString(), propertyId, clientId,
          dialog.getFinalPrice(), status, dialog.getNotes().toStdString());
      agency->getTransactionManager().addTransaction(newTrans);

      // Обновляем доступность недвижимости
      Property *prop = agency->getPropertyManager().findProperty(propertyId);
      if (prop) {
        bool shouldBeAvailable = (status == "cancelled");
        prop->setAvailable(shouldBeAvailable);
      }

      // Освобождаем старую недвижимость, если она была другая
      if (oldProp && oldPropertyId != propertyId) {
        if (!hasActiveTransactions(oldPropertyId)) {
          oldProp->setAvailable(true);
        }
      }

      refreshTransactions();
      refreshProperties();
      updateDashboardStats();
      if (statusBar()) {
        statusBar()->showMessage("Сделка обновлена", 3000);
      }
    } catch (const std::exception &e) {
      QMessageBox::warning(this, "Ошибка",
                           QString("Ошибка редактирования: %1").arg(e.what()));
    }
  }
}

void MainWindow::deleteTransaction() {
  if (!transactionsTable) {
    return;
  }

  int row = transactionsTable->currentRow();
  if (row < 0 || row >= transactionsTable->rowCount()) {
    QMessageBox::information(this, "Информация",
                             "Выберите сделку для удаления");
    return;
  }

  QTableWidgetItem *item = transactionsTable->item(row, 0);
  if (!item) {
    return;
  }

  QString id = item->text();
  int ret = QMessageBox::question(this, "Подтверждение", "Удалить сделку?",
                                  QMessageBox::Yes | QMessageBox::No);

  if (ret == QMessageBox::Yes) {
    Transaction *trans =
        agency->getTransactionManager().findTransaction(id.toStdString());
    std::string propertyId = trans ? trans->getPropertyId() : "";

    agency->getTransactionManager().removeTransaction(id.toStdString());

    // Освобождаем недвижимость, если нет других активных сделок
    if (!propertyId.empty()) {
      Property *prop = agency->getPropertyManager().findProperty(propertyId);
      if (prop) {
        auto remainingTrans =
            agency->getTransactionManager().getTransactionsByProperty(
                propertyId);
        bool hasActive = false;
        for (Transaction *t : remainingTrans) {
          if (t &&
              (t->getStatus() == "pending" || t->getStatus() == "completed")) {
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
    updateDashboardStats();
    if (statusBar()) {
      statusBar()->showMessage("Сделка удалена", 3000);
    }
  }
}

void MainWindow::refreshTransactions() {
  updateTransactionsTable();
  if (statusBar()) {
    statusBar()->showMessage(
        QString("Сделки: %1 записей")
            .arg(agency->getTransactionManager().getCount()),
        2000);
  }
}

void MainWindow::searchTransactions() {
  if (!transactionsTable || !searchTransactionEdit) {
    return;
  }

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
    Transaction *trans = agency->getTransactionManager().findTransaction(
        searchText.toStdString());
    if (trans) {
      int row = transactionsTable->rowCount();
      transactionsTable->insertRow(row);

      Property *prop =
          agency->getPropertyManager().findProperty(trans->getPropertyId());
      Client *client =
          agency->getClientManager().findClient(trans->getClientId());

      transactionsTable->setItem(
          row, 0, new QTableWidgetItem(QString::fromStdString(trans->getId())));
      transactionsTable->setItem(
          row, 1,
          new QTableWidgetItem(prop ? QString::fromStdString(prop->getAddress())
                                    : "N/A"));
      transactionsTable->setItem(
          row, 2,
          new QTableWidgetItem(
              client ? QString::fromStdString(client->getName()) : "N/A"));
      transactionsTable->setItem(row, 3,
                                 new QTableWidgetItem(QString::number(
                                     trans->getFinalPrice(), 'f', 2)));
      transactionsTable->setItem(
          row, 4,
          new QTableWidgetItem(QString::fromStdString(trans->getDate())));

      QString statusText;
      if (trans->getStatus() == "pending")
        statusText = "В ожидании";
      else if (trans->getStatus() == "completed")
        statusText = "Завершена";
      else
        statusText = "Отменена";
      transactionsTable->setItem(row, 5, new QTableWidgetItem(statusText));

      QString transId = QString::fromStdString(trans->getId());
      QWidget *actionsWidget = createActionButtons(
          transactionsTable, transId, [this]() { editTransaction(); },
          [this]() { deleteTransaction(); });
      transactionsTable->setCellWidget(row, 6, actionsWidget);

      if (statusBar()) {
        statusBar()->showMessage("Сделка найдена", 2000);
      }
    } else {
      if (statusBar()) {
        statusBar()->showMessage("Сделка не найдена", 2000);
      }
    }
  } else {
    QMessageBox::information(this, "Информация",
                             "Введите ID сделки (6-8 цифр)");
    refreshTransactions();
  }
}

void MainWindow::saveAllData() {
  if (!agency) {
    QMessageBox::warning(this, "Ошибка", "Система не инициализирована");
    return;
  }

  try {
    agency->saveAllData();
    refreshProperties();
    refreshClients();
    refreshTransactions();
    refreshAuctions();
    updateDashboardStats();
    if (statusBar()) {
      statusBar()->showMessage("Данные сохранены", 3000);
    }
    QMessageBox::information(this, "Успех", "Все данные успешно сохранены");
  } catch (const std::exception &e) {
    QMessageBox::warning(this, "Ошибка",
                         QString("Ошибка сохранения: %1").arg(e.what()));
  } catch (...) {
    QMessageBox::warning(this, "Ошибка",
                         "Неизвестная ошибка при сохранении данных");
  }
}

void MainWindow::loadAllData() {
  if (!agency) {
    QMessageBox::warning(this, "Ошибка", "Система не инициализирована");
    return;
  }

  try {
    agency->loadAllData();
    refreshProperties();
    refreshClients();
    refreshTransactions();
    refreshAuctions();
    updateDashboardStats();
    if (statusBar()) {
      statusBar()->showMessage("Данные загружены", 3000);
    }
    QMessageBox::information(this, "Успех", "Данные успешно загружены");
  } catch (const std::exception &e) {
    QMessageBox::warning(this, "Ошибка",
                         QString("Ошибка загрузки: %1").arg(e.what()));
  } catch (...) {
    QMessageBox::warning(this, "Ошибка",
                         "Неизвестная ошибка при загрузке данных");
  }
}

void MainWindow::showClientDetails(Client *client) {
  if (!client)
    return;

  QString html;
  html += "<html><body style='font-family: Arial, sans-serif;'>";
  html += "<h2 style='font-weight: bold; margin-bottom: 15px;'>ИНФОРМАЦИЯ О "
          "КЛИЕНТЕ</h2>";
  html += "<p><b>Идентификатор:</b> " +
          QString::fromStdString(client->getId()) + "</p>";
  html +=
      "<p><b>Имя:</b> " + QString::fromStdString(client->getName()) + "</p>";
  html += "<p><b>Телефон:</b> " + QString::fromStdString(client->getPhone()) +
          "</p>";
  html += "<p><b>Электронная почта:</b> " +
          QString::fromStdString(client->getEmail()) + "</p>";
  html += "<p><b>Дата регистрации:</b> " +
          QString::fromStdString(client->getRegistrationDate()) + "</p>";
  html += "</body></html>";

  clientDetailsText->setHtml(html);
}

void MainWindow::showTransactionDetails(Transaction *trans) {
  if (!trans)
    return;

  Property *prop =
      agency->getPropertyManager().findProperty(trans->getPropertyId());
  Client *client = agency->getClientManager().findClient(trans->getClientId());

  QString html;
  html += "<html><body style='font-family: Arial, sans-serif;'>";
  html +=
      "<h2 style='font-weight: bold; margin-bottom: 15px;'>ДЕТАЛИ СДЕЛКИ</h2>";

  html +=
      "<p><b>ID сделки:</b> " + QString::fromStdString(trans->getId()) + "</p>";
  html +=
      "<p><b>Дата:</b> " + QString::fromStdString(trans->getDate()) + "</p>";
  html += "<p><b>Цена:</b> " + QString::number(trans->getFinalPrice(), 'f', 2) +
          " руб.</p>";
  html += "<p><b>Статус:</b> " +
          TableHelper::getTransactionStatusText(trans->getStatus()) + "</p>";
  if (!trans->getNotes().empty()) {
    html += "<p><b>Примечания:</b> " +
            QString::fromStdString(trans->getNotes()) + "</p>";
  }

  html += "<h3 style='font-weight: bold; margin-top: 20px; margin-bottom: "
          "10px;'>НЕДВИЖИМОСТЬ</h3>";
  if (prop) {
    html += "<p><b>ID:</b> " + QString::fromStdString(prop->getId()) + "</p>";
    html += "<p><b>Тип:</b> " +
            TableHelper::getPropertyTypeText(prop->getType()) + "</p>";
    html += "<p><b>Адрес:</b> " + QString::fromStdString(prop->getAddress()) +
            "</p>";
    html += "<p><b>Цена:</b> " + QString::number(prop->getPrice(), 'f', 2) +
            " руб.</p>";
    html += "<p><b>Площадь:</b> " + QString::number(prop->getArea(), 'f', 2) +
            " м²</p>";
    html += "<p><b>Доступность:</b> " +
            QString(prop->getIsAvailable() ? "Доступна" : "Недоступна") +
            "</p>";
  } else {
    html += "<p>Недвижимость не найдена.</p>";
  }

  html += "<h3 style='font-weight: bold; margin-top: 20px; margin-bottom: "
          "10px;'>КЛИЕНТ</h3>";
  if (client) {
    html += "<p><b>ID:</b> " + QString::fromStdString(client->getId()) + "</p>";
    html +=
        "<p><b>Имя:</b> " + QString::fromStdString(client->getName()) + "</p>";
    html += "<p><b>Телефон:</b> " + QString::fromStdString(client->getPhone()) +
            "</p>";
    html += "<p><b>Электронная почта:</b> " +
            QString::fromStdString(client->getEmail()) + "</p>";
  } else {
    html += "<p>Клиент не найден.</p>";
  }

  html += "</body></html>";
  transactionDetailsText->setHtml(html);
}

void MainWindow::showPropertyTransactions(const std::string &propertyId) {
  auto transactions =
      agency->getTransactionManager().getTransactionsByProperty(propertyId);

  QString html;
  QString existingHtml = propertyDetailsText->toHtml();
  if (!existingHtml.isEmpty() && !existingHtml.contains("<body>")) {
    html = "<html><body style='font-family: Arial, sans-serif;'>";
  } else if (!existingHtml.isEmpty()) {
    html = existingHtml;
    html = html.replace("</body></html>", "");
  } else {
    html = "<html><body style='font-family: Arial, sans-serif;'>";
  }

  html += "<h3 style='font-weight: bold; margin-top: 20px; margin-bottom: "
          "10px;'>СВЯЗАННЫЕ СДЕЛКИ</h3>";

  if (transactions.empty()) {
    html += "<p>У недвижимости пока нет связанных сделок.</p>";
  } else {
    int num = 1;
    for (Transaction *trans : transactions) {
      if (!trans)
        continue;

      Client *client =
          agency->getClientManager().findClient(trans->getClientId());
      html += "<div style='margin-bottom: 15px;'>";
      html += "<h4 style='font-weight: bold; margin-bottom: 5px;'>Сделка #" +
              QString::number(num) + "</h4>";
      html += "<p><b>ID сделки:</b> " + QString::fromStdString(trans->getId()) +
              "</p>";
      html += "<p><b>Клиент:</b> " +
              QString::fromStdString(client ? client->getName() : "Не указан") +
              "</p>";
      html += "<p><b>Цена:</b> " +
              QString::number(trans->getFinalPrice(), 'f', 2) + " руб.</p>";
      html += "<p><b>Статус:</b> " +
              TableHelper::getTransactionStatusText(trans->getStatus()) +
              "</p>";
      html += "<p><b>Дата:</b> " + QString::fromStdString(trans->getDate()) +
              "</p>";
      html += "</div>";
      num++;
    }
  }

  html += "</body></html>";
  propertyDetailsText->setHtml(html);
}

void MainWindow::showClientTransactions(const std::string &clientId) {
  auto transactions =
      agency->getTransactionManager().getTransactionsByClient(clientId);

  QString html;
  QString existingHtml = clientDetailsText->toHtml();
  if (!existingHtml.isEmpty() && !existingHtml.contains("<body>")) {
    html = "<html><body style='font-family: Arial, sans-serif;'>";
  } else if (!existingHtml.isEmpty()) {
    html = existingHtml;
    html = html.replace("</body></html>", "");
  } else {
    html = "<html><body style='font-family: Arial, sans-serif;'>";
  }

  html += "<h3 style='font-weight: bold; margin-top: 20px; margin-bottom: "
          "10px;'>СВЯЗАННЫЕ СДЕЛКИ (" +
          QString::number(transactions.size()) + ")</h3>";

  if (transactions.empty()) {
    html += "<p>У клиента пока нет связанных сделок.</p>";
  } else {
    int num = 1;
    for (Transaction *trans : transactions) {
      if (!trans)
        continue;

      Property *prop =
          agency->getPropertyManager().findProperty(trans->getPropertyId());
      html += "<div style='margin-bottom: 15px;'>";
      html += "<h4 style='font-weight: bold; margin-bottom: 5px;'>Сделка #" +
              QString::number(num) + "</h4>";
      html += "<p><b>ID сделки:</b> " + QString::fromStdString(trans->getId()) +
              "</p>";
      html += "<p><b>Недвижимость:</b> " +
              QString::fromStdString(prop ? prop->getAddress() : "Не указана") +
              "</p>";
      html += "<p><b>Цена:</b> " +
              QString::number(trans->getFinalPrice(), 'f', 2) + " руб.</p>";
      html += "<p><b>Статус:</b> " +
              TableHelper::getTransactionStatusText(trans->getStatus()) +
              "</p>";
      html += "<p><b>Дата:</b> " + QString::fromStdString(trans->getDate()) +
              "</p>";
      html += "</div>";
      num++;
    }
  }

  html += "</body></html>";
  clientDetailsText->setHtml(html);
}

bool MainWindow::validateTransaction(const std::string &propertyId,
                                     const std::string &clientId,
                                     const std::string &status,
                                     const std::string &excludeTransactionId) {

  Property *prop = agency->getPropertyManager().findProperty(propertyId);
  if (!prop) {
    QMessageBox::warning(this, "Ошибка валидации",
                         QString("Недвижимость с ID '%1' не найдена!")
                             .arg(QString::fromStdString(propertyId)));
    return false;
  }

  // Проверка существования клиента
  Client *client = agency->getClientManager().findClient(clientId);
  if (!client) {
    QMessageBox::warning(this, "Ошибка валидации",
                         QString("Клиент с ID '%1' не найден!")
                             .arg(QString::fromStdString(clientId)));
    return false;
  }

  // Проверка доступности недвижимости при статусе pending или completed
  if (status == "pending" || status == "completed") {
    // Всегда проверяем, нет ли уже активных сделок для этой недвижимости
    auto existingTrans =
        agency->getTransactionManager().getTransactionsByProperty(propertyId);
    bool hasActive = false;
    for (Transaction *t : existingTrans) {
      // Исключаем текущую редактируемую сделку из проверки
      if (t && !excludeTransactionId.empty() &&
          t->getId() == excludeTransactionId) {
        continue;
      }

      if (t && (t->getStatus() == "pending" || t->getStatus() == "completed")) {
        hasActive = true;
        break;
      }
    }

    if (hasActive) {
      QMessageBox::warning(this, "Ошибка валидации",
                           "Недвижимость уже используется в другой активной "
                           "сделке (ожидание или завершена)!");
      return false;
    }
  }

  return true;
}

void MainWindow::setupAuctionsPage() {
  auctionsPageWidget = new QWidget;
  QVBoxLayout *layout = new QVBoxLayout(auctionsPageWidget);
  layout->setContentsMargins(20, 20, 20, 20);
  layout->setSpacing(15);

  // Заголовок
  QHBoxLayout *headerLayout = new QHBoxLayout;
  QLabel *title = new QLabel("Аукционы");
  title->setStyleSheet("color: #ffffff; font-size: 22pt; font-weight: bold;");
  headerLayout->addWidget(title);
  headerLayout->addStretch();

  // Поиск
  searchAuctionEdit = new QLineEdit;
  searchAuctionEdit->setPlaceholderText("🔍 Поиск по ID");
  searchAuctionEdit->setFixedWidth(250);
  searchAuctionBtn = new QPushButton("Найти");
  headerLayout->addWidget(searchAuctionEdit);
  headerLayout->addWidget(searchAuctionBtn);

  addAuctionBtn = new QPushButton("➕ Создать");
  refreshAuctionBtn = new QPushButton("🔄 Обновить");
  headerLayout->addWidget(addAuctionBtn);
  headerLayout->addWidget(refreshAuctionBtn);
  layout->addLayout(headerLayout);

  // Таблица и детали
  QSplitter *splitter = new QSplitter(Qt::Horizontal);

  auctionsTable = new QTableWidget;
  auctionsTable->setColumnCount(7);
  auctionsTable->setHorizontalHeaderLabels({"ID", "Недвижимость",
                                            "Начальная цена", "Текущая ставка",
                                            "Статус", "Ставок", "Действия"});
  auctionsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
  auctionsTable->setSelectionMode(QAbstractItemView::SingleSelection);
  auctionsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
  auctionsTable->setAlternatingRowColors(true);
  auctionsTable->verticalHeader()->setDefaultSectionSize(60);
  auctionsTable->setColumnWidth(0, 100);
  auctionsTable->setColumnWidth(1, 250);
  auctionsTable->setColumnWidth(2, 150);
  auctionsTable->setColumnWidth(3, 150);
  auctionsTable->setColumnWidth(4, 120);
  auctionsTable->setColumnWidth(5, 80);
  auctionsTable->setColumnWidth(6, 300);
  auctionsTable->horizontalHeader()->setStretchLastSection(false);

  QFrame *detailsFrame = new QFrame;
  detailsFrame->setFixedWidth(400);
  detailsFrame->setStyleSheet(
      "background-color: #252525; border-radius: 8px; padding: 15px;");
  QVBoxLayout *detailsLayout = new QVBoxLayout(detailsFrame);
  QLabel *detailsTitle = new QLabel("Детали аукциона");
  detailsTitle->setStyleSheet("color: #ffffff; font-size: 14pt; font-weight: "
                              "bold; padding-bottom: 10px;");
  detailsLayout->addWidget(detailsTitle);
  auctionDetailsText = new QTextEdit;
  auctionDetailsText->setReadOnly(true);
  auctionDetailsText->setStyleSheet(
      "background-color: #2d2d2d; color: #d0d0d0; border: none;");
  detailsLayout->addWidget(auctionDetailsText);

  splitter->addWidget(auctionsTable);
  splitter->addWidget(detailsFrame);
  splitter->setStretchFactor(0, 2);
  splitter->setStretchFactor(1, 1);

  layout->addWidget(splitter);

  // Подключения
  connect(addAuctionBtn, &QPushButton::clicked, this, &MainWindow::addAuction);
  connect(refreshAuctionBtn, &QPushButton::clicked, this,
          &MainWindow::refreshAuctions);
  connect(searchAuctionBtn, &QPushButton::clicked, this,
          &MainWindow::searchAuctions);
  connect(auctionsTable, &QTableWidget::itemSelectionChanged, this, [this]() {
    if (!auctionsTable)
      return;
    int row = auctionsTable->currentRow();
    if (row >= 0 && row < auctionsTable->rowCount()) {
      QTableWidgetItem *item = auctionsTable->item(row, 0);
      if (item) {
        QString id = item->text();
        Auction *auction =
            agency->getAuctionManager().findAuction(id.toStdString());
        if (auction) {
          showAuctionDetails(auction);
        }
      }
    }
  });
}

void MainWindow::updateAuctionsTable() {
  if (!auctionsTable) {
    return; // Таблица еще не инициализирована
  }

  auctionsTable->setRowCount(0);
  auto auctions = agency->getAuctionManager().getAllAuctions();

  for (Auction *auction : auctions) {
    if (!auction)
      continue;

    int row = auctionsTable->rowCount();
    auctionsTable->insertRow(row);

    Property *prop =
        agency->getPropertyManager().findProperty(auction->getPropertyId());
    double currentBid = auction->getCurrentHighestBid();

    auctionsTable->setItem(
        row, 0, new QTableWidgetItem(QString::fromStdString(auction->getId())));
    auctionsTable->setItem(row, 1,
                           new QTableWidgetItem(QString::fromStdString(
                               auction->getPropertyAddress())));
    auctionsTable->setItem(
        row, 2,
        new QTableWidgetItem(
            QString::number(auction->getStartingPrice(), 'f', 2) + " руб."));

    QString currentBidText = (currentBid > 0)
                                 ? QString::number(currentBid, 'f', 2) + " руб."
                                 : "Нет ставок";
    auctionsTable->setItem(row, 3, new QTableWidgetItem(currentBidText));

    QString statusText;
    if (auction->getStatus() == "active")
      statusText = "Активен";
    else if (auction->getStatus() == "completed")
      statusText = "Завершен";
    else
      statusText = "Отменен";
    auctionsTable->setItem(row, 4, new QTableWidgetItem(statusText));
    auctionsTable->setItem(
        row, 5,
        new QTableWidgetItem(QString::number(auction->getBids().size())));

    QString auctionId = QString::fromStdString(auction->getId());
    QWidget *actionsWidget = createActionButtons(
        auctionsTable, auctionId, [this]() { viewAuction(); },
        [this]() { deleteAuction(); }, true);
    auctionsTable->setCellWidget(row, 6, actionsWidget);
  }
}

void MainWindow::addAuction() {
  QStringList propertyIds;
  auto properties = agency->getPropertyManager().getAllProperties();

  if (properties.empty()) {
    QMessageBox::information(
        this, "Информация", "Нет доступной недвижимости для создания аукциона");
    return;
  }

  // Исключаем недвижимость, которая уже продана или находится в активном
  // аукционе
  for (Property *p : properties) {
    if (!p)
      continue;

    std::string propertyId = p->getId();

    // Проверяем, есть ли активные или завершенные сделки
    if (hasActiveTransactions(propertyId)) {
      continue; // Пропускаем недвижимость с активными сделками
    }

    // Проверяем, не находится ли недвижимость уже в активном аукционе
    auto auctions =
        agency->getAuctionManager().getAuctionsByProperty(propertyId);
    bool isInActiveAuction = false;
    for (Auction *auction : auctions) {
      if (auction && auction->getStatus() == "active") {
        isInActiveAuction = true;
        break;
      }
    }

    if (!isInActiveAuction) {
      propertyIds << QString::fromStdString(propertyId + " - " +
                                            p->getAddress());
    }
  }

  if (propertyIds.isEmpty()) {
    QMessageBox::information(
        this, "Информация",
        "Нет доступной недвижимости для создания аукциона.\n"
        "Вся недвижимость либо уже продана, либо находится в активных "
        "аукционах.");
    return;
  }

  AuctionDialog dialog(this, nullptr, propertyIds);
  if (dialog.exec() == QDialog::Accepted) {
    try {
      QString id = dialog.getId();
      QString propertyId = dialog.getPropertyId();
      double startingPrice = dialog.getStartingPrice();

      Property *prop =
          agency->getPropertyManager().findProperty(propertyId.toStdString());
      if (!prop) {
        QMessageBox::warning(this, "Ошибка", "Недвижимость не найдена");
        return;
      }

      auto auction =
          std::make_shared<Auction>(id.toStdString(), propertyId.toStdString(),
                                    prop->getAddress(), startingPrice);

      agency->getAuctionManager().addAuction(auction);

      refreshAuctions();
      updateDashboardStats();
      if (statusBar()) {
        statusBar()->showMessage("Аукцион создан", 3000);
      }
    } catch (const std::exception &e) {
      QMessageBox::warning(
          this, "Ошибка",
          QString("Ошибка создания аукциона: %1").arg(e.what()));
    }
  }
}

void MainWindow::viewAuction() {
  if (!auctionsTable) {
    return; // Таблица еще не инициализирована
  }

  int row = auctionsTable->currentRow();
  if (row < 0) {
    QMessageBox::information(this, "Информация",
                             "Выберите аукцион для просмотра");
    return;
  }

  QString id = auctionsTable->item(row, 0)->text();
  Auction *auction = agency->getAuctionManager().findAuction(id.toStdString());
  if (!auction)
    return;

  QStringList propertyIds;
  auto properties = agency->getPropertyManager().getAllProperties();
  for (Property *p : properties) {
    propertyIds << QString::fromStdString(p->getId() + " - " + p->getAddress());
  }

  AuctionDialog dialog(this, auction, propertyIds);
  dialog.exec();

  refreshAuctions();
  // Обновляем сделки и недвижимость, так как могла быть создана транзакция
  refreshTransactions();
  refreshProperties();
  updateDashboardStats();
}

void MainWindow::deleteAuction() {
  if (!auctionsTable) {
    return; // Таблица еще не инициализирована
  }

  int row = auctionsTable->currentRow();
  if (row < 0 || row >= auctionsTable->rowCount()) {
    QMessageBox::information(this, "Информация",
                             "Выберите аукцион для удаления");
    return;
  }

  QTableWidgetItem *item = auctionsTable->item(row, 0);
  if (!item) {
    return;
  }

  QString id = item->text();
  int ret = QMessageBox::question(this, "Подтверждение", "Удалить аукцион?",
                                  QMessageBox::Yes | QMessageBox::No);

  if (ret == QMessageBox::Yes) {
    agency->getAuctionManager().removeAuction(id.toStdString());
    refreshAuctions();
    updateDashboardStats();
    if (statusBar()) {
      statusBar()->showMessage("Аукцион удален", 3000);
    }
  }
}

void MainWindow::refreshAuctions() {
  if (!auctionsTable) {
    return; // Таблица еще не инициализирована
  }

  updateAuctionsTable();
  if (statusBar()) {
    statusBar()->showMessage(QString("Аукционы: %1 записей")
                                 .arg(agency->getAuctionManager().getCount()),
                             2000);
  }
}

void MainWindow::searchAuctions() {
  if (!auctionsTable || !searchAuctionEdit) {
    return; // Элементы еще не инициализированы
  }

  QString searchText = searchAuctionEdit->text();
  if (searchText.isEmpty()) {
    refreshAuctions();
    return;
  }

  auctionsTable->setRowCount(0);

  // Проверяем, является ли поисковый запрос ID (только цифры)
  bool isNumericOnly = true;
  for (QChar ch : searchText) {
    if (!ch.isDigit()) {
      isNumericOnly = false;
      break;
    }
  }

  if (isNumericOnly && searchText.length() >= 6 && searchText.length() <= 8) {
    // Поиск по ID аукциона
    Auction *auction =
        agency->getAuctionManager().findAuction(searchText.toStdString());
    if (auction) {
      int row = auctionsTable->rowCount();
      auctionsTable->insertRow(row);

      Property *prop =
          agency->getPropertyManager().findProperty(auction->getPropertyId());
      double currentBid = auction->getCurrentHighestBid();

      auctionsTable->setItem(
          row, 0,
          new QTableWidgetItem(QString::fromStdString(auction->getId())));
      auctionsTable->setItem(row, 1,
                             new QTableWidgetItem(QString::fromStdString(
                                 auction->getPropertyAddress())));
      auctionsTable->setItem(
          row, 2,
          new QTableWidgetItem(
              QString::number(auction->getStartingPrice(), 'f', 2) + " руб."));

      QString currentBidText =
          (currentBid > 0) ? QString::number(currentBid, 'f', 2) + " руб."
                           : "Нет ставок";
      auctionsTable->setItem(row, 3, new QTableWidgetItem(currentBidText));

      QString statusText;
      if (auction->getStatus() == "active")
        statusText = "Активен";
      else if (auction->getStatus() == "completed")
        statusText = "Завершен";
      else
        statusText = "Отменен";
      auctionsTable->setItem(row, 4, new QTableWidgetItem(statusText));
      auctionsTable->setItem(
          row, 5,
          new QTableWidgetItem(QString::number(auction->getBids().size())));

      QString auctionId = QString::fromStdString(auction->getId());
      QWidget *actionsWidget = createActionButtons(
          auctionsTable, auctionId, [this]() { viewAuction(); },
          [this]() { deleteAuction(); }, true);
      auctionsTable->setCellWidget(row, 6, actionsWidget);
    }
  }

  if (auctionsTable->rowCount() == 0) {
    if (statusBar()) {
      statusBar()->showMessage("Ничего не найдено", 2000);
    }
  } else {
    if (statusBar()) {
      statusBar()->showMessage("Найдено", 2000);
    }
  }
}

void MainWindow::auctionSelectionChanged() {
  if (!auctionsTable) {
    return; // Таблица еще не инициализирована
  }

  int row = auctionsTable->currentRow();
  if (row >= 0 && row < auctionsTable->rowCount()) {
    QTableWidgetItem *item = auctionsTable->item(row, 0);
    if (item) {
      QString id = item->text();
      Auction *auction =
          agency->getAuctionManager().findAuction(id.toStdString());
      if (auction) {
        showAuctionDetails(auction);
      }
    }
  }
}

void MainWindow::showAuctionDetails(Auction *auction) {
  if (!auction)
    return;

  Property *prop =
      agency->getPropertyManager().findProperty(auction->getPropertyId());

  QString html;
  html += "<html><body style='font-family: Arial, sans-serif;'>";
  html += "<h2 style='font-weight: bold; margin-bottom: 15px;'>ДЕТАЛИ "
          "АУКЦИОНА</h2>";

  html += "<p><b>ID аукциона:</b> " + QString::fromStdString(auction->getId()) +
          "</p>";
  html += "<p><b>Начальная цена:</b> " +
          QString::number(auction->getStartingPrice(), 'f', 2) + " руб.</p>";
  html += "<p><b>Цена автоматической покупки:</b> " +
          QString::number(auction->getBuyoutPrice(), 'f', 2) +
          " руб. (+70%)</p>";
  html += "<p><b>Дата создания:</b> " +
          QString::fromStdString(auction->getCreatedAt()) + "</p>";

  QString statusText = TableHelper::getAuctionStatusText(auction->getStatus());
  html += "<p><b>Статус:</b> " + statusText + "</p>";

  if (!auction->getCompletedAt().empty()) {
    html += "<p><b>Дата завершения:</b> " +
            QString::fromStdString(auction->getCompletedAt()) + "</p>";
  }

  double currentBid = auction->getCurrentHighestBid();
  if (currentBid > 0) {
    Bid *highest = auction->getHighestBid();
    html += "<p><b>Текущая максимальная ставка:</b> " +
            QString::number(currentBid, 'f', 2) + " руб.</p>";
    if (highest) {
      html += "<p><b>Победитель:</b> " +
              QString::fromStdString(highest->getClientName()) +
              " (ID: " + QString::fromStdString(highest->getClientId()) +
              ")</p>";
    }
  } else {
    html += "<p><b>Текущая максимальная ставка:</b> Нет ставок</p>";
  }

  html += "<p><b>Количество ставок:</b> " +
          QString::number(auction->getBids().size()) + "</p>";

  if (auction->wasBuyout()) {
    html += "<p style='margin-top: 10px;'><b>Аукцион завершен автоматической "
            "покупкой.</b></p>";
  }

  html += "<h3 style='font-weight: bold; margin-top: 20px; margin-bottom: "
          "10px;'>НЕДВИЖИМОСТЬ</h3>";
  if (prop) {
    html += "<p><b>ID:</b> " + QString::fromStdString(prop->getId()) + "</p>";
    html += "<p><b>Тип:</b> " +
            TableHelper::getPropertyTypeText(prop->getType()) + "</p>";
    html += "<p><b>Адрес:</b> " + QString::fromStdString(prop->getAddress()) +
            "</p>";
    html += "<p><b>Цена:</b> " + QString::number(prop->getPrice(), 'f', 2) +
            " руб.</p>";
    html += "<p><b>Площадь:</b> " + QString::number(prop->getArea(), 'f', 2) +
            " м²</p>";
  } else {
    html += "<p>Недвижимость не найдена.</p>";
  }

  html += "<h3 style='font-weight: bold; margin-top: 20px; margin-bottom: "
          "10px;'>СТАВКИ</h3>";
  auto bids = auction->getBids();
  if (bids.empty()) {
    html += "<p>Ставок пока нет.</p>";
  } else {
    int num = 1;
    for (const auto &bid : bids) {
      html += "<div style='margin-bottom: 15px;'>";
      html += "<h4 style='font-weight: bold; margin-bottom: 5px;'>Ставка #" +
              QString::number(num) + "</h4>";
      html += "<p><b>Клиент:</b> " +
              QString::fromStdString(bid->getClientName()) + "</p>";
      html += "<p><b>ID клиента:</b> " +
              QString::fromStdString(bid->getClientId()) + "</p>";
      html += "<p><b>Сумма:</b> " + QString::number(bid->getAmount(), 'f', 2) +
              " руб.</p>";
      html += "<p><b>Время:</b> " +
              QString::fromStdString(bid->getTimestamp()) + "</p>";
      html += "</div>";
      num++;
    }
  }

  html += "</body></html>";
  auctionDetailsText->setHtml(html);
}
