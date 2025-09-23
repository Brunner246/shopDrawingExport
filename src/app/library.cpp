#include <filesystem>
#include <cwapi3d/CwAPI3D.h>

#include <QPointer>
#include <QMainWindow>
#include <QDebug>

#include <utility>
#include <iostream>
#include <optional>
#include <ranges>
#include <algorithm>
#include <thread>

#include "MyDialog.hh"

enum class Action : uint8_t
{
    CREATE_DRAWING = 1,
    EXIT,
};
std::optional<std::wstring> toString(const Action type)
{
    switch (type) {
    case Action::CREATE_DRAWING: return L"CREATE_DRAWING";
    case Action::EXIT: return L"EXIT";
    }
    return std::nullopt;
}

Action buildAndQuerySelectedMenuAction(CwAPI3D::Interfaces::ICwAPI3DMenuController *menuController)
{
    CwAPI3D::Interfaces::ICwAPI3DMenu *menu = menuController->createMenu();
    menu->addButtonMenuItem(toString(Action::CREATE_DRAWING)->data());
    menu->addButtonMenuItem(L"");
    menu->addButtonMenuItem(toString(Action::EXIT)->data());
    menuController->displayMenu(menu);

    const auto selectedItemIndex = menuController->getSelectedMenuItem();
    return static_cast<Action>(selectedItemIndex);
}

bool temp_clipboards_exist()
{
    namespace fs = std::filesystem;
    const fs::path temp = fs::temp_directory_path();

    const fs::path f8 = temp / "clipboard_008.2dc";
    const fs::path f18 = temp / "clipboard_018.2dc";

    const bool e8 = fs::exists(f8);
    const bool e18 = fs::exists(f18);

    qDebug() << "clipboard_008.2dc: " << (e8 ? "exists" : "missing");
    qDebug() << "clipboard_018.2dc: " << (e18 ? "exists" : "missing");

    return e8 && e18;
}

CWAPI3D_PLUGIN bool plugin_x64_init(CwAPI3D::ControllerFactory *factory)
{
    qDebug() << "Hello from plugin_x64_init";

    const auto action = buildAndQuerySelectedMenuAction(factory->getMenuController());
    if (action == Action::EXIT) {
        return false;
    }
    if (action == Action::CREATE_DRAWING) {
        const auto selectedElementIDs = factory->getElementController()->getActiveIdentifiableElementIDs();

        const auto userProfilePath = factory->getUtilityController()->get3DUserprofilPath()->narrowData();
        constexpr std::string_view iniDirName = "esz_wall";
        constexpr std::string_view iniFileName = "nesting.ini";
        const auto presettingPath = std::filesystem::path(userProfilePath) / iniDirName / iniFileName;

        qDebug() << "Using presetting file: " << QString::fromStdString(presettingPath.string());

        constexpr int clipBoardNr8 = 8;
        qDebug() << "Start exporting wall with clipboard nr: " << clipBoardNr8;
        factory->getShopDrawingController()->exportWallWithClipboardAndPresetting(clipBoardNr8,
            selectedElementIDs,
            presettingPath.wstring().c_str());
        qDebug() << "End exporting wall with clipboard nr: " << clipBoardNr8;


        constexpr std::string_view iniDirNameNesting = "esz_nesting";
        const auto presettingPathNesting = std::filesystem::path(userProfilePath) / iniDirNameNesting / iniFileName;

        qDebug() << "Using presetting file: " << QString::fromStdString(presettingPath.string());

        constexpr int clipBoardNr18 = 18;
        qDebug() << "Start exporting wall with clipboard nr: " << clipBoardNr18;
        factory->getShopDrawingController()->exportWallWithClipboardAndPresetting(clipBoardNr18,
            selectedElementIDs,
            presettingPathNesting.wstring().c_str());
        qDebug() << "End exporting wall with clipboard nr: " << clipBoardNr18;
    }

    std::this_thread::sleep_for(std::chrono::seconds(2));
    if (!temp_clipboards_exist()) {
        qWarning() << "Temp clipboards do not exist!";
        factory->getUtilityController()->printMessage(L"Failed to create shop drawings. Please check the log.");
    }
    factory->getUtilityController()->printMessage(L"Shop drawings created successfully.");
    return true;
}

// std::vector<CwAPI3D::elementID> elementIDs;
// elementIDs.reserve(selectedElementIDs->count());
// for (decltype(selectedElementIDs->count()) i = 0; i < selectedElementIDs->count(); ++i) {
//     elementIDs.push_back(selectedElementIDs->at(i));
// }

// std::array<int, 3> testIndices = {1, 2, 3};
// std::ranges::for_each(testIndices,
//                       [factory, selectedElementIDs, presettingPath](const int i)
//                       {
//                           factory->getShopDrawingController()->exportWallWithClipboardAndPresetting(i,
//                               selectedElementIDs,
//                               presettingPath.wstring().c_str());
//                       });

// const auto mid = static_cast<int>(std::ceil(selectedElementIDs->count() / 2));
// auto firstHalf = elementIDs | std::views::take(mid);
// auto secondHalf = elementIDs | std::views::drop(mid);
//
// std::vector<CwAPI3D::elementID> firstHalfVec(firstHalf.begin(), firstHalf.end());
// std::vector<CwAPI3D::elementID> secondHalfVec(secondHalf.begin(), secondHalf.end());

// {
//     const auto extendedSettings = factory->createEmptyExtendedSettings();
//     extendedSettings->setComposite(true);
//     extendedSettings->setSinglePiece(true);
//
//     const auto selectedElementIDs = factory->getElementController()->getActiveIdentifiableElementIDs();
//     factory->getAttributeController()->setExtendedSettings(selectedElementIDs, extendedSettings);
// }
