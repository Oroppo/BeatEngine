
#include <GLFW/glfw3.h>
#include "Gameplay/GameObject.h"
#include "Gameplay/Scene.h"
#include "Utils/ImGuiHelper.h"
#include "InteractableMenu.h"
#include "Gameplay/InputEngine.h"
#include "Gameplay/Components/GUI/GuiPanel.h"
#include "Application/Application.h"

// Constructor Initializes Values for LERP and Set Position but Only SetPosition is being used atm
InteractableMenu::InteractableMenu(int MenuIndex) : IComponent()
{
    _MenuIndex = MenuIndex;
}

InteractableMenu::~InteractableMenu() = default;

//Move this from Awake if you want to dynamically add Menu Items
void InteractableMenu::Awake()
{
    //Every Child is a Menu Item IN THE ORDER THEY WERE MADE CHILDREN!
    for (auto& child : GetGameObject()->GetChildren()) {
        _MenuItems.push_back(child);
    }

}



nlohmann::json InteractableMenu::ToJson() const {
    return {
        {
            "_MenuIndex",_MenuIndex
        }
    };
}

std::vector<Gameplay::GameObject::Sptr> InteractableMenu::GetMenuItems() {
    return _MenuItems;
}

InteractableMenu::Sptr InteractableMenu::FromJson(const nlohmann::json & blob) {
    InteractableMenu::Sptr result = std::make_shared<InteractableMenu>();
    result->_MenuIndex = blob["_MenuIndex"];
    return result;
}
void InteractableMenu::RenderImGui() {

}
void InteractableMenu::HandleSelection() {


}

void InteractableMenu::Update(float deltaTime) {
    //Check if it's above a given element by grabbing element's position from the list
    //if( (mousePosition >= position - size/2) && (mousePosition <= position + size/2))
    //for (auto& element : _MenuItems) {
        //      Do the thing     
    //}


    if (InputEngine::GetKeyState(GLFW_KEY_LEFT)==ButtonState::Pressed) {
        _selection-=1;
        if (_selection < 0)
            _selection = _MenuItems.size() - 1;
    }
    if (InputEngine::GetKeyState(GLFW_KEY_RIGHT)==ButtonState::Pressed) {
        _selection += 1;
        if (_selection > _MenuItems.size() - 1)
            _selection = 0;
    }

    for (int i = 0; i < _MenuItems.size();i++) {
        if (i != _selection)
            _MenuItems[i]->Get<GuiPanel>()->SetColor({ 1,1,1,0.6 });
        else
            _MenuItems[i]->Get<GuiPanel>()->SetColor({ 1,1,1,1 });      
    }

    Application& app = Application::Get();

    //These DON'T all have to Load a scene! they could simply Enable/Disable Menu Items! 
    //this will be our approach for future implementation

    //MAIN MENU
    if ((InputEngine::GetKeyState(GLFW_KEY_ENTER) == ButtonState::Pressed) && (_MenuIndex == 0)) {
        switch (_selection) {
        case 0:
            app.LoadScene("Level1.json");
            break;
        case 1:
            app.LoadScene("ControlsMenu.json");
            break;
        case 2:
            app.LoadScene("CreditsMenu.json");
            break;
        case 3:
            app.Quit();
            break;
        default:
            LOG_WARN("Menu Items out of Selection Range!");
            break;
        }
    }
    // GAME OVER MENU
    if ((InputEngine::GetKeyState(GLFW_KEY_ENTER) == ButtonState::Pressed) && (_MenuIndex == 1)) {
        switch (_selection) {
        case 0:
            app.LoadScene("MainMenu.json");
            break;
        case 1:
            app.Quit();
            break;
        default:
            LOG_WARN("Menu Items out of Selection Range!");
            break;
        }
    }

    // CONTROLS SUB MENU
    if ((InputEngine::GetKeyState(GLFW_KEY_ENTER) == ButtonState::Pressed) && (_MenuIndex == 2)) {
        switch (_selection) {
        case 0:
            app.LoadScene("MainMenu.json");
            break;
        default:
            LOG_WARN("Menu Items out of Selection Range!");
            break;
        }
    }

    // CREDITS SUB MENU
    if ((InputEngine::GetKeyState(GLFW_KEY_ENTER) == ButtonState::Pressed) && (_MenuIndex == 3)) {
        switch (_selection) {
        case 0:
            app.LoadScene("MainMenu.json");
            break;
        default:
            LOG_WARN("Menu Items out of Selection Range!");
            break;
        }
    }


}