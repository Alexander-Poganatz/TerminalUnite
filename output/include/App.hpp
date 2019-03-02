#ifndef APP_HPP
#define APP_HPP

#include <vector>
#include <Panel.hpp>

int main(int argc, char* argv[]);
namespace ca_poganatz
{
	class App 
	{
	private:
		static App* appInstance;
		friend int ::main(int argc, char* argv[]);
		static int Main(int argc, char* argv[]);
		std::vector<std::string> arguments;
		std::vector<Panel*> panels;
		bool defaultRunFlag;
		std::vector<InputEvent> inputBuffer;
		Panel* focusedPanel;

		void sortPanelsForPainting();
		void sortPanelsForInput();
	public:
		App();
		virtual ~App() {};

		inline std::vector<std::string> const& getProgramArguments() { return arguments; }

		static inline App& getInstance() { return *appInstance; }

		static inline bool GetDefaultRunFlag() { return getInstance().defaultRunFlag; }

		static inline void SetDefaultRunFlagToFalse() { getInstance().defaultRunFlag = false; }

		virtual int Execute();

		void ReadAndHandleConsoleInput();

		inline void AddPanel(Panel* panel) { panels.push_back(panel); }

		void PaintPanels();

		void AddDefaultCtrlHandler();

		static inline void SetFocusedPanel(Panel* panel) { getInstance().focusedPanel = panel; };
	};
}

#endif