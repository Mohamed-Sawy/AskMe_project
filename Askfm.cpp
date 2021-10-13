#include "system_manager.h"

int main() {
    SystemManager& askMe_manager{ SystemManager::get_manager() };

    askMe_manager.start();

    return 0;
}