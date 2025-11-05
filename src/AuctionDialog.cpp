#include "../include/AuctionDialog.h"
#include "../include/Bid.h"
#include "../include/Client.h"
#include "../include/EstateAgency.h"
#include "../include/Property.h"
#include "../include/Transaction.h"
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QVBoxLayout>
#include <cctype>
#include <ctime>
#include <iomanip>
#include <sstream>

AuctionDialog::AuctionDialog(QWidget *parent, Auction *editAuction,
                             const QStringList &propertyIds)
    : QDialog(parent), isViewMode(editAuction != nullptr),
      currentAuction(editAuction), agency(EstateAgency::getInstance()),
      propertyIds(propertyIds) {
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
        QTableWidget {
            background-color: #2d2d2d;
            color: #d0d0d0;
            border: 1px solid #404040;
            gridline-color: #404040;
        }
        QTableWidget::item {
            background-color: #2d2d2d;
            color: #d0d0d0;
        }
        QTableWidget::item:selected {
            background-color: #5a9;
            color: white;
        }
        QPushButton {
            background-color: #404040;
            color: white;
            min-width: 100px;
            padding: 6px;
            border: 1px solid #505050;
        }
        QPushButton:hover {
            background-color: #505050;
        }
        QPushButton:disabled {
            background-color: #2d2d2d;
            color: #808080;
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
        QGroupBox {
            color: #d0d0d0;
            border: 1px solid #404040;
            margin-top: 10px;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 10px;
            padding: 0 5px;
        }
    )");

  setupUI();

  propertyCombo->addItems(propertyIds);

  // Заполняем список клиентов (только если clientCombo был создан)
  if (clientCombo) {
    auto clients = agency->getClientManager().getAllClients();
    QStringList clientIds;
    for (auto *client : clients) {
      clientIds << QString::fromStdString(client->getId() + " - " +
                                          client->getName());
    }
    clientCombo->addItems(clientIds);
  }

  if (editAuction) {
    loadAuctionData(editAuction);
    setWindowTitle("Аукцион: " + QString::fromStdString(editAuction->getId()));
  } else {
    setWindowTitle("Создать аукцион");
  }

  setMinimumWidth(600);
  setMinimumHeight(500);
}

AuctionDialog::~AuctionDialog() {}

