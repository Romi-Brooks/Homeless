#include "Window/MainWindow.hpp"

auto main() -> int {
	MainWindow window(1920, 1080, "SFML + ImGui Test");
	window.Run();

	return 0;
}
