
#pragma once
#include "UI.hpp"

namespace Window {
	inline Window::Window( \
		Application::CalculateDragAndDrop \
		*app) : QMainWindow{}, _app{app} , \
		_rgb0{8, 116, 235}, _rgb1{17, 61, 138} {
		setWindowTitle(::windowTitle);
		resize(400, 800);
		setObjectName("Window");
		setCentralWidget(new MainWidget::MainWidget{_app, this});
		Setting::SettingDock * settingDock \
			{new Setting::SettingDock{this}};
		_settingDock = settingDock;
		addDockWidget(Qt::LeftDockWidgetArea, \
			settingDock);
		// Закрытие окна по Ctrl+Shift+W
		new QShortcut{QKeySequence("Ctrl+Shift+W"), \
			this, SLOT(close())};

		//centralWidget()->setAutoFillBackground(true);
		//setAutoFillBackground(true);  // Для QMainWindow
		//setAttribute(Qt::WA_StyledBackground, true);
		show();
		return;
	}

	inline CreateHistori::HistoriScroll* \
		Window::getGlobalHistori( \
	) const noexcept {
		return _globalHistori;
	}
	inline void Window::setGlobalHistori( \
		CreateHistori::HistoriScroll* newGlobalHistori \
	) noexcept {
		_globalHistori = newGlobalHistori;
		return;
	}	
	inline CreateHistori::HistoriWidget* \
	Window::getResizeGlobalHistori( \
	) const noexcept {
		return _resizeGlobalHistori;
	}
	inline void Window::setResizeGlobalHistori( \
		CreateHistori::HistoriWidget* newResizeGlobalHistori \
	) noexcept {
		_resizeGlobalHistori = newResizeGlobalHistori;
		return;
	}

	inline CreateHistori::HistoriVBox* \
	Window::getAddGlobalHistori(void) const noexcept {
		return _addGlobalHistori;
	}
	inline void Window::setAddGlobalHistori( \
		CreateHistori::HistoriVBox* newAddGlobalHistori \
	) noexcept {
		_addGlobalHistori = newAddGlobalHistori;
		return;
	}
	inline CreateHistori::HistoriScroll* \
	Window::getLocalHistori(MODS mod) const noexcept {
		return _localHistori[mod];
	}
	inline CreateHistori::HistoriScroll* \
	Window::getLocalHistori(void) const noexcept {
		return _localHistori[_currentMod];
	}
	inline void Window::setLocalHistori( \
		CreateHistori::HistoriScroll* newLocalHistori, MODS mod \
	) noexcept {
		_localHistori[mod] = newLocalHistori;
		return;
	}	
	inline CreateHistori::HistoriWidget* \
	Window::getResizeLocalHistori(MODS mod) const noexcept {
		return _resizeLocalHistori[mod];
	}
	inline CreateHistori::HistoriWidget* \
	Window::getResizeLocalHistori(void) const noexcept {
		return _resizeLocalHistori[_currentMod];
	}
	inline void Window::setResizeLocalHistori( \
		CreateHistori::HistoriWidget* newResizeLocalHistori, \
		MODS mod) noexcept {
		_resizeLocalHistori[mod] = newResizeLocalHistori;
		return;
	}

	inline CreateHistori::HistoriVBox* \
	Window::getAddLocalHistori(MODS mod) const noexcept {
		return _addLocalHistori[mod];
	}
	inline CreateHistori::HistoriVBox* \
	Window::getAddLocalHistori(void) const noexcept {
		return _addLocalHistori[_currentMod];
	}
	inline void Window::setAddLocalHistori( \
		CreateHistori::HistoriVBox* newAddLocalHistori, \
		MODS mod) noexcept {
		_addLocalHistori[mod] = newAddLocalHistori;
		return;
	}
	inline LineEdit::LineEdit* Window::getLineEdit( \
		MODS mod, byte index \
	) const noexcept {
		return _lineEdit[mod][index];
	}
	inline LineEdit::LineEdit* Window::getLineEdit(void \
	) const noexcept {
		return _lineEdit[_currentMod][_currentIndex];
	}
	inline void Window::setLineEdit( \
		LineEdit::LineEdit* newLineEdit, MODS mod, byte index \
	) noexcept {
		_lineEdit[mod][index] = newLineEdit;
		return;
	}

