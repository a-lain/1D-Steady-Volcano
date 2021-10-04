QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Simulación/Koyaguchi.cpp \
    Simulación/analyticalKoyaguchi.cpp \
    Simulación/fundamentos.cpp \
    Simulación/progreso.cpp \
    Simulación/solveDiff.cpp \
    Simulación/volcanes.cpp \
    Simulación/útil.cpp \
    UI/graficaUI.cpp \
    UI/main.cpp \
    UI/multspinbox.cpp \
    UI/parametrosUI.cpp \
    UI/progresoUI.cpp \
    UI/resultadosUI.cpp \
    UI/seleccionGraficaUI.cpp

HEADERS += \
    Simulación/Koyaguchi.h \
    Simulación/analyticalKoyaguchi.h \
    Simulación/fundamentos.h \
    Simulación/progreso.h \
    Simulación/solveDiff.h \
    Simulación/volcanes.h \
    Simulación/útil.h \
    UI/graficaUI.h \
    UI/multspinbox.h \
    UI/parametrosUI.h \
    UI/progresoUI.h \
    UI/resultadosUI.h \
    UI/seleccionGraficaUI.h

FORMS += \
    UI/graficaUI.ui \
    UI/parametrosUI.ui \
    UI/progresoUI.ui \
    UI/resultadosUI.ui \
    UI/seleccionGraficaUI.ui

TRANSLATIONS += \
    VolcanesGUI_es_ES.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    UI/ImágenesUnidades.qrc


install_it.path = %{buildDir}
install_it.files += %{sourceDir}/graficador.py

INSTALLS += install_it
