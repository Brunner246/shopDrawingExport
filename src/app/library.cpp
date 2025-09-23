#include <filesystem>
#include <cwapi3d/CwAPI3D.h>

#include <QPointer>
#include <QMainWindow>

#include <utility>
#include <iostream>
#include <optional>
#include <ranges>
#include <algorithm>

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

CWAPI3D_PLUGIN bool plugin_x64_init(CwAPI3D::ControllerFactory *factory)
{
    qDebug() << "Hello from plugin_x64_init";

    // QPointer<MyDialog> dlg = new MyDialog();
    // dlg->setAttribute(Qt::WA_DeleteOnClose);
    //
    // HWND hwnd = factory->getUtilityController()->get3DHWND();
    // const auto lHostWindow = dynamic_cast<QMainWindow *>(QWidget::find(reinterpret_cast<WId>(hwnd)));
    // dlg->setParent(lHostWindow);
    // lHostWindow->addDockWidget(Qt::RightDockWidgetArea, dlg);
    // dlg->show();
    //
    //
    // // QObject::connect(dlg,
    // //                  &QDialog::accepted,
    // //                  [dlg]()
    // //                  {
    // //                      qDebug() << "User entered:" << (dlg ? dlg->text() : QString());
    // //                  });
    // dlg->show();
    //
    // return false;

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

        qDebug() << "Using presetting file: " << presettingPath.string();

        // factory->getUtilityController()->printToConsole(std::format(L"Using presetting file: {}\n",
        //     presettingPath.wstring()).c_str());

        factory->getShopDrawingController()->exportWallWithClipboardAndPresetting(8,
                                     selectedElementIDs,
                                     presettingPath.wstring().c_str());


        constexpr std::string_view iniDirNameNesting = "esz_nesting";
        const auto presettingPathNesting = std::filesystem::path(userProfilePath) / iniDirNameNesting / iniFileName;

        factory->getUtilityController()->printToConsole(std::format(L"Using presetting file: {}\n",
            presettingPath.wstring()).c_str());

        factory->getShopDrawingController()->exportWallWithClipboardAndPresetting(18,
                                     selectedElementIDs,
                                     presettingPathNesting.wstring().c_str());
    }
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
