#include "../include/MainWindow.h"
#include <QApplication>

namespace {
constexpr const char *APPLICATION_STYLE = "Fusion";
}

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  app.setStyle(APPLICATION_STYLE);

  MainWindow window;
  window.show();

  return app.exec();
}