void AuctionDialog::setupUI() {
  QVBoxLayout *mainLayout = new QVBoxLayout(this);

  // Форма для создания/просмотра аукциона
  QGroupBox *auctionGroup =
      new QGroupBox(isViewMode ? "Информация об аукционе" : "Создать аукцион");
  QFormLayout *formLayout = new QFormLayout(auctionGroup);

  idEdit = new QLineEdit;
  idEdit->setPlaceholderText("123456 (6-8 цифр)");
  idEdit->setMaxLength(8);
  idEdit->setToolTip(
      "ID должен содержать только цифры\nДлина: от 6 до 8 символов");
  if (isViewMode) {
    idEdit->setReadOnly(true);
  }

  propertyCombo = new QComboBox;
  propertyCombo->setToolTip("Выберите недвижимость для аукциона");
  if (isViewMode) {
    propertyCombo->setEnabled(false);
  }
  connect(propertyCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
          this, &AuctionDialog::onPropertyChanged);

  priceSpin = new QDoubleSpinBox;
  priceSpin->setMinimum(10000);
  priceSpin->setMaximum(1000000000);
  priceSpin->setValue(100000);
  priceSpin->setSuffix(" руб.");
  priceSpin->setDecimals(2);
  priceSpin->setSingleStep(10000);
  priceSpin->setToolTip("Начальная цена аукциона");
  if (isViewMode) {
    priceSpin->setReadOnly(true);
  }

  propertyPriceLabel = new QLabel("Цена недвижимости: не выбрана");
  propertyPriceLabel->setStyleSheet(
      "color: #8fa8b3; font-size: 9pt; padding-left: 5px;");

  buyoutPriceLabel = new QLabel("Цена автоматической покупки: не рассчитана");
  buyoutPriceLabel->setStyleSheet(
      "color: #a96; font-size: 9pt; padding-left: 5px;");
  buyoutPriceLabel->setToolTip("Если клиент предложит эту цену или выше, "
                               "недвижимость будет сразу выкуплена");

  statusLabel = new QLabel("Статус: не установлен");
  statusLabel->setStyleSheet("color: #6a9; font-size: 9pt; font-weight: bold;");

  formLayout->addRow("ID аукциона:", idEdit);
  formLayout->addRow("Недвижимость:", propertyCombo);
  formLayout->addRow("Начальная цена:", priceSpin);
  formLayout->addRow("", propertyPriceLabel);
  formLayout->addRow("", buyoutPriceLabel);
  formLayout->addRow("", statusLabel);

  mainLayout->addWidget(auctionGroup);

  // Раздел для ставок (только в режиме просмотра)
  if (isViewMode) {
    QGroupBox *bidsGroup = new QGroupBox("Ставки");
    QVBoxLayout *bidsLayout = new QVBoxLayout(bidsGroup);

    currentHighestBidLabel =
        new QLabel("Текущая максимальная ставка: нет ставок");
    currentHighestBidLabel->setStyleSheet(
        "color: #6a9; font-size: 10pt; font-weight: bold; padding: 5px;");
    bidsLayout->addWidget(currentHighestBidLabel);

    bidsTable = new QTableWidget;
    bidsTable->setColumnCount(4);
    bidsTable->setHorizontalHeaderLabels(
        {"Клиент", "ID клиента", "Ставка", "Время"});
    bidsTable->horizontalHeader()->setStretchLastSection(true);
    bidsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    bidsLayout->addWidget(bidsTable);

    // Форма для добавления ставки
    QGroupBox *addBidGroup = new QGroupBox("Добавить ставку");
    QFormLayout *bidFormLayout = new QFormLayout(addBidGroup);

    clientCombo = new QComboBox;

    bidAmountSpin = new QDoubleSpinBox;
    bidAmountSpin->setMinimum(0.01);
    bidAmountSpin->setMaximum(1000000000);
    bidAmountSpin->setSuffix(" руб.");
    bidAmountSpin->setDecimals(2);
    bidAmountSpin->setSingleStep(0.01);
    bidAmountSpin->setToolTip("Минимальная ставка должна быть на 0.01 руб. "
                              "больше текущей максимальной");

    bidFormLayout->addRow("Клиент:", clientCombo);
    bidFormLayout->addRow("Ставка:", bidAmountSpin);

    QHBoxLayout *bidButtonsLayout = new QHBoxLayout;
    addBidBtn = new QPushButton("Сделать ставку");
    connect(addBidBtn, &QPushButton::clicked, this, &AuctionDialog::addBid);
    bidButtonsLayout->addWidget(addBidBtn);
    bidButtonsLayout->addStretch();
    bidFormLayout->addRow("", bidButtonsLayout);

    bidsLayout->addWidget(addBidGroup);

    completeAuctionBtn = new QPushButton("Завершить аукцион");
    completeAuctionBtn->setStyleSheet("background-color: #a66; color: white; "
                                      "font-weight: bold; padding: 8px;");
    connect(completeAuctionBtn, &QPushButton::clicked, this,
            &AuctionDialog::completeAuction);
    bidsLayout->addWidget(completeAuctionBtn);

    mainLayout->addWidget(bidsGroup);

    refreshAuctionInfo();
  } else {
    currentHighestBidLabel = nullptr;
    bidsTable = nullptr;
    clientCombo = nullptr;
    bidAmountSpin = nullptr;
    addBidBtn = nullptr;
    completeAuctionBtn = nullptr;
  }

  buttonBox = new QDialogButtonBox(
      isViewMode ? QDialogButtonBox::Close
                 : (QDialogButtonBox::Ok | QDialogButtonBox::Cancel));
  mainLayout->addWidget(buttonBox);

  if (isViewMode) {
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
  } else {
    connect(buttonBox, &QDialogButtonBox::accepted, this,
            &AuctionDialog::validateAndAccept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
  }
}

void AuctionDialog::loadAuctionData(Auction *auction) {
  if (!auction)
    return;

  idEdit->setText(QString::fromStdString(auction->getId()));

  // Ищем индекс по ID, сравнивая начало строки "ID - "
  QString propertyId = QString::fromStdString(auction->getPropertyId());
  int propIndex = -1;
  for (int i = 0; i < propertyIds.size(); ++i) {
    if (propertyIds[i].startsWith(propertyId + " - ")) {
      propIndex = i;
      break;
    }
  }
  if (propIndex >= 0) {
    propertyCombo->setCurrentIndex(propIndex);
  }

  priceSpin->setValue(auction->getStartingPrice());
  updatePropertyInfo();
  refreshBids();
  refreshAuctionInfo();
}

void AuctionDialog::onPropertyChanged(int index) {
  if (index >= 0) {
    updatePropertyInfo();
  }
}

void AuctionDialog::updatePropertyInfo() {
  if (propertyCombo->currentIndex() < 0 ||
      propertyCombo->currentIndex() >= propertyIds.size()) {
    propertyPriceLabel->setText("Цена недвижимости: не выбрана");
    buyoutPriceLabel->setText("Цена автоматической покупки: не рассчитана");
    return;
  }

  QString propId =
      propertyIds[propertyCombo->currentIndex()].split(" - ").first();
  Property *prop =
      agency->getPropertyManager().findProperty(propId.toStdString());

  if (prop) {
    double propertyPrice = prop->getPrice();
    QString priceText = QString("Цена недвижимости: %1 руб.")
                            .arg(QString::number(propertyPrice, 'f', 2));
    propertyPriceLabel->setText(priceText);

    if (!isViewMode) {
      // Обновляем начальную цену аукциона на цену недвижимости
      priceSpin->setValue(propertyPrice);
    }

    // Обновляем цену автоматической покупки
    double startingPrice = priceSpin->value();
    double buyoutPrice = startingPrice * 1.7;
    QString buyoutText = QString("Цена автоматической покупки: %1 руб. (+70%%)")
                             .arg(QString::number(buyoutPrice, 'f', 2));
    buyoutPriceLabel->setText(buyoutText);
  }

  // Обновляем цену автоматической покупки при изменении начальной цены
  connect(priceSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          [this](double value) {
            double buyoutPrice = value * 1.7;
            QString buyoutText =
                QString("Цена автоматической покупки: %1 руб. (+70%%)")
                    .arg(QString::number(buyoutPrice, 'f', 2));
            buyoutPriceLabel->setText(buyoutText);
          });
}

void AuctionDialog::refreshBids() {
  if (!currentAuction || !bidsTable)
    return;

  auto bids = currentAuction->getBids();
  bidsTable->setRowCount(bids.size());

  for (size_t i = 0; i < bids.size(); ++i) {
    const auto &bid = bids[i];
    bidsTable->setItem(
        i, 0,
        new QTableWidgetItem(QString::fromStdString(bid->getClientName())));
    bidsTable->setItem(
        i, 1, new QTableWidgetItem(QString::fromStdString(bid->getClientId())));
    bidsTable->setItem(
        i, 2,
        new QTableWidgetItem(QString::number(bid->getAmount(), 'f', 2) +
                             " руб."));
    bidsTable->setItem(
        i, 3,
        new QTableWidgetItem(QString::fromStdString(bid->getTimestamp())));
  }

  bidsTable->sortItems(2,
                       Qt::DescendingOrder); // Сортировка по ставке (убывание)
  refreshAuctionInfo();
}

void AuctionDialog::addBid() {
  if (!currentAuction || !isViewMode)
    return;

  if (!currentAuction->isActive()) {
    QMessageBox::warning(this, "Ошибка",
                         "Аукцион не активен. Ставки больше не принимаются.");
    return;
  }

  if (clientCombo->currentIndex() < 0) {
    QMessageBox::warning(this, "Ошибка", "Выберите клиента");
    return;
  }

  QString clientStr = clientCombo->currentText();
  QString clientId = clientStr.split(" - ").first();

  // Находим клиента для получения имени
  Client *client =
      agency->getClientManager().findClient(clientId.toStdString());
  if (!client) {
    QMessageBox::warning(this, "Ошибка", "Клиент не найден");
    return;
  }

  double bidAmount = bidAmountSpin->value();

  // Создаем ставку
  auto bid = std::make_shared<Bid>(clientId.toStdString(), client->getName(),
                                   bidAmount);

  // Добавляем ставку в аукцион
  if (!currentAuction->addBid(bid)) {
    double currentHighest = currentAuction->getCurrentHighestBid();
    double minBid = (currentHighest > 0) ? currentHighest + 0.01
                                         : currentAuction->getStartingPrice();

    QString errorMsg = QString("Ставка слишком низкая!\n") +
                       QString("Минимальная ставка: %1 руб.\n")
                           .arg(QString::number(minBid, 'f', 2));

    if (bidAmount >= currentAuction->getBuyoutPrice()) {
      errorMsg +=
          QString("\n💡 При ставке %1 руб. недвижимость будет сразу выкуплена!")
              .arg(QString::number(currentAuction->getBuyoutPrice(), 'f', 2));
    }

    QMessageBox::warning(this, "Ошибка", errorMsg);
    return;
  }

  // Проверяем, не была ли автоматическая покупка
  if (bidAmount >= currentAuction->getBuyoutPrice()) {
    // Аукцион уже завершен в addBid(), создаем сделку
    createTransactionFromAuction();

    QMessageBox::information(this, "Аукцион завершен",
                             QString("Недвижимость автоматически выкуплена по "
                                     "цене %1 руб.!\nСделка создана.")
                                 .arg(QString::number(bidAmount, 'f', 2)));
  }

  refreshBids();
  refreshAuctionInfo();

  // Обновляем минимальную ставку
  double newMinBid = currentAuction->getCurrentHighestBid() + 0.01;
  bidAmountSpin->setMinimum(newMinBid);
  bidAmountSpin->setValue(newMinBid);

  // Если аукцион завершен, отключаем кнопки
  if (!currentAuction->isActive()) {
    addBidBtn->setEnabled(false);
    bidAmountSpin->setEnabled(false);
    clientCombo->setEnabled(false);
    if (completeAuctionBtn) {
      completeAuctionBtn->setEnabled(false);
    }
  }
}

void AuctionDialog::completeAuction() {
  if (!currentAuction)
    return;

  if (!currentAuction->isActive()) {
    QMessageBox::information(this, "Информация", "Аукцион уже завершен");
    return;
  }

  // Проверяем, есть ли ставки
  Bid *winner = currentAuction->getHighestBid();
  if (!winner) {
    int ret = QMessageBox::question(
        this, "Завершить аукцион",
        "На аукционе нет ставок. Завершить аукцион без сделки?",
        QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes) {
      currentAuction->cancel(); // Отменяем, если нет ставок
      refreshAuctionInfo();
      addBidBtn->setEnabled(false);
      bidAmountSpin->setEnabled(false);
      clientCombo->setEnabled(false);
      completeAuctionBtn->setEnabled(false);
      QMessageBox::information(this, "Аукцион отменен",
                               "Аукцион отменен (нет ставок).");
    }
    return;
  }

  int ret = QMessageBox::question(this, "Завершить аукцион",
                                  "Вы уверены, что хотите завершить аукцион?",
                                  QMessageBox::Yes | QMessageBox::No);
  if (ret == QMessageBox::Yes) {
    currentAuction->complete();

    // Создаем сделку с победителем
    createTransactionFromAuction();

    refreshAuctionInfo();
    addBidBtn->setEnabled(false);
    bidAmountSpin->setEnabled(false);
    clientCombo->setEnabled(false);
    completeAuctionBtn->setEnabled(false);

    QMessageBox::information(
        this, "Аукцион завершен",
        QString("Аукцион успешно завершен.\nСделка создана для клиента: %1")
            .arg(QString::fromStdString(winner->getClientName())));
  }
}

void AuctionDialog::refreshAuctionInfo() {
  if (!currentAuction)
    return;

  QString statusText = QString::fromStdString(currentAuction->getStatus());
  if (statusText == "active") {
    statusText = "Активен";
    statusLabel->setStyleSheet(
        "color: #6a9; font-size: 9pt; font-weight: bold;");
  } else if (statusText == "completed") {
    statusText = "Завершен";
    statusLabel->setStyleSheet(
        "color: #a66; font-size: 9pt; font-weight: bold;");
  } else {
    statusText = "Отменен";
    statusLabel->setStyleSheet(
        "color: #888; font-size: 9pt; font-weight: bold;");
  }
  statusLabel->setText("Статус: " + statusText);

  if (currentHighestBidLabel) {
    double highestBid = currentAuction->getCurrentHighestBid();
    if (highestBid > 0) {
      Bid *highest = currentAuction->getHighestBid();
      QString bidText =
          QString("Текущая максимальная ставка: %1 руб. (клиент: %2)")
              .arg(QString::number(highestBid, 'f', 2))
              .arg(QString::fromStdString(highest ? highest->getClientName()
                                                  : "неизвестно"));
      currentHighestBidLabel->setText(bidText);
    } else {
      currentHighestBidLabel->setText(
          "Текущая максимальная ставка: нет ставок");
    }

    // Обновляем минимальную ставку
    double minBid = (highestBid > 0) ? highestBid + 0.01
                                     : currentAuction->getStartingPrice();
    bidAmountSpin->setMinimum(minBid);
    if (bidAmountSpin->value() < minBid) {
      bidAmountSpin->setValue(minBid);
    }
  }

  // Обновляем цену автоматической покупки
  double buyoutPrice = currentAuction->getBuyoutPrice();
  QString buyoutText = QString("Цена автоматической покупки: %1 руб. (+70%%)")
                           .arg(QString::number(buyoutPrice, 'f', 2));
  buyoutPriceLabel->setText(buyoutText);
}

void AuctionDialog::validateAndAccept() {
  QString idText = idEdit->text();
  if (idText.isEmpty()) {
    QMessageBox::warning(this, "Ошибка", "ID не может быть пустым");
    return;
  }

  if (!Transaction::validateId(idText.toStdString())) {
    QMessageBox::warning(
        this, "Ошибка валидации",
        "ID должен содержать только цифры и быть длиной от 6 до 8 символов");
    return;
  }

  if (propertyCombo->currentIndex() < 0) {
    QMessageBox::warning(this, "Ошибка", "Выберите недвижимость");
    return;
  }

  double price = priceSpin->value();
  if (price <= 0) {
    QMessageBox::warning(this, "Ошибка",
                         "Начальная цена должна быть положительной");
    return;
  }

  accept();
}

QString AuctionDialog::getId() const { return idEdit->text(); }

QString AuctionDialog::getPropertyId() const {
  if (propertyCombo->currentIndex() >= 0 &&
      propertyCombo->currentIndex() < propertyIds.size()) {
    return propertyIds[propertyCombo->currentIndex()].split(" - ").first();
  }
  return "";
}

double AuctionDialog::getStartingPrice() const { return priceSpin->value(); }

void AuctionDialog::createTransactionFromAuction() {
  if (!currentAuction || !agency)
    return;

  // Получаем победителя аукциона
  Bid *winner = currentAuction->getHighestBid();
  if (!winner) {
    return; // Нет победителя, сделка не создается
  }

  // Проверяем, не создана ли уже сделка для этого аукциона
  auto existingTransactions =
      agency->getTransactionManager().getTransactionsByProperty(
          currentAuction->getPropertyId());
  for (Transaction *trans : existingTransactions) {
    if (trans->getClientId() == winner->getClientId() &&
        trans->getStatus() == "completed" &&
        trans->getFinalPrice() == winner->getAmount()) {
      // Сделка уже существует
      return;
    }
  }

  // Генерируем ID для транзакции (6-8 цифр, как требуется для Transaction)
  // Используем текущее время для уникальности
  std::string transactionId;
  {
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);
    std::ostringstream oss;
    // Формат: HHMMSS = 6 цифр
    oss << std::put_time(&tm, "%H%M%S");
    transactionId = oss.str();

    // Если нужно больше цифр, добавляем день месяца
    if (transactionId.length() < 8) {
      transactionId +=
          std::to_string(tm.tm_mday % 100); // Добавляем 2 цифры дня = 8 цифр
    }

    // Обеспечиваем, что ID состоит только из цифр и имеет длину 6-8
    if (transactionId.length() > 8) {
      transactionId = transactionId.substr(0, 8);
    }
    if (transactionId.length() < 6) {
      transactionId =
          transactionId + std::string(6 - transactionId.length(), '0');
    }
  }

  // Проверяем уникальность ID, если не уникален - добавляем суффикс
  int suffix = 1;
  std::string originalId = transactionId;
  while (agency->getTransactionManager().findTransaction(transactionId) !=
         nullptr) {
    // Добавляем суффикс, сохраняя длину 6-8 цифр
    if (transactionId.length() < 8) {
      transactionId = originalId + std::to_string(suffix % 10);
      if (transactionId.length() > 8)
        transactionId = transactionId.substr(0, 8);
    } else {
      // Если уже 8 цифр, заменяем последние 2 цифры
      transactionId = originalId.substr(0, 6) + std::to_string(suffix % 100);
    }
    suffix++;
    if (suffix > 999) {
      // Fallback: используем ID аукциона как основу (должен быть валидным)
      std::string baseId = currentAuction->getId();
      // Убеждаемся, что baseId состоит только из цифр
      std::string cleanId;
      for (char c : baseId) {
        if (std::isdigit(static_cast<unsigned char>(c)))
          cleanId += c;
      }
      if (cleanId.length() >= 6) {
        transactionId = cleanId.substr(0, 6);
      } else {
        transactionId = cleanId + std::string(6 - cleanId.length(), '0');
      }
      // Добавляем случайное число для уникальности
      auto now = std::time(nullptr);
      transactionId += std::to_string((now % 100));
      if (transactionId.length() > 8)
        transactionId = transactionId.substr(0, 8);
      suffix = 1;
    }
  }

  try {
    // Создаем транзакцию
    double finalPrice = winner->getAmount();
    std::string notes =
        "Продажа через аукцион. Аукцион ID: " + currentAuction->getId();

    auto transaction = std::make_shared<Transaction>(
        transactionId, currentAuction->getPropertyId(), winner->getClientId(),
        finalPrice,
        "completed", // Статус "завершена"
        notes);

    // Добавляем транзакцию
    agency->getTransactionManager().addTransaction(transaction);

    // Помечаем недвижимость как недоступную
    Property *prop = agency->getPropertyManager().findProperty(
        currentAuction->getPropertyId());
    if (prop) {
      prop->setAvailable(false);
    }

  } catch (const std::exception &e) {
    QMessageBox::warning(this, "Ошибка",
                         QString("Ошибка создания сделки: %1").arg(e.what()));
  }
}
