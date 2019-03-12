#-------------------------------------------------
#
# Project created by QtCreator 2018-06-15T20:56:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NeuroshimaNPCGenerator
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    NPC/Widgets/NPCTrickManagerWidget.cpp \
        main.cpp \
        MainWindow.cpp \
    NPC/NPCCard.cpp \
    NPC/Widgets/NPCAttributesModsInfoWidget.cpp \
    NPC/Widgets/NPCAttributeWidget.cpp \
    NPC/Widgets/NPCAttributeValueWidget.cpp \
    NPC/Widgets/NPCSkillPackWidget.cpp \
    NPC/Widgets/NPCProgressWidget.cpp \
    NPC/Widgets/NPCTrickWidgetItem.cpp \
    NPC/Widgets/NPCTricksWidget.cpp \
    NPC/Widgets/SkillSpinBox.cpp \
    NPC/Widgets/DragDropAreaWidget.cpp \
    NPC/Utils/DataLoader.cpp \
    NPC/Utils/Dice.cpp \
    NPC/Dialogs/NPCTrickManagerDialog.cpp \
    NPC/Dialogs/NPCAttributeManagerDialog.cpp \
    NPC/Dialogs/NPCSkicnessDialog.cpp \
    NPC/NPCCardObverse.cpp \
    NPC/NPCCardReverse.cpp \
    NPC/NPCCardTab.cpp \
    NPC/Utils/CardConverter.cpp \
    NPC/Dialogs/NPCOriginManagerDialog.cpp \
    NPC/Dialogs/NPCCreatorDialog.cpp \
    NPC/Widgets/NPCAttributeManagerWidget.cpp \
    NPC/Widgets/NPCSpecializationManagerWidget.cpp \
    NPC/Widgets/NPCOriginManagerWidget.cpp \
    NPC/Widgets/NPCProfessionManagerWidget.cpp \
    NPC/Widgets/NPCSicknessManagerWidget.cpp \
    NPC/Widgets/NPCSkillsManagerWidget.cpp \
    NPC/Widgets/NPCBioManagerWidget.cpp \
    NPC/Widgets/NPCAttributeView.cpp \
    NPC/Widgets/NPCSkillpackView.cpp

HEADERS += \
        MainWindow.h \
    NPC/NPCCard.h \
    NPC/Widgets/NPCAttributesModsInfoWidget.h \
    NPC/Widgets/NPCAttributeWidget.h \
    NPC/Widgets/NPCAttributeValueWidget.h \
    NPC/Widgets/NPCSkillPackWidget.h \
    NPC/Widgets/NPCProgressWidget.h \
    NPC/Widgets/NPCTrickManagerWidget.h \
    NPC/Widgets/NPCTricksWidget.h \
    NPC/Widgets/NPCTrickWidgetItem.h \
    NPC/Widgets/SkillSpinBox.h \
    NPC/Widgets/DragDropAreaWidget.h \
    NPC/Utils/DataLoader.h \
    NPC/Utils/Dice.h \
    NPC/Dialogs/NPCTrickManagerDialog.h \
    NPC/Dialogs/NPCAttributeManagerDialog.h \
    NPC/Dialogs/NPCSkicnessDialog.h \
    NPC/NPCCardObverse.h \
    NPC/NPCCardReverse.h \
    NPC/NPCCardTab.h \
    NPC/Utils/CardConverter.h \
    NPC/Dialogs/NPCOriginManagerDialog.h \
    NPC/Dialogs/NPCCreatorDialog.h \
    NPC/Widgets/NPCAttributeManagerWidget.h \
    NPC/Widgets/NPCSpecializationManagerWidget.h \
    NPC/Widgets/NPCOriginManagerWidget.h \
    NPC/Widgets/NPCProfessionManagerWidget.h \
    NPC/Widgets/NPCSicknessManagerWidget.h \
    NPC/Widgets/NPCSkillsManagerWidget.h \
    NPC/Widgets/NPCBioManagerWidget.h \
    NPC/Widgets/NPCAttributeView.h \
    NPC/Widgets/NPCSkillpackView.h

RESOURCES += \
    Resources/resources.qrc

DISTFILES += \
    Othres/Concept.txt \
    json/Origins.json \
    json/Attributes.json \
    json/Specializations.json \
    json/Professions.json \
    json/Sickness.json \
    json/Tricks.json \
    json/DifficultyLevel.json \
    json/Questions.json \
    json/Reputation.json
