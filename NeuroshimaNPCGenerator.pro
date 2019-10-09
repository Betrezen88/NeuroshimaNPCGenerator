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
    NPC/Widgets/NPCAbstractSkillpackView.cpp \
    NPC/Widgets/NPCArmor.cpp \
    NPC/Widgets/NPCFeatureWidget.cpp \
    NPC/Widgets/NPCFriendCard.cpp \
    NPC/Widgets/NPCFriendCreator.cpp \
    NPC/Widgets/NPCFriendEquipment.cpp \
    NPC/Widgets/NPCFriendManager.cpp \
    NPC/Widgets/NPCFriendObverse.cpp \
    NPC/Widgets/NPCFriendStats.cpp \
    NPC/Widgets/NPCFriendView.cpp \
    NPC/Widgets/NPCGeneralSkillpack.cpp \
    NPC/Widgets/NPCInventory.cpp \
    NPC/Widgets/NPCItem.cpp \
    NPC/Widgets/NPCOtherSkills.cpp \
    NPC/Widgets/NPCOtherSkillsView.cpp \
    NPC/Widgets/NPCReputationView.cpp \
    NPC/Widgets/NPCShop.cpp \
    NPC/Widgets/NPCShopItem.cpp \
    NPC/Widgets/NPCSkillChoose.cpp \
    NPC/Widgets/NPCTrickManagerWidget.cpp \
    NPC/Widgets/NPCWeaponItem.cpp \
    NPC/Widgets/NPCWeaponView.cpp \
        main.cpp \
        MainWindow.cpp \
    NPC/Widgets/NPCAttributesModsInfoWidget.cpp \
    NPC/Widgets/NPCAttributeWidget.cpp \
    NPC/Widgets/NPCAttributeValueWidget.cpp \
    NPC/Widgets/NPCProgressWidget.cpp \
    NPC/Widgets/NPCTrickWidgetItem.cpp \
    NPC/Widgets/NPCTricksWidget.cpp \
    NPC/Widgets/SkillSpinBox.cpp \
    NPC/Widgets/DragDropAreaWidget.cpp \
    NPC/Utils/DataLoader.cpp \
    NPC/Utils/Dice.cpp \
    NPC/NPCCardObverse.cpp \
    NPC/NPCCardReverse.cpp \
    NPC/NPCCardTab.cpp \
    NPC/Utils/CardConverter.cpp \
    NPC/Dialogs/NPCCreatorDialog.cpp \
    NPC/Widgets/NPCAttributeManagerWidget.cpp \
    NPC/Widgets/NPCSpecializationManagerWidget.cpp \
    NPC/Widgets/NPCOriginManagerWidget.cpp \
    NPC/Widgets/NPCProfessionManagerWidget.cpp \
    NPC/Widgets/NPCSicknessManagerWidget.cpp \
    NPC/Widgets/NPCSkillsManagerWidget.cpp \
    NPC/Widgets/NPCBioManagerWidget.cpp \
    NPC/Widgets/NPCAttributeView.cpp \
    NPC/Widgets/NPCSkillpackView.cpp \
    NPC/Widgets/NPCReputationManagerWidget.cpp \
    NPC/Widgets/ReputationValueBox.cpp \
    NPC/Widgets/NPCCustomSkillPack.cpp \
    NPC/Widgets/NPCAbstractSkillPack.cpp \
    NPC/Widgets/NPCSkillPack.cpp \
    NPC/Widgets/NPCAttributeResultsRow.cpp

HEADERS += \
        MainWindow.h \
    NPC/Widgets/NPCAbstractSkillpackView.h \
    NPC/Widgets/NPCArmor.h \
    NPC/Widgets/NPCAttributesModsInfoWidget.h \
    NPC/Widgets/NPCAttributeWidget.h \
    NPC/Widgets/NPCAttributeValueWidget.h \
    NPC/Widgets/NPCFeatureWidget.h \
    NPC/Widgets/NPCFriendCard.h \
    NPC/Widgets/NPCFriendCreator.h \
    NPC/Widgets/NPCFriendEquipment.h \
    NPC/Widgets/NPCFriendManager.h \
    NPC/Widgets/NPCFriendObverse.h \
    NPC/Widgets/NPCFriendStats.h \
    NPC/Widgets/NPCFriendView.h \
    NPC/Widgets/NPCGeneralSkillpack.h \
    NPC/Widgets/NPCInventory.h \
    NPC/Widgets/NPCItem.h \
    NPC/Widgets/NPCOtherSkills.h \
    NPC/Widgets/NPCOtherSkillsView.h \
    NPC/Widgets/NPCProgressWidget.h \
    NPC/Widgets/NPCReputationView.h \
    NPC/Widgets/NPCShop.h \
    NPC/Widgets/NPCShopItem.h \
    NPC/Widgets/NPCSkillChoose.h \
    NPC/Widgets/NPCTrickManagerWidget.h \
    NPC/Widgets/NPCTricksWidget.h \
    NPC/Widgets/NPCTrickWidgetItem.h \
    NPC/Widgets/NPCWeaponItem.h \
    NPC/Widgets/NPCWeaponView.h \
    NPC/Widgets/SkillSpinBox.h \
    NPC/Widgets/DragDropAreaWidget.h \
    NPC/Utils/DataLoader.h \
    NPC/Utils/Dice.h \
    NPC/NPCCardObverse.h \
    NPC/NPCCardReverse.h \
    NPC/NPCCardTab.h \
    NPC/Utils/CardConverter.h \
    NPC/Dialogs/NPCCreatorDialog.h \
    NPC/Widgets/NPCAttributeManagerWidget.h \
    NPC/Widgets/NPCSpecializationManagerWidget.h \
    NPC/Widgets/NPCOriginManagerWidget.h \
    NPC/Widgets/NPCProfessionManagerWidget.h \
    NPC/Widgets/NPCSicknessManagerWidget.h \
    NPC/Widgets/NPCSkillsManagerWidget.h \
    NPC/Widgets/NPCBioManagerWidget.h \
    NPC/Widgets/NPCAttributeView.h \
    NPC/Widgets/NPCSkillpackView.h \
    NPC/Widgets/NPCReputationManagerWidget.h \
    NPC/Widgets/ReputationValueBox.h \
    NPC/Widgets/NPCCustomSkillPack.h \
    NPC/Widgets/NPCAbstractSkillPack.h \
    NPC/Widgets/NPCSkillPack.h \
    NPC/Widgets/NPCCustomWidget.h \
    NPC/Widgets/NPCAttributeResultsRow.h

RESOURCES += \
    Resources/resources.qrc

DISTFILES += \
    Othres/Concept.txt
