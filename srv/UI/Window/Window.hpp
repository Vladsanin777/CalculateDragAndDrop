#include <QMainWindow>
#include "../Application/Aplication.cpp"
#include "../Setting/Setting.cpp"
#include "../CreateHistory/CreateHistory.cpp"


namespace Window {
	class Window;
	class Window : public QMainWindow {
	private:
		RGB::RGB _rgb0 {0, 0, 0}, _rgb1{0, 0, 0};
		MODS _currentMod {BASIC};
		byte _currentIndex {byte(0)};
		Setting::SettingDock * _settingDock {nullptr};
		LineEdit::LineEdit * _lineEditLongArifmetic {nullptr};
		CreateHistori::HistoriScroll ** const _localHistori \
			{new CreateHistori::HistoriScroll*[COUNT_LOCAL_HISTORI]{nullptr}};
		CreateHistori::HistoriWidget ** const _resizeLocalHistori \
			{new CreateHistori::HistoriWidget*[COUNT_LOCAL_HISTORI]{nullptr}};
		CreateHistori::HistoriVBox ** const _addLocalHistori \
			{new CreateHistori::HistoriVBox*[COUNT_LOCAL_HISTORI]{nullptr}};
		LineEdit::LineEdit ** const * const _lineEdit { \
			new LineEdit::LineEdit ** const [COUNT_LOCAL_HISTORI] { \
				new LineEdit::LineEdit * [1]{nullptr}, \
				new LineEdit::LineEdit * [1]{nullptr}, \
				new LineEdit::LineEdit * [1]{nullptr}, \
				new LineEdit::LineEdit * [3]{nullptr, \
					nullptr, nullptr}, \
				new LineEdit::LineEdit * [3]{nullptr, \
					nullptr, nullptr} \
			}
		};
		const char ** const * const _result { \
			new const char ** const [5]{ \
				{new const char * [1]{new const char [] {"0"}}}, \
				{new const char * [1]{new const char [] {"0"}}}, \
				{new const char * [1]{new const char [] {"0"}}}, \
				{new const char * [3]{new const char [] {"1"}, \
					new const char [] {"2"}, new const char [] {"0"}}}, \
				{new const char * [3]{new const char [] {"x"}, \
					new const char [] {"0"}, new const char [] {"0"}}} \
			} \
		};
		Button::ButtonDrag *_resultButton {nullptr};
		CreateHistori::HistoriScroll *_globalHistori {nullptr};
		CreateHistori::HistoriVBox *_addGlobalHistori {nullptr};
		CreateHistori::HistoriWidget *_resizeGlobalHistori {nullptr};
		Application::CalculateDragAndDrop *_app {nullptr};
		//Setting *_setting {nullptr};
	public:
		explicit inline Window( \
			Application::CalculateDragAndDrop *app = nullptr \
		);
		inline CreateHistori::HistoriScroll* getGlobalHistori( \
			void) const noexcept;
		inline void setGlobalHistori( \
			CreateHistori::HistoriScroll* newGlobalHistori \
		) noexcept;
		inline CreateHistori::HistoriWidget* getResizeGlobalHistori( \
			void) const noexcept;
		inline void setResizeGlobalHistori( \
			CreateHistori::HistoriWidget* newResizeGlobalHistori \
		) noexcept;
		inline CreateHistori::HistoriVBox* getAddGlobalHistori( \
			void) const noexcept;
		inline void setAddGlobalHistori( \
			CreateHistori::HistoriVBox* newAddGlobalHistori \
		) noexcept;
		inline CreateHistori::HistoriScroll* getLocalHistori( \
			MODS mod) const noexcept;
		inline CreateHistori::HistoriScroll* getLocalHistori( \
			void) const noexcept;
		inline void setLocalHistori( \
		CreateHistori::HistoriScroll* newLocalHistori, \
			MODS mod) noexcept;
		inline CreateHistori::HistoriWidget* getResizeLocalHistori( \
			MODS mod) const noexcept;
		inline CreateHistori::HistoriWidget* getResizeLocalHistori( \
			void) const noexcept;
		inline void setResizeLocalHistori( \
		CreateHistori::HistoriWidget* newResizeLocalHistori, \
			MODS mod) noexcept;
		inline CreateHistori::HistoriVBox* getAddLocalHistori( \
			MODS mod) const noexcept;
		inline CreateHistori::HistoriVBox* getAddLocalHistori( \
			void) const noexcept;
		inline void setAddLocalHistori( \
			CreateHistori::HistoriVBox* newAddLocalHistori, \
			MODS mod) noexcept;
		inline LineEdit::LineEdit* getLineEdit( \
			MODS mod, byte index) const noexcept;
		inline LineEdit::LineEdit* getLineEdit(void \
		) const noexcept;
		inline void setLineEdit(LineEdit::LineEdit* newLineEdit, \
			MODS mod, byte index) noexcept;
		inline MODS getMod(void) const noexcept;
		inline void setMod(const MODS mod) noexcept;
		inline byte getIndex(void) const noexcept;
		inline void setIndex(const byte index) noexcept;
		inline Button::ButtonDrag* getResultButton() noexcept;
		inline void setResultButton( \
			Button::ButtonDrag* resultButton) noexcept;
		inline const char *getResult( \
			MODS mod, byte index) const noexcept;
		inline const char *getResult(void) const noexcept;
		inline void setResult( \
			const char *newResult, MODS mod, byte index \
		) noexcept;
		inline void setResult(const char *newResult \
		) noexcept;
		template<typename TDel>
		inline void deleteResultOrLineEdit(TDel ptr \
		) const noexcept;
		inline void updataResultButton(void) const noexcept;
		inline void updataResultButton(MODS mod, byte index \
		) const noexcept;
		inline void setLineEditLongArifmetic( \
			LineEdit::LineEdit * lineEdit) noexcept;
		inline LineEdit::LineEdit * getLineEditLongArifmetic( \
			void) const noexcept;
		inline void setSettingDock( \
			Setting::SettingDock * settingDock) noexcept;
		inline Setting::SettingDock * getSettingDock( \
			void) const noexcept;
		inline void setRGB( \
			byte red0, byte green0, byte blue0, \
			byte red1, byte green1, byte blue1 \
		) noexcept;
		inline void setRGB( \
			RGB::RGB rgb0, RGB::RGB rgb1 \
		) noexcept;
		inline const RGB::RGB& getRGB0(void) noexcept;
		inline const RGB::RGB& getRGB1(void) noexcept;
		inline void setFuncRGB0(std::function<void(byte, \
			byte, byte)> func) noexcept;
		inline void setFuncRGB1(std::function<void(byte, \
			byte, byte)> func) noexcept;
	protected:
		inline void paintEvent(QPaintEvent *event) override;
	private:
		inline ~Window(void);
	};
}