	inline MODS Window::getMod(void) const noexcept {
		return _currentMod;
	}
	inline void Window::setMod(const MODS mod) noexcept {
		_currentMod = mod;
		return;
	}
	inline byte Window::getIndex(void) const noexcept {
		return _currentIndex;
	}
	inline void Window::setIndex(const byte index) noexcept {
		_currentIndex = index;
		return;
	}
	inline Button::ButtonDrag* Window::getResultButton( \
		void) noexcept {
		return _resultButton;
	}
	inline void Window::setResultButton( \
		Button::ButtonDrag* resultButton \
	) noexcept {
		_resultButton = resultButton;
		return;
	}
	inline const char *Window::getResult( \
		MODS mod, byte index \
	) const noexcept {
		return _result[mod][index];
	}
	inline const char *Window::getResult(void) const noexcept {
		return _result[_currentMod][_currentIndex];
	}
	inline void Window::setResult(const char *newResult,
		MODS mod, byte index) noexcept {
		const char * & res {_result[mod][index]};
		delete[] res;
		res = newResult;
		updataResultButton(mod, index);
		return;
	}
	inline void Window::setResult( \
		const char *newResult) noexcept {
		const char * & res {_result[_currentMod][_currentIndex]};
		delete [] res;
		res = newResult;
		updataResultButton();
		return;
	}
	template<typename TDel>
	inline void Window::deleteResultOrLineEdit(TDel ptr) \
	const noexcept {
		delete **ptr;
		delete *ptr[1];
		delete *ptr[2];
		delete *ptr[3];
		delete ptr[3][1];
		delete ptr[3][2];
		delete *ptr[4];
		delete ptr[4][1];
		delete ptr[4][2];
		return;
	}
	inline void Window::updataResultButton(void) const noexcept {
		_resultButton->setText(_result[_currentMod][_currentIndex]);
	}
	inline void Window::updataResultButton(MODS mod, byte index) \
	const noexcept {
		puts(_result[mod][index]);
		_resultButton->setText(_result[mod][index]);
	}
	inline void Window::setLineEditLongArifmetic( \
		LineEdit::LineEdit * lineEdit \
	) noexcept {
		_lineEditLongArifmetic = lineEdit;
		return;
	}
	inline LineEdit::LineEdit * Window::getLineEditLongArifmetic( \
		void) const noexcept {
		return _lineEditLongArifmetic;
	}
	inline void Window::setSettingDock( \
		Setting::SettingDock * settingDock \
	) noexcept {
		_settingDock = settingDock;
		return;
	}
	inline Setting::SettingDock * Window::getSettingDock( \
		void) const noexcept {
		return _settingDock;
	}
	inline void Window::setRGB( \
		byte red0, byte green0, byte blue0, \
		byte red1, byte green1, byte blue1 \
	) noexcept {
		_rgb0.set(red0, green0, blue0);
		_rgb1.set(red1, green1, blue1);
		return;
	}
	inline void Window::setRGB( \
		RGB rgb0, RGB rgb1 \
	) noexcept {
		_rgb0.set(rgb0.red(), rgb0.green(), rgb0.blue());
		_rgb1.set(rgb1.red(), rgb1.green(), rgb1.blue());
		return;
	}
	inline void Window::paintEvent(QPaintEvent *event) {
        Q_UNUSED(event);
        
        QPainter painter(this);
        
        // Создаем радиальный градиент
        QRadialGradient gradient(rect().center(), 
                                qMax(width(), height()) / 2);
        
        gradient.setColorAt(0, QColor( \
			_rgb0.red(), _rgb0.green(), _rgb0.blue()));
        gradient.setColorAt(1, QColor( \
			_rgb1.red(), _rgb1.green(), _rgb1.blue()));
        
        // Заливаем фон градиентом
        painter.fillRect(rect(), gradient);
        
        // Важно: вызываем родительский paintEvent для отрисовки дочерних виджетов
        QMainWindow::paintEvent(event);
    }
	inline Window::~Window(void) {
		LineEdit::LineEdit *** ptrLineEdit \
			{const_cast<LineEdit::LineEdit ***>(_lineEdit)};
		const char ***ptrResult \
			{const_cast<const char ***>(_result)};
		deleteResultOrLineEdit(ptrLineEdit);
		deleteResultOrLineEdit(ptrResult);
		size_t len {COUNT_LOCAL_HISTORI};
		CreateHistori::HistoriScroll ** \
			ptrLocalHistori{_localHistori};
		CreateHistori::HistoriWidget ** \
			ptrResizeLocalHistori{_resizeLocalHistori};
		CreateHistori::HistoriVBox ** \
			ptrAddLocalHistori{_addLocalHistori};
		for (; len--; ptrLocalHistori++, \
			ptrResizeLocalHistori++, \
			ptrAddLocalHistori++ \
		) {
			delete *ptrLocalHistori;
			delete *ptrResizeLocalHistori;
			delete *ptrAddLocalHistori;
			delete [] *ptrLineEdit;
		}
		delete [] _localHistori;
		delete [] _resizeLocalHistori;
		delete [] _addLocalHistori;

		delete [] _lineEdit;
		delete _resultButton;
		delete _globalHistori;
		delete _addGlobalHistori;
		delete _resizeGlobalHistori;
		QWidget::~QWidget();
		return;
	}
}