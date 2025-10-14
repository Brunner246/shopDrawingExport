#include <filesystem>
#include <cwapi3d/CwAPI3D.h>


#include <utility>
#include <iostream>
#include <optional>
#include <ranges>
#include <thread>

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

CWAPI3D_PLUGIN bool plugin_x64_init(CwAPI3D::ControllerFactory *factory)
{
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

        std::cout << "Using presetting file: " << presettingPath.string() << std::endl;

        constexpr int clipBoardNr8 = 8;
        std::cout << "Start exporting wall with clipboard nr: " << clipBoardNr8;
        factory->getShopDrawingController()->exportWallWithClipboardAndPresetting(clipBoardNr8,
            selectedElementIDs,
            presettingPath.wstring().c_str());
        std::cout << "End exporting wall with clipboard nr: " << clipBoardNr8;


        constexpr std::string_view iniDirNameNesting = "esz_nesting";
        const auto presettingPathNesting = std::filesystem::path(userProfilePath) / iniDirNameNesting / iniFileName;

        std::cout << "Using presetting file: " << (presettingPath.string());

        constexpr int clipBoardNr18 = 18;
        std::cout << "Start exporting wall with clipboard nr: " << clipBoardNr18;
        factory->getShopDrawingController()->exportWallWithClipboardAndPresetting(clipBoardNr18,
            selectedElementIDs,
            presettingPathNesting.wstring().c_str());
        std::cout << "End exporting wall with clipboard nr: " << clipBoardNr18;
    }
    return true;
}
