#include "device/application.hpp"

Application::Application(string name):
    app_name(name)
{
    device()->Register_application(this);
}
