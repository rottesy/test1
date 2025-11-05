#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <string>

namespace Utils {
// Конвертация QString в std::string
inline std::string toString(const QString &qstr) { return qstr.toStdString(); }

// Конвертация std::string в QString
inline QString toQString(const std::string &str) {
  return QString::fromStdString(str);
}

// Безопасное получение текста из QString
inline std::string safeToString(const QString &qstr) {
  return qstr.isEmpty() ? std::string() : qstr.toStdString();
}

// Форматирование цены
inline QString formatPrice(double price) {
  return QString::number(price, 'f', 2) + " руб.";
}

// Форматирование числа
inline QString formatNumber(double number, int decimals = 2) {
  return QString::number(number, 'f', decimals);
}

// Проверка, является ли строка числовым ID (6-8 цифр)
inline bool isNumericId(const QString &str) {
  if (str.length() < 6 || str.length() > 8) {
    return false;
  }
  for (const QChar &ch : str) {
    if (!ch.isDigit()) {
      return false;
    }
  }
  return true;
}

// Получить ID из строки вида "ID - Description"
inline QString extractIdFromDisplayString(const QString &displayString) {
  int separatorIndex = displayString.indexOf(" - ");
  if (separatorIndex > 0) {
    return displayString.left(separatorIndex);
  }
  return displayString;
}
} // namespace Utils

#endif // UTILS_H
