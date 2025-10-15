#include <assert.h>
#include <filesystem>
#include <cwapi3d/CwAPI3D.h>

#include <utility>
#include <iostream>
#include <optional>
#include <ranges>
#include <set>
#include <thread>

enum class Action : uint8_t
{
    CREATE_DRAWING_ACTIVE_ELEMENTS = 1,
    CREATE_DRAWING_NESTING_PARENTS,
    EXIT,
};

std::optional<std::wstring> toString(const Action type)
{
    switch (type) {
    case Action::CREATE_DRAWING_ACTIVE_ELEMENTS: return L"draw active elements";
    case Action::CREATE_DRAWING_NESTING_PARENTS: return L"draw nesting parents";
    case Action::EXIT: return L"EXIT";
    }
    return std::nullopt;
}

Action buildAndQuerySelectedMenuAction(CwAPI3D::Interfaces::ICwAPI3DMenuController *menuController)
{
    CwAPI3D::Interfaces::ICwAPI3DMenu *menu = menuController->createMenu();
    menu->addButtonMenuItem(toString(Action::CREATE_DRAWING_ACTIVE_ELEMENTS)->data());
    menu->addButtonMenuItem(toString(Action::CREATE_DRAWING_NESTING_PARENTS)->data());
    menu->addButtonMenuItem(L"");
    menu->addButtonMenuItem(toString(Action::EXIT)->data());
    menuController->displayMenu(menu);

    const auto selectedItemIndex = menuController->getSelectedMenuItem();
    return static_cast<Action>(selectedItemIndex);
}

std::filesystem::path generatePresettingPathEszWall(CwAPI3D::UtilityController *utilityCtrl)
{
    const auto userProfilePath = utilityCtrl->get3DUserprofilPath()->narrowData();
    constexpr std::string_view iniDirName = "esz_wall";
    constexpr std::string_view iniFileName = "nesting.ini";

    return std::filesystem::path(userProfilePath) / iniDirName / iniFileName;
}

std::filesystem::path generatePresettingPathEszNesting(CwAPI3D::UtilityController *utilityCtrl)
{
    const auto userProfilePath = utilityCtrl->get3DUserprofilPath()->narrowData();
    constexpr std::string_view iniDirName = "esz_nesting";
    constexpr std::string_view iniFileName = "nesting.ini";

    return std::filesystem::path(userProfilePath) / iniDirName / iniFileName;
}

void exportShopDrawingWithClipboardAndPresetting(CwAPI3D::ControllerFactory *factory,
                                                 CwAPI3D::Interfaces::ICwAPI3DElementIDList *const selectedElementIDs,
                                                 const std::wstring &presettingPath,
                                                 const int clipBoardNr8)
{
    factory->getShopDrawingController()->exportWallWithClipboardAndPresetting(clipBoardNr8,
                                                                              selectedElementIDs,
                                                                              presettingPath.c_str());
}

CwAPI3D::Interfaces::ICwAPI3DElementIDList *getAllNestingParents(CwAPI3D::ControllerFactory *factory)
{
    const auto elementIDs = factory->getElementController()->getAllIdentifiableElementIDs();
    std::set<CwAPI3D::elementID> lIds;
    for (decltype(elementIDs->count()) il{0}; il < elementIDs->count(); ++il) {
        if (const auto nestingParent = factory->getElementController()->getNestingParentId(elementIDs->at(il));
            factory->getElementController()->checkElementId(nestingParent)) {
            lIds.insert(nestingParent);
        }
    }

    const auto result = factory->createEmptyElementIDList();
    for (const auto &id : lIds) {
        result->append(id);
    }

    return result;
}

CwAPI3D::Interfaces::ICwAPI3DElementIDList *getAllNestingRawParts(CwAPI3D::ControllerFactory *factory)
{
    return factory->getElementController()->getAllNestingRawParts();
}

CwAPI3D::Interfaces::ICwAPI3DElementIDList *getActiveElements(CwAPI3D::ControllerFactory *factory)
{
    return factory->getElementController()->getActiveIdentifiableElementIDs();
}

CWAPI3D_PLUGIN bool plugin_x64_init(CwAPI3D::ControllerFactory *factory)
{
    const auto action = buildAndQuerySelectedMenuAction(factory->getMenuController());
    if (action == Action::EXIT) {
        return false;
    }

    const auto elementIDs = action == Action::CREATE_DRAWING_NESTING_PARENTS
                                ? getAllNestingParents(factory)
                                : getActiveElements(factory);

    assert(!elementIDs->empty());
    factory->getUtilityController()->printMessage(std::format(L"Processing {} elements{}",
                                                              elementIDs->count(),
                                                              action == Action::CREATE_DRAWING_NESTING_PARENTS
                                                                  ? L" (nesting parents)"
                                                                  : L" (active elements)").c_str());

    const std::filesystem::path presettingPath = generatePresettingPathEszWall(factory->getUtilityController());

    constexpr int clipBoardNr8 = 8;
    exportShopDrawingWithClipboardAndPresetting(factory,
                                                elementIDs,
                                                presettingPath.wstring(),
                                                clipBoardNr8);

    const auto presettingPathNesting = generatePresettingPathEszNesting(factory->getUtilityController());

    constexpr int clipBoardNr18 = 18;
    exportShopDrawingWithClipboardAndPresetting(factory,
                                                elementIDs,
                                                presettingPathNesting.wstring(),
                                                clipBoardNr18
    );

    return true;
}
